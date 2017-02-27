#include "config.h"
#include "dispatcher.h"

std::unordered_map<std::string, RequestHandler*> Dispatcher::handler_map_;

bool Dispatcher::Dispatch(const Request *req,
    const NginxConfig *conf, std::string &handler_name,
    RequestHandler *&handler)
{
  NginxConfigStatement *info;

  if (!Config::GetHandlerInfo(conf, req, info)) {
    handler_name = "NotFoundHandler";
  } else {
    handler_name = info->tokens_[2];
  }

  /*
   * Get handler from static instance.
   */
  if (!GetHandler(handler_name, handler)) {
    debugf("Dispatcher::Dispatch", "Failed to get handler: %s.\n",
        handler_name.c_str());
    return false;
  }

  return true;
}
