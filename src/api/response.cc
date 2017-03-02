#include "api/response.h"
#include <stdexcept>
#include <sstream>

std::string Response::status_message_from_code(const ResponseCode& response_code) {
	switch (response_code) {
		case ResponseCode::HTTP_200_OK: return "200 OK";
		case ResponseCode::HTTP_302_FOUND: return "302 FOUND";
		case ResponseCode::HTTP_404_NOT_FOUND: return "404 NOT_FOUND";
		default:
			throw std::invalid_argument("Invalid or unimplemented response code");
	}
}

std::string Response::ToString() const {
	// Stringstream to stream string parts into
	std::stringstream raw_response_string;

	raw_response_string \
		<< version_ << " "
		<< status_message_;

	// Add carriage return before each header
	for (unsigned i = 0; i < this->headers_.size(); i++) {
		auto header = headers_[i];
		raw_response_string << "\r\n" << header.first << ": " << header.second;
	}

	// Add two carriage returns before the body
	raw_response_string << "\r\n\r\n" << this->body_;

	return raw_response_string.str();
}

std::unique_ptr<Response> Response::Parse(const std::string& raw_response){
  std::istringstream raw_response_stream(raw_response);
  
  // Used to read the headers
  std::string header_string;
  std::string lineTemp;
  
  std::string version;
  std::string status_code;
  std::string status;
  
  Headers headers;
  std::string body;
  
  std::getline(raw_response_stream, version, ' ');
  std::getline(raw_response_stream, status_code, ' ');
  std::getline(raw_response_stream, status, '\r');
  std::getline(raw_response_stream, lineTemp, '\n');
  std::getline(raw_response_stream, header_string, '\r');
  
  std::unique_ptr<Response> response ( new Response() );
  
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
    response->AddHeader(header);
    
    std::getline(raw_response_stream, lineTemp, '\n');
    std::getline(raw_response_stream, header_string, '\r');
  }
  
  std::getline(raw_response_stream, lineTemp, '\n');
  std::getline(raw_response_stream, body, '\0');
  raw_response_stream.clear();
  
  response->SetVersion(version);
  response->SetStatus(string_to_resp_code(std::stoi(status_code)));
  response->SetBody(body);
  
  return response;

}

Response::ResponseCode Response::string_to_resp_code(const int status_code){
  switch(status_code){
    case 200:
      return ResponseCode::HTTP_200_OK;
    case 302:
      return ResponseCode::HTTP_302_FOUND;
    default:
      return ResponseCode::HTTP_404_NOT_FOUND;
  }
}
