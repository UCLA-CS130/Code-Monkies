#ifndef HANDLER_H
#define HANDLER_H

/*
 * All handler base classes live here.
 */

#include "http/request.h"
#include "http/response.h"

// Abstract base Handler for server
// It createes a Response based on the Request and type of Handler
class RequestHandler
{
  public:
    RequestHandler()
      : started_handling_(false), done_handling_(false)
    {
    }
    // Function for handler to execute when called
    virtual bool handle(const Request &request, Response *&response) = 0;
    virtual bool doneHandling() { return done_handling_; };
    virtual ~RequestHandler() {};
  protected:
    // Used to indicate what basic state the handler is in. Useful for handlers
    // which send back multiple messages.
    bool started_handling_;
    bool done_handling_;
};

#endif
