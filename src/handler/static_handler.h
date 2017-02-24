#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include "api/request_handler.h"
#include <iostream>
#include <fstream>
#include <string>

// Request handler for serving a static file
// File handler can create 200 responses if the file is found
// or 404 if the file requested is not found
class StaticHandler : public RequestHandler {
private:
	std::string root_;

	// TODO: TEMPORARY MEASURE TO PASS TESTS
	std::string file_path_;

public:
	virtual ~StaticHandler() {}

	virtual Status Init(const std::string& uri_prefix,
	                    const NginxConfig& config);

	virtual Status HandleRequest(const Request& request,
	                             Response* response);

	// Helper functions
	std::string getFileExtension(const std::string& fileName);
	Header getContentTypeFromExtension(const std::string& extension);
};

REGISTER_REQUEST_HANDLER(StaticHandler);

#endif
