#include "handler/echo_handler.h"
#include <string>

// Convert Request to string and dump it in the body
bool EchoRequestHandler::handle(const std::unique_ptr<Request> &request, Response *&response) {
  	started_handling_ = true;

	std::string body = request->raw_request();
	response = new Response();
	response->SetStatus(Response::ResponseCode::HTTP_200_OK);
	response->AddHeader("Content-Type", "text/plain");
	response->SetBody(body);

  	done_handling_ = true;
	return true;
}
