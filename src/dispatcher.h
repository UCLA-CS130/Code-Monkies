#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <unordered_map>

#include "api/request_handler.h"
#include "helpers.h"

using helpers::debugf;

/*
 * This class coordinates dispatch of Requests to RequestHandlers.
 * There is a singleton Dispatcher which contains the string->handler map
 * handler_map_. Dispatch looks up which handler to return.
 */
class Dispatcher
{
  public:
    static void Init()
    {
      handler_map_.emplace("EchoHandler",
          RequestHandler::CreateByName("EchoHandler"));
      handler_map_.emplace("StaticHandler",
          RequestHandler::CreateByName("StaticHandler"));
      handler_map_.emplace("NotFoundHandler",
          RequestHandler::CreateByName("NotFoundHandler"));
      handler_map_.emplace("ProxyHandler",
          RequestHandler::CreateByName("ProxyHandler"));
      handler_map_.emplace("DelayHandler",
          RequestHandler::CreateByName("DelayHandler"));
    }

    /*
     * Return the appropriate RequestHandler given the request and
     * configuration. Do a longest-prefix match on the given URI and the URIs
     * of each handler to determine which one is the best fit.
     */
    static bool Dispatch(const Request *req, const NginxConfig *conf,
        std::string &handler_name, RequestHandler *&handler);

    /*
     * Basically just a caching wrapper around RequestHandler::CreateByName.
     * Since request handlers are long-lived and this server is
     * single-threaded, we can trivially return references to our long-lived
     * handlers.
     * This method is particularly useful for handlers registering their
     * actions with the StatusHandler.
     */
    static bool GetHandler(const std::string& handler_name,
        RequestHandler *&handler)
    {
      if (handler_map_.find(handler_name) == handler_map_.end()) {
        debugf("Dispatcher::GetHandler", "Instantiating new handler late in "
            "server's lifetime. This is likely a bug.\n");
        handler = RequestHandler::CreateByName(handler_name.c_str());
        if (handler == nullptr) {
          debugf("Dispatcher::GetHandler", "Unknown request handler: %s.\n",
              handler_name.c_str());
          return false;
        }
        handler_map_.emplace(handler_name, handler);
      } else {
        handler = handler_map_[handler_name];
      }
      return true;
    }

  private:
    Dispatcher() {}

    static std::unordered_map<std::string, RequestHandler*> handler_map_;
};

#endif
