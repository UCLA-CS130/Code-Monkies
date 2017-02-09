#include "handler/echo_handler.h"
#include "http/request.h"
#include "http/response.h"
#include "http/constants.h"
#include <string>

// Convert Request to string and dump it in the body
Response EchoRequestHandler::handle(Request request) {
	std::string body = request.build();
	Response response(status::HTTP_200_OK);
	response.addHeader(TEXT_PLAIN);
	response.setBody(body);

	return response;
}