#ifndef HANDLER_H
#define HANDLER_H

#include "http/request.h"
#include "http/response.h"

// Abstract base Handler for server
// It createes a Response based on the Request and type of Handler
class RequestHandler {
public:
	// Function for handler to execute when called
	virtual Response handle(Request request) = 0;
	virtual ~RequestHandler() {};
};

#endif