#ifndef DELAY_HANDLER_H
#define DELAY_HANDLER_H

#include <unistd.h>
#include "api/request_handler.h"

// Waits DELAY seconds before returning a 200 OK
// Should only be used in testing. Useless otherwise.
class DelayHandler : public RequestHandler {
private:
	const int DELAY = 5;

public:
	virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);

	virtual Status HandleRequest(const Request& request, Response* response);
};

REGISTER_REQUEST_HANDLER(DelayHandler);

#endif