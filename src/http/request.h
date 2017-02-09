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
	Request() {}
	Request(std::string method, std::string uri)
		: method_(method), uri_(uri) {}
	virtual ~Request() {};
	virtual std::string buildFirstLine();

	std::string getUri() { return uri_; }
	std::string getMethod() { return method_; }
	
	void setUri(std::string uri) { uri_ = uri; }
	void setMethod(std::string method) { method_ = method; }

	// Consumes a request string and sets the fields of the 
	// Request object
	void consume(std::string requestString);
};

#endif