#ifndef NOT_FOUND_HANDLER_H
#define NOT_FOUND_HANDLER_H

#include "handler.h"

// Handles sending 404's back to the client.
class NotFoundHandler : public RequestHandler
{
  public:
    NotFoundHandler() {};
    virtual ~NotFoundHandler() {};

    virtual bool handle(const std::unique_ptr<Request> &request, Response *&response);
};

#endif
