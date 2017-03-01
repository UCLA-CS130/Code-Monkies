#ifndef PROXY_HANDLER_H
#define PROXY_HANDLER_H

#include "api/request_handler.h"

// Echos a request with Content-Type: text/plain
class ProxyHandler : public RequestHandler
{
public:
	virtual ~ProxyHandler() {}

  	virtual Status Init(const std::string& uri_prefix,
                        const NginxConfig& config);

  	virtual Status HandleRequest(const Request& request,
                                 Response* response);
};

REGISTER_REQUEST_HANDLER(ProxyHandler);

#endif
