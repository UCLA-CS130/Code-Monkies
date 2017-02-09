#include "gtest/gtest.h"
#include "handler/echo_handler.h"
#include "http/request.h"
#include "http/response.h"
#include "http/constants.h"
#include <algorithm>
#include <string>
#include <vector>

class EchoRequestHandlerTest : public ::testing::Test {
public:
	EchoRequestHandler handler_;
	const std::string method_ = "GET";
	const std::string uri_ = "/echo/me/pls";
	const std::string header_ = "Content-Type: test/plain";
	const std::string body_ = "I AM A BODY!";

	Request makeValidRequest() {
		Request r(method_, uri_);
		r.addHeader(header_);
		r.setBody(body_);

		return r;
	}

};

TEST_F(EchoRequestHandlerTest, EchoRequest) {
	Request req = makeValidRequest();
	Response res = handler_.handle(req);

	EXPECT_EQ(res.getStatus(), status::HTTP_200_OK);

	std::vector<std::string> resHeaders = res.getHeaders();

	EXPECT_TRUE(
		// This makes me miss `TEXT_PLAIN in resHeaders` in python :(
		// Makes sure that Content-Type: text/plain is in the headers
		std::find(resHeaders.begin(),
			 	  resHeaders.end(),
			 	  handler_.TEXT_PLAIN
		) != resHeaders.end()
	);
}

