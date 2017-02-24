#include "gtest/gtest.h"
#include "handler/echo_handler.h"
#include <algorithm>
#include <string>
#include <vector>

class EchoRequestHandlerTest : public ::testing::Test {
public:
	EchoRequestHandler handler_;
	const std::string method_ = "GET";
	const std::string uri_ = "/path/to/some_file";
	const Header header_ = std::make_pair("Content-Type", "text/plain");
	const std::string body_ = "I AM A BODY!";

	std::unique_ptr<Request> makeValidRequest() {
		std::string request_str = \
			"GET /path/to/some_file HTTP/1.1\r\n"
			"Content-Type: text/plain\r\n\r\n"
			"I AM A BODY!";

		auto r = Request::Parse(request_str);
		return r;
	}

};

TEST_F(EchoRequestHandlerTest, EchoRequest) {
	auto req = makeValidRequest();
 	Response *res;

	EXPECT_TRUE(handler_.handle(req, res));

	EXPECT_TRUE(res != NULL);

	EXPECT_EQ(
		res->status(), 
		Response::ResponseCode::HTTP_200_OK
	);

	EXPECT_EQ(
		req->headers()[0].first,
		"Content-Type"
	);

	EXPECT_EQ(
		req->headers()[0].second,
		"text/plain"
	);

	EXPECT_EQ(res->body(), req->raw_request());
}

