#ifndef RESPONSE_H
#define RESPONSE_H

#include "http/message.h"
#include <string>

class Response : public Message {
protected:
	int status_;
	std::string reason_;
public:
	Response(int status);
	virtual ~Response() {};
	virtual std::string buildFirstLine();

	std::string getReason() const { return reason_; }

	int getStatus() const { return status_; }
};

#endif