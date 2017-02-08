#include "gtest/gtest.h"
#include "http/response.h"
#include <string>

class Response200Test : public ::testing::Test {
public: 
	Response* response_;

	void SetUp() {
		response_ = new Response(200);
	}

	void TearDown() {
		delete response_;
	}
};

TEST(ResponseConstructorTest, ShouldFailOnInvalidNum) {
	ASSERT_EXIT(
		Response(-1),
		::testing::ExitedWithCode(1),
		"-1 is not a valid status code for this class"
	);
}

TEST(ResponseConstructorTest, StatusOkFor200) {
	Response r(200);
	EXPECT_EQ(
		r.getReason(),
		"OK"
	);
}

TEST(ResponseConstructorTest, StatusNotFoundFor404) {
	Response r(404);
	EXPECT_EQ(
		r.getReason(),
		"NOT FOUND"
	);
}

TEST_F(Response200Test, buildFirstLine) {
	std::string expectedFirstLine = "HTTP/1.1 200 OK";

	EXPECT_EQ(
		response_->buildFirstLine(),
		expectedFirstLine
	);
}

TEST_F(Response200Test, buildRequestNoHeadersNoBody) {
	// We expect a carriage return to end the 
	// empty headers and a carriage return to separate 
	// the empty body
	std::string expectedRequest = "HTTP/1.1 200 OK\r\n\r\n";

	EXPECT_EQ(
		response_->build(),
		expectedRequest
	);
}

TEST_F(Response200Test, buildRequestSomeHeadersNoBody) {
	std::string expectedRequest = \
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: max-age=60\r\n\r\n";

	response_->addHeader("Content-Type: application/json");
	response_->addHeader("Cache-Control: max-age=60");

	EXPECT_EQ(
		response_->build(),
		expectedRequest
	);
}

TEST_F(Response200Test, buildFullRequest) {
	std::string expectedRequest = \
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: max-age=60\r\n\r\n"
		"{\"key\": 5}";

	response_->addHeader("Content-Type: application/json");
	response_->addHeader("Cache-Control: max-age=60");
	response_->setBody("{\"key\": 5}");

	EXPECT_EQ(
		response_->build(),
		expectedRequest
	);
}