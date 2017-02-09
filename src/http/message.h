#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>

/*
 * Interface for HTTP messages. Used to hold common code
 * for HTTP Requests and Responses
 */
class Message {
protected:
	std::vector<std::string> headers_;
	std::string body_;

public:
	Message() : headers_() {};
	virtual ~Message() {};

	// Virtual definition for the first line of a 
	// Request or Response
	virtual std::string buildFirstLine() = 0;

	// Creates the entire HTTP message from the first line, headers, and body
	// and outputs it as a string.
	std::string build();

	// Basic accessors
	std::vector<std::string> getHeaders() const;
	std::string getBody() const;

	// Basic setter functions 
	void addHeader(std::string header);
	void addHeaders(const std::vector<std::string>& headers);
	bool removeHeader(std::string header);	
	void setBody(std::string body);
};

#endif