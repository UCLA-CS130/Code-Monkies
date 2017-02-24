#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "api/request_handler.h"

// Echos a request with Content-Type: text/plain
class EchoHandler : public RequestHandler
{
public:
	virtual ~EchoHandler() {}

  	virtual Status Init(const std::string& uri_prefix,
                        const NginxConfig& config);

  	virtual Status HandleRequest(const Request& request,
                                 Response* response);
};

REGISTER_REQUEST_HANDLER(EchoHandler);

#endif
