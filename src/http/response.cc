#include "http/constants.h"
#include "http/response.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

Response::Response(int status) {
	this->status_ = status;
	try {
		this->reason_ = status::getReasonPhrase(status);
	} catch (const std::invalid_argument& e) {
		std::cerr << status 
			<< " is not a valid status code for this class" 
			<< std::endl;
		exit(1);
	}
}

// Response first line uses this format
// <HTTP Ver> <Status Code> <Reason>
// We do not include the CRLF
std::string Response::buildFirstLine() {
	std::stringstream firstLine;

	firstLine << http::HTTP_VERSION << " "
				 << this->status_ << " "
				 << this->reason_;
	return firstLine.str();
}