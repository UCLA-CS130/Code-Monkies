#include "config.h"
#include "handler/status_handler.h"
#include "request_logger.h"

#include <ctime>
#include <string>
#include <sstream>

RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {
  // neither of these are needed. squash Wunused with void cast.
	(void) uri_prefix;
	(void) config;

  if (handler_info_.size() == 0) {
    Config::GetAllHandlerInfo(&config, handler_info_);
  }

	return RequestHandler::Status::OK;
}

RequestHandler::Status StatusHandler::HandleRequest(const Request& request, 
					 Response* response) {
  (void) request;

	std::stringstream body;
  const std::list<RequestInfo> *req_log =
    RequestLogger::GetInstance().GetLog();

  body << "Code Monkies Webserver Status Page" << std::endl << std::endl;

  body << "Requests served:" << std::endl;
  body << "  ";
  body << "time served             " << " | ";
  body << "handler name            " << " | ";
  body << "url                             " << " | ";
  body << "response code" << std::endl;
  for (auto const& req : *req_log) {
    body << "  ";
    body << req.time_finished << " | ";
    body << req.handler_name << " | ";
    body << req.url << " | ";
    body << req.response_code << std::endl;
  }
  body << std::endl;

  body << "Handlers available:" << std::endl;
  body << "  uri prefix -> handler name" << std::endl;
  for (auto const& stmt : handler_info_) {
    if (stmt->tokens_[0] == "path") {
      body << "  " << stmt->tokens_[1] << " -> " << stmt->tokens_[2];
      body << std::endl;
    } else if (stmt->tokens_[0] == "default") {
      body << "  * -> " << stmt->tokens_[1] << std::endl;
    }
  }

	response->SetStatus(Response::ResponseCode::HTTP_200_OK);
	response->AddHeader("Content-Type", "text/plain");
	response->SetBody(body.str());

	return RequestHandler::Status::OK;
}
