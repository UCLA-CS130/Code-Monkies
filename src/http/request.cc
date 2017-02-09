#include "http/constants.h"
#include "http/request.h"
#include <string>
#include <sstream>

// Request first line follows this format
// <Request Method> <Path> <HTTP Version>
// We do not include the CRLF
std::string Request::buildFirstLine() {
	std::stringstream firstLine;

	firstLine << this->method_ << " "
				 << this->uri_ << " "
				 << http::HTTP_VERSION;
	return firstLine.str();
}