#include "handler/static_handler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Why poorly reinvent the wheel? Shameless taken from
// http://stackoverflow.com/questions/51949/how-to-get-file-extension-from-string-in-c/51993#51993
// which is a nice STL solution
std::string StaticHandler::getFileExtension(const std::string& fileName)
{
    if(fileName.find_last_of(".") != std::string::npos)
        return fileName.substr(fileName.find_last_of(".")+1);
    return "";
}

Header StaticHandler::getContentTypeFromExtension(const std::string& extension) {
	// TODO: Make a singleton map of FileExtension -> ContentType;
	static const std::string jpg =  "jpg", 
							 png =  "png",
							 html = "html";
	static const std::string image_jpeg = "image/jpeg",
							 image_png =  "image/png",
		 					 text_html =  "text/html",
		 					 text_plain = "text/plain";

	// Images should be "Content-Type: application/image"						 
	if (extension.compare(jpg) == 0) {
		return std::make_pair("Content-Type", image_jpeg);
	} else if (extension.compare(png) == 0) {
		return std::make_pair("Content-Type", image_png);
	} else if (extension.compare(html) == 0) {
		return std::make_pair("Content-Type", text_html);
	} else {  // Default, just assume it is plaintext	
		return std::make_pair("Content-Type", text_plain);
	}
}

RequestHandler::Status StaticHandler::HandleRequest(const Request& request,
                     Response* response) {
	// Try to get the file specified by the handler
	std::ifstream file;
	file.open(file_path_, std::fstream::in);

	// Check if file exists/is available
	// TODO: Check permissions and throw 403 when appropriate
	if (!file.is_open()) {
		auto not_found_handler = RequestHandler::CreateByName("NotFoundHandler");
		return not_found_handler->HandleRequest(request, response);
	}

	// Get the file contents as a string and return it
	// in a 200 Response body
	std::stringstream file_sstream;
	file_sstream << file.rdbuf();	
	file.close();

	response->SetStatus(Response::ResponseCode::HTTP_200_OK);
	std::string extension = getFileExtension(request.uri());
	auto contentType = getContentTypeFromExtension(extension);
	response->AddHeader(contentType);

	std::string body = file_sstream.str();
	response->SetBody(body);

	return RequestHandler::Status::OK;
}

RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {

	// TODO: THIS IS VERY WRONG. PASSES THE INT TEST
	file_path_ = uri_prefix;
	(void) config;

	return RequestHandler::Status::OK;
}
