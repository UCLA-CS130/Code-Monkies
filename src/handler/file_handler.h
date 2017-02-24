#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "handler/handler.h"
#include <iostream>
#include <fstream>
#include <string>

// Request handler for serving a static file
// File handler can create 200 responses if the file is found
// or 404 if the file requested is not found
class FileRequestHandler : public RequestHandler {
private:
	const std::string file_path_;

public:
	FileRequestHandler(const std::string& file_path)
    : RequestHandler(), file_path_(file_path)
  {
  }
	virtual ~FileRequestHandler() {};

	////
	// No accessors or mutators needed. Get it right the first time
	//// 

	virtual bool handle(const std::unique_ptr<Request> &request, Response *&response);

	// Helper functions
	std::string getFileExtension(const std::string& fileName);
	Header getContentTypeFromExtension(const std::string& extension);
};

#endif
