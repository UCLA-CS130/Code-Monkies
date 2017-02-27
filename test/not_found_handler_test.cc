#include "gtest/gtest.h"
#include "handler/not_found_handler.h"

TEST(NotFoundHandlerTest, TestHandle) {
	Request request;
	Response response;

	auto handler = RequestHandler::CreateByName("NotFoundHandler");

	handler->HandleRequest(request, &response);

	EXPECT_EQ(
		response.status(),
		Response::ResponseCode::HTTP_404_NOT_FOUND
	);

	EXPECT_EQ(
		response.body(),
		"404: File not found\n"
	);
}
