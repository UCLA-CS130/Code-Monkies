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

TEST_F(ResponseTest, buildResponseNoHeadersNoBody) {
	// We expect a carriage return to end the 
	// empty headers and a carriage return to separate 
	// the empty body
	std::string expectedResponse = "HTTP/1.1 200 OK\r\n\r\n";

	EXPECT_EQ(
		response_->ToString(),
		expectedResponse
	);
}

TEST_F(ResponseTest, buildResponseSomeHeadersNoBody) {
	std::string expectedResponse = \
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: max-age=60\r\n\r\n";

	response_->AddHeader("Content-Type", "application/json");
	response_->AddHeader("Cache-Control", "max-age=60");

	EXPECT_EQ(
		response_->ToString(),
		expectedResponse
	);
}

TEST_F(ResponseTest, buildFullResponse) {
	std::string expectedResponse = \
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: max-age=60\r\n\r\n"
		"{\"key\": 5}";

	response_->AddHeader("Content-Type", "application/json");
	response_->AddHeader("Cache-Control", "max-age=60");
	response_->SetBody("{\"key\": 5}");

	EXPECT_EQ(
		response_->ToString(),
		expectedResponse
	);
}

TEST_F(ResponseTest, parseFromRawResponse) {
  std::string rawResponse = \
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: max-age=60\r\n\r\n"
		"{\"key\": 5}";
  response_->Parse(rawResponse);
  EXPECT_EQ(rawResponse, response_->ToString());
  EXPECT_EQ("HTTP/1.1", response_->version());
  EXPECT_EQ("200 OK", response_->status_message());
  EXPECT_EQ("{\"key\": 5}", response_->body());
}
