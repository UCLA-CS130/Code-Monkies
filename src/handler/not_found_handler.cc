#include "handler/not_found_handler.h"
#include <string>

RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {
    // Does not actually need any information
    // to be initialized. We void cast the 
    // arguments to prevent W-unused-variable 
    // and W-error from getting mad
    (void) uri_prefix;
    (void) config;

    return RequestHandler::Status::OK;
}

RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request,
                     Response* response) {
    // Don't need request for this
    (void) request;

    response->SetStatus(Response::ResponseCode::HTTP_404_NOT_FOUND);
    response->AddHeader("Content-Type", "text/plain");
    std::string body = "404: File not found";
    response->SetBody(body);

    return RequestHandler::Status::OK;
}