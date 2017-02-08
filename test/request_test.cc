#include "gtest/gtest.h"
#include "http/request.h"
#include <string>

class RequestTest : public ::testing::Test {
public: 
	Request* request_;

	void SetUp() {
		request_ = new Request("GET", "/path/to/some_file");
	}

	void TearDown() {
		delete request_;
	}
};

TEST_F(RequestTest, buildFirstLine) {
	std::string expectedFirstLine = "GET /path/to/some_file HTTP/1.1";

	EXPECT_EQ(
		request_->buildFirstLine(),
		expectedFirstLine
	);
}

TEST_F(RequestTest, buildRequestNoHeadersNoBody) {
	// We expect a carriage return to end the 
	// empty headers and a carriage return to separate 
	// the empty body
	std::string expectedRequest = "GET /path/to/some_file HTTP/1.1\r\n\r\n";

	EXPECT_EQ(
		request_->build(),
		expectedRequest
	);
}

TEST_F(RequestTest, buildRequestSomeHeadersNoBody) {
	std::string expectedRequest = \
		"GET /path/to/some_file HTTP/1.1\r\n"
		"Content-Type: application/json\r\n"
		"Authorization: Bearer abcdefg\r\n\r\n";

	request_->addHeader("Content-Type: application/json");
	request_->addHeader("Authorization: Bearer abcdefg");

	EXPECT_EQ(
		request_->build(),
		expectedRequest
	);
}

TEST_F(RequestTest, buildFullRequest) {
	std::string expectedRequest = \
		"GET /path/to/some_file HTTP/1.1\r\n"
		"Content-Type: application/json\r\n"
		"Authorization: Bearer abcdefg\r\n\r\n"
		"{\"key\": 5}";

	request_->addHeader("Content-Type: application/json");
	request_->addHeader("Authorization: Bearer abcdefg");
	request_->setBody("{\"key\": 5}");

	EXPECT_EQ(
		request_->build(),
		expectedRequest
	);
}