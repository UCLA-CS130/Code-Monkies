#ifndef API___RESPONSE_H
#define API___RESPONSE_H

#include <string>  // std::string
#include <utility> // std::pair
#include <vector>  // std::vector
#include <memory>
#include <boost/algorithm/string.hpp>

using Header = std::pair<std::string, std::string>;
using Headers = std::vector<Header>;

// Represents an HTTP response.
//
// Usage:
//   Response r;
//   r.SetStatus(OK);
//   r.SetBody(...);
//   return r.ToString();
//
// Constructed by the RequestHandler, after which the server should call ToString
// to serialize.
class Response {
public:
    Response() : version_("HTTP/1.1") {}
    ~Response() = default;

    enum ResponseCode {
        HTTP_200_OK = 200,
        HTTP_302_FOUND = 302,
        HTTP_404_NOT_FOUND = 404
    };
  
    // for reverse proxy handler
    static std::unique_ptr<Response> Parse(const std::string& raw_response);
    static ResponseCode string_to_resp_code(const int status_code);
  
    // Simple accesors
    std::string version() { return version_; }
    ResponseCode status() { return status_; }
    std::string status_message() { return status_message_; }
    Headers headers() { return headers_; }
    std::string body() { return body_; }

    // Simple mutators 
    void SetStatus(const ResponseCode& response_code)
    {
        status_ = response_code;
        status_message_ = status_message_from_code(response_code);
    }
  
    void SetVersion(const std::string version)
    {
      version_ = version;
    }

    void AddHeader(const std::string& header_name, const std::string& header_value) 
    {
        headers_.push_back(std::make_pair(header_name, header_value));
    }

    void AddHeader(const Header& header) 
    {
        headers_.push_back(header); 
    }

    void SetBody(const std::string& body)
    {
        body_ = body;
    }
    
    std::string ToString() const;

private:
    std::string version_;
    ResponseCode status_;
    std::string status_message_;

    Headers headers_;

    std::string body_;

    // Gets status_message for a specific response_code
    // Ex: status_message_from_code(OK) // returns "OK"
    std::string status_message_from_code(const ResponseCode& response_code);
};

#endif
