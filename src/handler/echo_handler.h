#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "handler/handler.h"
#include "http/request.h"
#include "http/response.h"

// Echos a request with Content-Type: text/plain
class EchoRequestHandler : public RequestHandler
{
  public:
    const std::string TEXT_PLAIN = "Content-Type: text/plain";

    EchoRequestHandler()
      : RequestHandler()
    {
    }
    virtual ~EchoRequestHandler() {};

    /*
     * This method should only fail in out-of-memory conditions.
     */
    virtual bool handle(const Request &request, Response *&response);
};

#endif
