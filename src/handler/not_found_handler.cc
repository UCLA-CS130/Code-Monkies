#include "handler/not_found_handler.h"
#include <string>

// Convert Request to string and dump it in the body
bool NotFoundHandler::handle(const std::unique_ptr<Request> &request, Response *&response) {
	// Ignore unused error
	(void) request;

  	started_handling_ = true;

  	// TODO null check allocation
	response = new (std::nothrow) Response();
	response->SetStatus(Response::ResponseCode::HTTP_404_NOT_FOUND);
	response->AddHeader("Content-Type", "text/plain");
  	std::string body = "404: File not found.\n";
	response->SetBody(body);

  	done_handling_ = true;
	return true;
}
