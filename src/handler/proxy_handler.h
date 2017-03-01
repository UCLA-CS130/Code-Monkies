#ifndef PROXY_HANDLER_H
#define PROXY_HANDLER_H

#include "api/request_handler.h"
#include <string>
#include<boost/asio.hpp>

#define HOST_NAME_ID "host"
#define HOST_PORT_ID "host_port"

class ProxyHandler : public RequestHandler
{
public:
	virtual ~ProxyHandler() {}

  	virtual Status Init(const std::string& uri_prefix,
                        const NginxConfig& config);

  	virtual Status HandleRequest(const Request& request,
                                 Response* response);
  
    Request createRequestFromRequest(const Request& request);
    void send_something(std::string host_name, int port, std::string message);
    std::string handle_resolve_query(std::string host_name);

private:
  std::string uri_prefix_;
  NginxConfig const *conf_;
  void SetHostValues(std::vector<std::shared_ptr<NginxConfigStatement>>
      handler_block_statements,
      std::string& host_str,
      std::string target);
};

REGISTER_REQUEST_HANDLER(ProxyHandler);

#endif
