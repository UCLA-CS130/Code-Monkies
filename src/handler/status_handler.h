#ifndef STATUS_HANDLER_H
#define STATUS_HANDLER_H

#include "api/request_handler.h"

#include <list>

/*
 * Handler responsible for generating webserver's status page.
 */
class StatusHandler : public RequestHandler
{
  public:
    virtual ~StatusHandler() {}

    virtual Status Init(const std::string& uri_prefix,
        const NginxConfig& config);

    virtual Status HandleRequest(const Request& request,
        Response* response);

  private:
    /*
     * List of all handlers' entries in the server's configuration.
     */
    std::list<NginxConfigStatement const *> handler_info_;
};

REGISTER_REQUEST_HANDLER(StatusHandler);

#endif
