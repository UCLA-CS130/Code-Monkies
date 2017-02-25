#include "gtest/gtest.h"
#include "api/request.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

class RequestTest : public ::testing::Test {
public: 
	Request* request_;

	void SetUp() {
		request_ = new Request();
		request_->SetMethod("GET");
		request_->SetUri("/path/to/some_file");
		request_->SetVersion("HTTP/1.1");
	}

	void TearDown() {
		delete request_;
	}
};

TEST_F(RequestTest, buildRequestNoHeadersNoBody) {
	// We expect a carriage return to end the 
	// empty headers and a carriage return to separate 
	// the empty body
	std::string expectedRequest = "GET /path/to/some_file HTTP/1.1\r\n\r\n";

	EXPECT_EQ(
		request_->raw_request(),
		expectedRequest
	);
}

TEST_F(RequestTest, buildRequestSomeHeadersNoBody) {
	std::string expectedRequest = \
		"GET /path/to/some_file HTTP/1.1\r\n"
		"Content-Type: application/json\r\n"
		"Authorization: Bearer abcdefg\r\n\r\n";

	Headers headers;
	headers.push_back(std::make_pair("Content-Type", "application/json"));
	headers.push_back(std::make_pair("Authorization", "Bearer abcdefg"));

	request_->SetHeaders(headers);

	EXPECT_EQ(
		request_->raw_request(),
		expectedRequest
	);
}

TEST_F(RequestTest, buildFullRequest) {
	std::string expectedRequest = \
		"GET /path/to/some_file HTTP/1.1\r\n"
		"Content-Type: application/json\r\n"
		"Authorization: Bearer abcdefg\r\n\r\n"
		"{\"key\": 5}";

	Headers headers;
	headers.push_back(std::make_pair("Content-Type", "application/json"));
	headers.push_back(std::make_pair("Authorization", "Bearer abcdefg"));

	request_->SetHeaders(headers);
	request_->SetBody("{\"key\": 5}");

	EXPECT_EQ(
		request_->raw_request(),
		expectedRequest
	);
}

TEST(RequestParseTest, ParseValidRequest) {
	std::string vaild_request = \
		"GET /path/to/some_file HTTP/1.1\r\n"
		"Content-Type: application/json\r\n"
		"Authorization: Bearer abcdefg\r\n\r\n"
		"{\"key\": 5}";

	auto request = Request::Parse(vaild_request);

	EXPECT_EQ(
		request->method(),
		"GET"
	);
	EXPECT_EQ(
		request->uri(),
		"/path/to/some_file"
	);
	EXPECT_EQ(
		request->version(),
		"HTTP/1.1"
	);

	Headers headers = request->headers();
	auto content_type = headers[0];
	auto authorization = headers[1];

	EXPECT_EQ(
		content_type.first,
		"Content-Type"
	);
	EXPECT_EQ(
		content_type.second,
		"application/json"
	);
	EXPECT_EQ(
		authorization.first,
		"Authorization"
	);
	EXPECT_EQ(
		authorization.second,
		"Bearer abcdefg"
	);

	EXPECT_EQ(
		request->body(),
		"{\"key\": 5}"
	);

	EXPECT_EQ(
		request->raw_request(),
		vaild_request
	);
}