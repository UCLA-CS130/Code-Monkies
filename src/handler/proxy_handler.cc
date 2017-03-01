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

	return RequestHandler::Status::OK;
}
