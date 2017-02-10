#ifndef NOT_FOUND_HANDLER_H
#define NOT_FOUND_HANDLER_H

#include "handler.h"

// Handles sending 404's back to the client.
class NotFoundHandler : public RequestHandler
{
  public:
    const std::string TEXT_PLAIN = "Content-Type: text/plain";

    NotFoundHandler() {};
    virtual ~NotFoundHandler() {};

    virtual bool handle(const Request&, Response *&response);
};

#endif
