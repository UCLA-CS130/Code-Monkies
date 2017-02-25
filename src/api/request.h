#ifndef API__REQUEST_H
#define API__REQUEST_H

#include <memory>  // std::unique_ptr
#include <string>  // std::string
#include <utility> // std::pair
#include <vector>  // std::vector

using Header = std::pair<std::string, std::string>;
using Headers = std::vector<Header>;

// Represents an HTTP Request.
//
// Usage:
//   auto request = Request::Parse(raw_request);
class Request {
private:
    std::string method_;
    std::string uri_;
    std::string version_;
    std::string body_;

    Headers headers_;

public:
    static std::unique_ptr<Request> Parse(const std::string& raw_request);

    // Converts Request object to its string representation
    std::string raw_request() const;

    // Simple accessors
    std::string method() const 
    {
        return method_; 
    }
    std::string uri() const { return uri_; }
    std::string version() const { return version_; }
    Headers headers() const { return headers_; }
    std::string body() const { return body_; }

    // Simple mutators
    void SetMethod(std::string method) { method_ = method; }
    void SetUri(std::string uri) { uri_ = uri; }
    void SetVersion(std::string version) { version_ = version; }
    void SetHeaders(Headers headers) { headers_ = headers; }
    void AddHeader(Header header) { headers_.push_back(header); }
    void SetBody(std::string body) { body_ = body; }
};

#endif