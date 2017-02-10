#include "handler/echo_handler.h"
#include "http/request.h"
#include "http/response.h"
#include "http/constants.h"
#include <string>

// Convert Request to string and dump it in the body
bool EchoRequestHandler::handle(const Request &request, Response *&response) {
  started_handling_ = true;
	std::string body = request.build();
	response = new Response(status::HTTP_200_OK);
	response->addHeader(TEXT_PLAIN);
	response->setBody(body);

  done_handling_ = true;
	return true;
}
