#include "handler/not_found_handler.h"
#include "http/request.h"
#include "http/response.h"
#include "http/constants.h"
#include <string>

// Convert Request to string and dump it in the body
bool NotFoundHandler::handle(const Request&, Response *&response) {
  started_handling_ = true;
  // TODO null check allocation
	response = new (std::nothrow) Response(status::HTTP_404_NOT_FOUND);

  done_handling_ = true;
	return true;
}
