#include "handler/proxy_handler.h"
#include "config.h"
#include <fstream>

using boost::asio::ip::tcp;

RequestHandler::Status ProxyHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {

  conf_ = &config;
	uri_prefix_ = uri_prefix;

	return RequestHandler::Status::OK;
}

void ProxyHandler::SetHostValues(
  std::vector<std::shared_ptr<NginxConfigStatement>>
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

  (void) response;

  SetHostValues(handler_block_statements, host_name, HOST_NAME_ID);
  SetHostValues(handler_block_statements, host_port, HOST_PORT_ID);

  if (host_name == "" || host_port == "") {
    printf("Invalid config\n");
    return RequestHandler::Status::GENERIC_ERROR;
  }

  Request proxy_req = CreateProxyRequestFromClientRequest(request);

//  printf("Sending request\n%s\n", proxy_req.raw_request().c_str());

  IssueProxyRequestAndGetResponse(host_name, host_port, proxy_req, response);
  printf("Request done!\n");
	return RequestHandler::Status::OK;
}


Request ProxyHandler::CreateProxyRequestFromClientRequest(
  const Request& request){

  Request* new_request = new Request();
  new_request->SetMethod(request.method());

  std::string proxy_uri = request.uri();
  proxy_uri.erase(0, uri_prefix_.length());

  if (proxy_uri == "") {
    proxy_uri = "/";
  }

  new_request->SetUri(proxy_uri);
  new_request->SetVersion(request.version());
  new_request->SetHeaders(request.headers());
  new_request->SetBody(request.body());
  return *new_request;
}

// https://gist.github.com/vladon/8b487e41cb3b49e172db
std::string buffer_to_string(const boost::asio::streambuf &buffer) {
  using boost::asio::buffers_begin;

  auto bufs = buffer.data();
  std::string result(buffers_begin(bufs), buffers_begin(bufs) + buffer.size());
  return result;
}

void ProxyHandler::IssueProxyRequestAndGetResponse(std::string host_name,
  std::string port_num, const Request& request, Response* response) {
    (void) response;
    (void) request;

  boost::asio::io_service io_service;
  tcp::resolver resolver(io_service);

  tcp::socket sock(io_service);
  boost::asio::ip::tcp::resolver::query query(host_name, port_num);
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

  //boost::asio::ip::tcp::resolver::iterator i = resolver.resolve(query);
  //boost::asio::ip::tcp::endpoint end = *i;
  //printf("Endpoint: %s", end.address().to_string().c_str());
  boost::asio::connect(sock, endpoint_iterator);
/*
  // send request
  write(sock, boost::asio::buffer(request.raw_request()));

  boost::asio::streambuf response_buff;
  size_t len = read_until(sock, response_buff, boost::asio::error::eof);

  printf("Response received: it is\n%s", buffer_to_string(response_buff).c_str());*/
}
