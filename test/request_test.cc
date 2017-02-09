#include "gtest/gtest.h"
#include "http/request.h"
#include "http/constants.h"
#include <string>
#include <sstream>
#include <vector>

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

TEST (RequestConsumeTest, ConsumeValidRequest) {
	std::string firstLine = "GET /path/to/file HTTP/1.1";
	std::vector<std::string> headers;
	headers.push_back("Content-Type: text/plain");
	headers.push_back("Authorization: Bearer abcdefg");
	std::string body = "{\"key\": 5}";

	std::stringstream validRequestStream;
	validRequestStream <<
		firstLine << http::CRLF 
		<< headers[0] << http::CRLF 
		<< headers[1] << http::CRLF << http::CRLF 
		<< body;
	std::string validRequestString = validRequestStream.str();

	Request r;
	r.consume(validRequestString);

	std::cout << r.build() << std::endl;

	EXPECT_EQ(r.buildFirstLine(), firstLine);
	EXPECT_EQ(r.getHeaders()[0], headers[0]);
	EXPECT_EQ(r.getHeaders()[1], headers[1]);
	EXPECT_EQ(r.getBody(), body);
}