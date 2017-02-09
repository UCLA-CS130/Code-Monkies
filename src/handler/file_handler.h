#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "handler/handler.h"
#include "http/request.h"
#include "http/response.h"
#include "gtest/gtest_prod.h"
#include <iostream>
#include <fstream>
#include <string>


// Request handler for serving a static file
// File handler can create 200 responses if the file is found
// or 404 if the file requested is not found
class FileRequestHandler : public RequestHandler {
private:
	std::string root_dir_;

public:
	FileRequestHandler(const std::string& root_dir) : root_dir_(root_dir) {};
	virtual ~FileRequestHandler() {};

	////
	// No accessors or mutators needed. Get it right the first time
	//// 

	virtual Response handle(Request request);

	// Helper functions
	std::string getFileExtension(const std::string& fileName);
	std::string getContentTypeFromExtension(const std::string& extension);
};

#endif