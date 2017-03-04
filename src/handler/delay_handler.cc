#include "handler/delay_handler.h"
#include <sstream>

RequestHandler::Status DelayHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
	(void) uri_prefix;
	(void) config;

	return RequestHandler::Status::OK;
}

RequestHandler::Status DelayHandler::HandleRequest(const Request& request, Response* response) {
	(void) request;

	response->SetStatus(Response::ResponseCode::HTTP_200_OK);
	std::stringstream body;
	body << "Response delayed by " << DELAY << " seconds";
	response->SetBody(body.str());
	sleep(DELAY);

	return RequestHandler::Status::OK;
}