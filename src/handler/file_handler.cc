#include "handler/file_handler.h"
#include "http/request.h"
#include "http/response.h"
#include "http/constants.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Why poorly reinvent the wheel? Shameless taken from
// http://stackoverflow.com/questions/51949/how-to-get-file-extension-from-string-in-c/51993#51993
// which is a nice STL solution
std::string FileRequestHandler::getFileExtension(const std::string& fileName)
{
    if(fileName.find_last_of(".") != std::string::npos)
        return fileName.substr(fileName.find_last_of(".")+1);
    return "";
}

std::string FileRequestHandler::getContentTypeFromExtension(const std::string& extension) {
	// TODO: Make a singleton map of FileExtension -> ContentType;
	static const std::string jpg =  "jpg", 
							 png =  "png",
							 html = "html";
	static const std::string image_jpeg = "Content-Type: image/jpeg",
							 image_png =  "Content-Type: image/png",
		 					 text_html =  "Content-Type: text/html",
		 					 text_plain = "Content-Type: text/plain";

	// Images should be "Content-Type: application/image"						 
	if (extension.compare(jpg) == 0) {
		return image_jpeg;
	} else if (extension.compare(png) == 0) {
		return image_png;
	} else if (extension.compare(html) == 0) {
		return text_html;
	} else {  // Default, just assume it is plaintext	
		return text_plain;
	}
}

Response FileRequestHandler::handle(Request request) {
	// Try to get the file specified by the handler
	std::string full_path = this->root_dir_ + request.getUri();
	std::ifstream file;
	file.open(full_path, std::fstream::in);

	// Check if file exists/is available
	// TODO: Check permissions and throw 403 when appropriate
	if (!file.is_open()) {
		std::cerr << "Could not open file " 
				  << full_path 
				  << std::endl;
		Response response(status::HTTP_404_NOT_FOUND);
		return response;
	}

	// Get the file contents as a string and return it
	// in a 200 Response body
	std::stringstream file_sstream;
	file_sstream << file.rdbuf();	
	file.close();

	Response response(status::HTTP_200_OK);
	std::string extension = getFileExtension(request.getUri());
	std::string contentType = getContentTypeFromExtension(extension);
	response.addHeader(contentType);

	std::string body = file_sstream.str();
	response.setBody(body);

	return response;
}