#ifndef REQUEST_H
#define REQUEST_H

#include "http/message.h"
#include <string>

class Request : public Message {
protected:
	// HTTP request method
	std::string method_;

	// Request URI path for file
	std::string uri_;

public:
	Request(std::string method, std::string uri)
		: method_(method), uri_(uri) {}
	virtual ~Request() {};
	virtual std::string buildFirstLine();
};

#endif