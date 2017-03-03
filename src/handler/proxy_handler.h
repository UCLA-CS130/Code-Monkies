#ifndef PROXY_HANDLER_H
#define PROXY_HANDLER_H

#include <string>
#include <boost/asio.hpp>
#include <memory>
#include "api/request_handler.h"

#define HOST_NAME_ID "host"
#define HOST_PORT_ID "host_port"

class ProxyHandler : public RequestHandler {
 public:
  virtual ~ProxyHandler() {}
  
  virtual Status Init(const std::string& uri_prefix,
		      const NginxConfig& config);

  virtual Status HandleRequest(const Request& request,
			       Response* response);
 private:
  std::string uri_prefix_;
  NginxConfig const *conf_;

  void SetHostValues(std::vector<std::shared_ptr<NginxConfigStatement>>
		     handler_block_statements,
		     std::string& host_str,
		     std::string target);

  std::unique_ptr<Request> CreateProxyRequestFromClientRequest(const Request& request);
  void IssueProxyRequestAndGetResponse(std::string host_name,
				       std::string port_num, 
				       const Request& request, 
				       Response* response);
  
  void RewriteUrls(Response& res);
};

REGISTER_REQUEST_HANDLER(ProxyHandler);

#endif
