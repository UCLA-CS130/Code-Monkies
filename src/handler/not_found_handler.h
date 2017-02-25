#ifndef NOT_FOUND_HANDLER_H
#define NOT_FOUND_HANDLER_H

#include "api/request_handler.h"

// Handles sending 404's back to the client.
class NotFoundHandler : public RequestHandler
{
public:
	virtual ~NotFoundHandler() {}

	virtual Status Init(const std::string& uri_prefix,
    	                const NginxConfig& config);

	virtual Status HandleRequest(const Request& request,
        	                     Response* response);
};

REGISTER_REQUEST_HANDLER(NotFoundHandler);

#endif
