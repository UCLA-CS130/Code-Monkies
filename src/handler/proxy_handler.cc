#include "handler/proxy_handler.h"
#include "config.h"

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
  } else {
    printf("Host_name: %s, Host_port: %s", host_name.c_str(), host_port.c_str());
  }

  printf("DNS RESULT: %s\n", handle_resolve_query(host_name).c_str());

	return RequestHandler::Status::OK;
}


Request createRequestFromRequest(const Request& request){
  Request* new_request = new Request();
  new_request->SetMethod(request.method());
  new_request->SetUri(request.uri());
  new_request->SetVersion(request.version());
  new_request->SetHeaders(request.headers());
  new_request->SetBody(request.body());
  return *new_request;
}

// inspired by https://gist.github.com/bechu/2423333
void ProxyHandler::send_something(std::string host_name, int port, std::string message){
  boost::asio::io_service ios;

  std::string host_ip = handle_resolve_query(host_name);
//  printf("host: %s", host_ip.c_str());
  (void) port;
  (void) message;
//  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_ip), port);
//
//  boost::asio::ip::tcp::socket socket(ios);
//
//  socket.connect(endpoint);
//
//  boost::array<char, 128> buf;
//  std::copy(message.begin(),message.end(),buf.begin());
//  boost::system::error_code error;
//  socket.write_some(boost::asio::buffer(buf, message.size()), error);

  // TODO: get response

//  socket.close();
}

// inspired by http://stackoverflow.com/questions/5486113/how-to-turn-url-into-ip-address-using-boostasio
std::string ProxyHandler::handle_resolve_query(std::string host_name) {
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query(host_name, "");
  for(boost::asio::ip::tcp::resolver::iterator i = resolver.resolve(query);
      i != boost::asio::ip::tcp::resolver::iterator();
      ++i)
  {
    boost::asio::ip::tcp::endpoint end = *i;

    return end.address().to_string();
  }
  return "";
}
