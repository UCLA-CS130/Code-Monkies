#include "gtest/gtest.h"
#include "server.h"
#include <string>

using std::string;

class ServerTest : public ::testing::Test {
public:
	void init(int port) {
		boost::asio::io_service io_service;
		Server s(io_service, port);
	    io_service.run();
	};
};

TEST_F(ServerTest, NegativePortShouldExitTest) {
	ASSERT_EXIT(init(-1), ::testing::ExitedWithCode(1),"Port -1 is out of range");
}

TEST_F(ServerTest, TooBigPortShouldExitTest) {
	ASSERT_EXIT(init(65536), ::testing::ExitedWithCode(1), "Port 65536 is out of range");
}

// Note: valid port testing in done through the instrumentation test