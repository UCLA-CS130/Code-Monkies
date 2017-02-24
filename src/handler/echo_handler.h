#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "handler/handler.h"

// Echos a request with Content-Type: text/plain
class EchoRequestHandler : public RequestHandler
{
  public:
    EchoRequestHandler()
      : RequestHandler()
    {
    }
    virtual ~EchoRequestHandler() {};

    /*
     * This method should only fail in out-of-memory conditions.
     */
    virtual bool handle(const std::unique_ptr<Request> &request, Response *&response);
};

#endif
