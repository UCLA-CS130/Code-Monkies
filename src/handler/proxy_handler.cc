#include <fstream>
#include <boost/regex.hpp>
#include "handler/proxy_handler.h"
#include "config.h"

using boost::asio::ip::tcp;

RequestHandler::Status ProxyHandler::Init(const std::string& uri_prefix,
					  const NginxConfig& config) {
  conf_ = &config;
  uri_prefix_ = uri_prefix;
  return RequestHandler::Status::OK;
}

void ProxyHandler::SetHostValues(std::vector<std::shared_ptr<NginxConfigStatement>>
				 handler_block_statements,
				 std::string& host_str,
				 std::string target) {

  if (handler_block_statements[0]->tokens_[0] == target) {
    host_str = handler_block_statements[0]->tokens_[1];
  }

  if (handler_block_statements[1]->tokens_[0] == target) {
    host_str = handler_block_statements[1]->tokens_[1];
  }
}

RequestHandler::Status ProxyHandler::HandleRequest(const Request& request,
						   Response* response) {
  NginxConfigStatement* handler_info;

  if (!Config::GetHandlerInfo(conf_, &request, handler_info)) {
    printf("Unable to get handler info\n");
    return RequestHandler::Status::GENERIC_ERROR;
  }

  std::vector<std::shared_ptr<NginxConfigStatement>> handler_block_statements =
    handler_info->child_block_->statements_;

  if (handler_block_statements.size() < 2) {
    printf("Invalid number of statements in ProxyHandler block\n");
    return RequestHandler::Status::GENERIC_ERROR;
  }

  std::string host_name = "";
  std::string host_port = "";

  SetHostValues(handler_block_statements, host_name, HOST_NAME_ID);
  SetHostValues(handler_block_statements, host_port, HOST_PORT_ID);

  if (host_name == "" || host_port == "") {
    printf("Invalid config\n");
    return RequestHandler::Status::GENERIC_ERROR;
  }
  
  std::unique_ptr<Request> proxy_req = CreateProxyRequestFromClientRequest(request);

  try {
    IssueProxyRequestAndGetResponse(host_name, host_port, *proxy_req, response);
  } catch (std::exception& e) {
    std::cerr << "Error sending proxy request: " << e.what() << std::endl;
    return RequestHandler::Status::GENERIC_ERROR;
  }

  printf("Request done!\n");
  return RequestHandler::Status::OK;
}

std::unique_ptr<Request> ProxyHandler::CreateProxyRequestFromClientRequest(const Request& request) {
  std::string proxy_uri = request.uri();
  proxy_uri.erase(0, uri_prefix_.length());

  if (proxy_uri == "") {
    proxy_uri = "/";
  }

  std::unique_ptr<Request> new_request(new Request());
  new_request->SetMethod(request.method());
  new_request->SetUri(proxy_uri);  
  new_request->SetVersion("HTTP/1.0");  // Disable chunked HTTP/1.1 Encoding
  new_request->SetBody(request.body());

  // Disable headers that make proxying difficult
  std::vector<Header> non_encoding_headers;  
  non_encoding_headers.push_back(std::make_pair("Connection", "close"));
  new_request->SetHeaders(non_encoding_headers);
  return new_request;
}

// https://gist.github.com/vladon/8b487e41cb3b49e172db
std::string buffer_to_string(const boost::asio::streambuf &buffer) {
  using boost::asio::buffers_begin;

  auto bufs = buffer.data();
  std::string result(buffers_begin(bufs), buffers_begin(bufs) + buffer.size());
  return result;
}

void ProxyHandler::IssueProxyRequestAndGetResponse(std::string host_name,
						   std::string port_num, 
						   const Request& request, 
						   Response* response) {
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver(io_service);
  
  boost::asio::ip::tcp::socket sock(io_service);
  boost::asio::ip::tcp::resolver::query query(host_name, port_num);
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  boost::asio::connect(sock, endpoint_iterator);

  boost::asio::ip::tcp::endpoint end = *endpoint_iterator;
  printf("Endpoint: %s", end.address().to_string().c_str());

  std::cout << "Host: " << host_name << std::endl;
  std::cout << "Port: " << port_num << std::endl;
  std::cout << "Sending proxy request: " << std::endl << request.raw_request() << std::endl;

  write(sock, boost::asio::buffer(request.raw_request()));
  
  // Lazy way to read response. Just read everything in and parse it later.
  std::string ser_resp = "";
  boost::system::error_code error;
  boost::asio::streambuf response_buff;
  while (boost::asio::read(sock, 
			   response_buff,
			   boost::asio::transfer_at_least(1), 
			   error)) {}
  
  if (error != boost::asio::error::eof) {
    throw boost::system::system_error(error);
  }
  
  ser_resp = buffer_to_string(response_buff);
  std::cout << "Ser resp: " << std::endl << ser_resp << std::endl;

  std::unique_ptr<Response> resp = Response::Parse(ser_resp);  
  
  std::vector<Header> stripped_headers;
  for (auto header : resp->headers()) {
    std::string key = header.first;    
    if (key != "Content-Type") {
      continue;
    }

    stripped_headers.push_back(header);
  }
 
  resp->SetHeaders(stripped_headers);   

  RewriteUrls(*resp);
  *response = *resp;  
  sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
}

void ProxyHandler::RewriteUrls(Response& resp) {
  std::string body = resp.body();
  
  std::string reg_url = "\\s*=\\s*\"/?((?!http)[^\"]*)\"";

  boost::regex r("src" + reg_url);
  std::string fmt = "src=\"" + uri_prefix_ + "/\\1\"";

  boost::regex r2("href" + reg_url);
  std::string fmt2 = "href=\"" + uri_prefix_ + "/\\1\"";
  
  std::string replaced  = boost::regex_replace(body, r, fmt, boost::match_default | boost::format_sed);
  body = boost::regex_replace(replaced, r2, fmt2, boost::match_default | boost::format_sed);
  resp.SetBody(body);
}
