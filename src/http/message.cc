#include "http/constants.h"
#include "http/message.h"
#include <vector>
#include <string>
#include <sstream>

std::string Message::build() {
	// Stringstream to stream string parts into
	std::stringstream message;

	std::string firstLine = this->buildFirstLine();
	message << firstLine;

	// Add carriage return before each header
	for (unsigned i = 0; i < this->headers_.size(); i++) {
		message << http::CRLF << headers_[i];
	}

	// Add two carriage returns before the body
	message << http::CRLF << http::CRLF << this->body_;

	return message.str();
}

std::vector<std::string> Message::getHeaders() const {
	return this->headers_;
}

std::string Message::getBody() const {
	return this->body_;
}

void Message::addHeader(std::string header) {
	this->headers_.push_back(header);
}

void Message::addHeaders(const std::vector<std::string>& headers) {
	this->headers_.insert(
		this->headers_.end(),
		headers.begin(),
		headers.end()
	);
}

bool Message::removeHeader(std::string header) {
	for (unsigned i = 0; i < this->headers_.size(); i++) {
		std::string s = this->headers_[i];

		// If we find the desired string in the headers vector, remove it
		if (header.compare(s) == 0) {
			this->headers_.erase(this->headers_.begin() + i);
			return true;
		}
	}

	// Did not find any match, return false to let caller know it failed
	return false;
}

void Message::setBody(std::string body) {
	this->body_ = body;
}