#ifndef STATUS_H
#define STATUS_H

#include <iostream>
#include <string>
#include <stdexcept>

/**
 * Non-exhaustive constants file for different HTTP status
 * These status are added on an at need basis. 
 * Currently the only supported statuses are 200 and 404
 * 
 * Heavily inspired by
 * https://github.com/tomchristie/django-rest-framework/blob/master/rest_framework/status.py
 */
namespace status {
	const int HTTP_200_OK = 200;
	const int HTTP_404_NOT_FOUND = 404;

	inline std::string getReasonPhrase(int fromStatus) {
		switch(fromStatus) {
			case HTTP_200_OK: return "OK";
			case HTTP_404_NOT_FOUND: return "NOT FOUND";
			default: 
				throw std::invalid_argument("invalid or unimplemented status code");
		}
	}
} // namespace status

namespace http {
	const std::string CRLF = "\r\n";
	const std::string HTTP_VERSION = "HTTP/1.1";
} // namespace http

#endif // STATUS_H