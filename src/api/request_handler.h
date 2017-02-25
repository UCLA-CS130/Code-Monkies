#ifndef API__REQUEST_HANDLER_H
#define API__REQUEST_HANDLER_H

#include "api/request.h"
#include "api/response.h"
#include "config_parser.h"
#include <map>
#include <memory>
#include <string>

// Represents the parent of all request handlers. Implementations should expect to
// be long lived and created at server constrution.
class RequestHandler {
public:
  enum Status {
    OK = 0,
    GENERIC_ERROR = 500 // TODO: make more granular errors
  };
  
  virtual ~RequestHandler() {}

  static RequestHandler* CreateByName(const char* type);

  // Initializes the handler. Returns a response code indicating success or
  // failure condition.
  // uri_prefix is the value in the config file that this handler will run for.
  // config is the contents of the child block for this handler ONLY.
  virtual Status Init(const std::string& uri_prefix,
                      const NginxConfig& config) = 0;

  // Handles an HTTP request, and generates a response. Returns a response code
  // indicating success or failure condition. If ResponseCode is not OK, the
  // contents of the response object are undefined, and the server will return
  // HTTP code 500s.
  virtual Status HandleRequest(const Request& request,
                               Response* response) = 0;
};


extern std::map<std::string, RequestHandler* (*)(void)>* request_handler_builders;
template<typename T>
class RequestHandlerRegisterer {
 public:
  RequestHandlerRegisterer(const std::string& type) {
    if (request_handler_builders == nullptr) {
      request_handler_builders = new std::map<std::string, RequestHandler* (*)(void)>;
    }
    (*request_handler_builders)[type] = RequestHandlerRegisterer::Create;
  }
  static RequestHandler* Create() {
    return new T;
  }
};

// Special macro that registers a subclass of RequestHandler with the Registerer.
// Allows for static creation via Request::CreateByName("$ClassName");
// Ex:
// class SomethingRequestHandler: public RequestHandler {
// ...
// };
// REGISTER_REQUEST_HANDLER(SomethingRequestHandler);
// ....
// auto handler = RequestHandler::CreateByName("SomethingRequestHandler");
#define REGISTER_REQUEST_HANDLER(ClassName) \
  static RequestHandlerRegisterer<ClassName> ClassName##__registerer(#ClassName)


#endif
