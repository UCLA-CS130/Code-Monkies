#include "handler/echo_handler.h"
#include <string>

RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {
	// EchoRequestHandler does not actually need any information
	// to be initialized. We void cast the arguments to prevent
	// W-unused-variable from getting mad
	(void) uri_prefix;
	(void) config;

	return RequestHandler::Status::OK;
}

RequestHandler::Status EchoHandler::HandleRequest(const Request& request, 
					 Response* response) {

	std::string body = request.raw_request();
	response->SetStatus(Response::ResponseCode::HTTP_200_OK);
	response->AddHeader("Content-Type", "text/plain");
	response->SetBody(body);

	return RequestHandler::Status::OK;
}
