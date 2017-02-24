#include "api/response.h"
#include <stdexcept>
#include <sstream>

std::string Response::status_message_from_code(const ResponseCode& response_code) {
	switch (response_code) {
		case ResponseCode::HTTP_200_OK: return "OK";
		case ResponseCode::HTTP_302_FOUND: return "FOUND";
		case ResponseCode::HTTP_404_NOT_FOUND: return "NOT_FOUND";
		default:
			throw std::invalid_argument("Invalid or unimplemented response code");
	}
}

std::string Response::ToString() {
	// Stringstream to stream string parts into
	std::stringstream raw_response_string;

	raw_response_string \
		<< version_ << " "
		<< status_ << " "
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