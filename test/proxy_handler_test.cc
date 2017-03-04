#include "gtest/gtest.h"
#include "handler/proxy_handler.h"
#include <string>
#include <sstream>
#include "../../lib/nginx-configparser/config_parser.h"

class ProxyHandlerTest : public ::testing::Test {
public:
  ProxyHandler* handler_;
  NginxConfigParser parser_;
  NginxConfig config_;

  bool CreateProxyHandlerTest() {
    std::string config_string = "port 2020;"; // just a simple valid config so we can call Init
    std::stringstream config_stream(config_string);

    if (!parser_.Parse(&config_stream, &config_)) {
      return false;
    }

    handler_ = new ProxyHandler();
    handler_->Init("/proxy", config_);

    return true;
  }

  std::unique_ptr<Request> GetClientRequest() {
    std::string request_str = \
      "GET /proxy/path/to/some_file HTTP/1.1\r\n"
      "Content-Type: text/plain\r\n\r\n"
      "I AM A BODY!";
    auto r = Request::Parse(request_str);
    return r;
  }

  std::unique_ptr<Response> GetResponse() {
    std::string response_str = \
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n\r\n"
      "<html>\n\n<head>\n<title>A very simple <href=\"link.html\">webpage</href></title>\n</body>\n\n</html>\n";
    auto r = Response::Parse(response_str);
    return r;
  }
};

TEST_F(ProxyHandlerTest, ProxyHandlerInitTest) {
  EXPECT_TRUE(CreateProxyHandlerTest());
}

TEST_F(ProxyHandlerTest, RequestTransformationTest) {
  EXPECT_TRUE(CreateProxyHandlerTest());
  auto req = GetClientRequest();
  auto proxy_req = handler_->CreateProxyRequestFromClientRequest(*req, "localhost:8080");
  EXPECT_EQ("/path/to/some_file", proxy_req->uri());
}

TEST_F(ProxyHandlerTest, ResponseTransformationTest) {
  EXPECT_TRUE(CreateProxyHandlerTest());
  auto resp = GetResponse();
  handler_->RewriteUrls(*resp);
  std::string correct_response_str = \
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n\r\n"
    "<html>\n\n<head>\n<title>A very simple <href=\"/proxy/link.html\">webpage</href></title>\n</body>\n\n</html>\n";
  EXPECT_EQ(correct_response_str, resp->ToString());
}
