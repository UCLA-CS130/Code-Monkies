#include "api/request.h"
#include <boost/algorithm/string.hpp>
#include <string>
#include <sstream>

// TODO: This function assumes a correctly formatted raw request
std::unique_ptr<Request> Request::Parse(const std::string& raw_request) {
  std::istringstream raw_request_stream(raw_request);

  // Used to read the headers
  std::string header_string;
  std::string lineTemp;

  std::string method;
  std::string uri;
  std::string version;

  Headers headers;
  std::string body;

  std::getline(raw_request_stream, method, ' ');
  std::getline(raw_request_stream, uri, ' ');
  std::getline(raw_request_stream, version, '\r');
  std::getline(raw_request_stream, lineTemp, '\n');
  std::getline(raw_request_stream, header_string, '\r');

  // Get headers
  std::string delimiter = ":";
  while (header_string != "") {
    // Split header string by colon
    Header header;
    auto colon_pos = header_string.find(delimiter);
    std::string key = header_string.substr(0, colon_pos);
    std::string value = header_string.substr(++colon_pos, header_string.size());
    value = boost::trim_left_copy(value);
    header = std::make_pair(key, value);
    headers.push_back(header);

    std::getline(raw_request_stream, lineTemp, '\n');
    std::getline(raw_request_stream, header_string, '\r');
  }

  std::getline(raw_request_stream, lineTemp, '\n');
  std::getline(raw_request_stream, body, '\0');
  raw_request_stream.clear();	   

  std::unique_ptr<Request> request (new Request());
  request->SetMethod(method);
  request->SetUri(uri);
  request->SetVersion(version);
  request->SetHeaders(headers);
  request->SetBody(body);
  return request;
}

std::string Request::raw_request() const {
  // Stringstream to stream string parts into
  std::stringstream raw_request_stream;

  raw_request_stream \
    << method_ << " "
    << uri_ << " "
    << version_;

  // Add carriage return before each header
  for (unsigned i = 0; i < this->headers_.size(); i++) {
    auto header = headers_[i];
    raw_request_stream << "\r\n" << header.first << ": " << header.second;
  }

  // Add two carriage returns before the body
  raw_request_stream << "\r\n\r\n" << this->body_;

  return raw_request_stream.str();
}
