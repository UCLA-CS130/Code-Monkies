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

  std::unique_ptr<Request> proxy_req = CreateProxyRequestFromClientRequest(request, host_name);

  try {
    IssueProxyRequestAndGetResponse(host_name, host_port, *proxy_req, response);
  } catch (std::exception& e) {
    std::cerr << "Error sending proxy request: " << e.what() << std::endl;
    return RequestHandler::Status::GENERIC_ERROR;
  }

  std::cout << "Response done!" << std::endl;
  return RequestHandler::Status::OK;
}

std::unique_ptr<Request> ProxyHandler::CreateProxyRequestFromClientRequest(const Request& request,
									   std::string host) {
  std::string proxy_uri = request.uri();
  if (uri_prefix_ != "/") {
    proxy_uri.erase(0, uri_prefix_.length());
  }

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
  non_encoding_headers.push_back(std::make_pair("Host", host));
  new_request->SetHeaders(non_encoding_headers);
  return new_request;
}

std::string read_socket(boost::asio::ip::tcp::socket* sock) {
  // Lazy way to read response. Just read everything in and parse it later.
  std::ostringstream ss;
  boost::system::error_code error;
  boost::asio::streambuf response_buff;
  while (boost::asio::read(*sock,
			   response_buff,
			   boost::asio::transfer_at_least(1),
			   error)) {
    ss << &response_buff;
  }

  if (error != boost::asio::error::eof) {
    throw boost::system::system_error(error);
  }

  return ss.str();
}

std::unique_ptr<Response> ProxyHandler::ForwardRequest(const Request& request,
						       std::string host_name,
						       std::string port_num) {

  std::cout << "Forwarding request to " << host_name << std::endl;

  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver(io_service);

  boost::asio::ip::tcp::socket sock(io_service);
  boost::asio::ip::tcp::resolver::query query(host_name, port_num,
					      boost::asio::ip::resolver_query_base::flags());
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  boost::asio::ip::tcp::resolver::iterator end;

  boost::system::error_code error = boost::asio::error::host_not_found;

  while (error && endpoint_iterator != end) {
      sock.close();
      sock.connect(*endpoint_iterator++, error);
  }

  std::cout << "Sending proxy request: " << std::endl << request.raw_request() << std::endl;
  write(sock, boost::asio::buffer(request.raw_request()));

  std::string ser_resp = read_socket(&sock);
  std::unique_ptr<Response> resp = Response::Parse(ser_resp);  
  return resp;
}

// http://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/tutorial/tutdaytime1.html
void ProxyHandler::IssueProxyRequestAndGetResponse(std::string host_name,
						   std::string port_num,
						   const Request& request,
						   Response* response) {
  std::unique_ptr<Response> forward_resp = ForwardRequest(request, host_name, port_num);
  if (forward_resp->status() == Response::HTTP_301_MOVED ||
      forward_resp->status() == Response::HTTP_302_FOUND) {          
    std::string move_loc = GetMoveLocation(*forward_resp);

    Request req_copy = request;
    req_copy.ReplaceHeader("Host", move_loc);

    Response dfs_resp;
    IssueProxyRequestAndGetResponse(move_loc, "80", req_copy, &dfs_resp);
    *forward_resp = dfs_resp;
  }

  ProcessRemoteResponse(*forward_resp);
  *response = *forward_resp;
}

void ProxyHandler::ProcessRemoteResponse(Response& resp) {
  RewriteUrls(resp);
  std::cout << resp.ToString() << std::endl;
}

void ProxyHandler::RewriteUrls(Response& resp) {
  std::string body = resp.body();
  
  std::string prefix = uri_prefix_;
  if (uri_prefix_.length() > 0 && 
      uri_prefix_[uri_prefix_.length() - 1] == '/') {
    prefix = "";
  }

  std::string reg_url = "\\s*=\\s*\"/?((?!http)[^\"]*)\"";

  boost::regex r("src" + reg_url);
  std::string fmt = "src=\"" + prefix + "/\\1\"";

  boost::regex r2("href" + reg_url); 
  std::string fmt2 = "href=\"" + prefix + "/\\1\"";

  std::string replaced = boost::regex_replace(body, r, fmt, boost::match_default | boost::format_sed);
  body = boost::regex_replace(replaced, r2, fmt2, boost::match_default | boost::format_sed);
  resp.SetBody(body);
}

std::string ProxyHandler::GetMoveLocation(const Response& resp) {
  for (auto header : resp.headers()) {
    std::string key = header.first;
    if (key == "Location") {
      std::string move_loc = header.second;	
      std::string http = "http://";
      std::string https = "https://";

      if (move_loc.find(http) == 0) {
	move_loc = move_loc.replace(0, http.length(), "");
      }

      if (move_loc.find(http) == 0) {
	move_loc = move_loc.replace(0, https.length(), "");
      }

      if (move_loc.back() == '/') {
	move_loc.pop_back();
      }

      return move_loc;
    }
  }

  return "";
}
