#include "http/constants.h"
#include "http/request.h"
#include <string>
#include <sstream>
#include <vector>



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

// TODO: This assumes the request is valid
// Need to handle edge cases
void Request::consume(std::string requestString) {
    std::istringstream requestStream(requestString);

    // Used to discard sections we do not need
    std::string DUMP = "";

    // Used to read the headers
    std::string header;
    std::string lineTemp;

	std::string method;
	std::string uri;
	std::vector<std::string> headers;
	std::string body;

    std::getline(requestStream, method, ' ');
    std::getline(requestStream, uri, ' ');
    std::getline(requestStream, DUMP, '\r');
    std::getline(requestStream, lineTemp, '\n');
    std::getline(requestStream, header, '\r');

    // Get headers
    while (header != "") {
    	headers.push_back(header);
		std::getline(requestStream, lineTemp, '\n');
		std::getline(requestStream, header, '\r'); // while check
    }

    std::getline(requestStream, lineTemp, '\n');
    std::getline(requestStream, body, '\0');
    requestStream.clear();	   

    std::cout << "Consumption complete" << std::endl;

    this->setMethod(method);
    this->setUri(uri);
    this->addHeaders(headers);
    this->setBody(body);
}