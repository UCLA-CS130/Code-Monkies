#include "gtest/gtest.h"
#include "api/response.h"
#include <string>

class ResponseTest : public ::testing::Test {
public: 
	Response* response_;

	void SetUp() {
		response_ = new Response();
		response_->SetStatus(Response::ResponseCode::HTTP_200_OK);
	}

	void TearDown() {
		delete response_;
	}
};

TEST_F(ResponseTest, buildRequestNoHeadersNoBody) {
	// We expect a carriage return to end the 
	// empty headers and a carriage return to separate 
	// the empty body
	std::string expectedRequest = "HTTP/1.1 200 OK\r\n\r\n";

	EXPECT_EQ(
		response_->ToString(),
		expectedRequest
	);
}

TEST_F(ResponseTest, buildRequestSomeHeadersNoBody) {
	std::string expectedRequest = \
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: max-age=60\r\n\r\n";

	response_->AddHeader("Content-Type", "application/json");
	response_->AddHeader("Cache-Control", "max-age=60");

	EXPECT_EQ(
		response_->ToString(),
		expectedRequest
	);
}

TEST_F(ResponseTest, buildFullRequest) {
	std::string expectedRequest = \
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: max-age=60\r\n\r\n"
		"{\"key\": 5}";

	response_->AddHeader("Content-Type", "application/json");
	response_->AddHeader("Cache-Control", "max-age=60");
	response_->SetBody("{\"key\": 5}");

	EXPECT_EQ(
		response_->ToString(),
		expectedRequest
	);
}