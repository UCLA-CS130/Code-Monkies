#include "http/constants.h"
#include "http/response.h"
#include <string>
#include <sstream>

Response::Response(int status) {
	this->status_ = status;
	this->reason_ = status::getReasonPhrase(status);
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