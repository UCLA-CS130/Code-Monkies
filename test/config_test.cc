#include <sstream>
#include <string.h>
#include <strings.h>

#include "gtest/gtest.h"
#include "config.h"
#include "helpers.h"

#define SERVER_BLOCK "server {\n%s\n}"
#define PORT "listen %d;\n%s"
#define ECHO_BLOCK "echo {\n%s\n}\n%s"
#define SERVE_BLOCK "serve {\n%s\n}\n%s"

class ConfigBuilderTest : public ::testing::Test {
  public:
    NginxConfigParser parser;
    NginxConfig *config;
    Config *valid_config;
    char *config_str;

    char *getbuf(int size) {
      char *buf = (char*) malloc(size);
      if (!buf) {
        ADD_FAILURE() << "Failed to allocate buffer for test.\n";
      }
      bzero(buf, size);
      return buf;
    }

    char *bufcpy(const char *src) {
      char *dst = getbuf(strlen(src) + 1);
      bzero(dst, strlen(src) + 1);
      if (strcpy(dst, src) == NULL) {
        ADD_FAILURE() << "Failed to copy string for test.\n";
      }
      return dst;
    }

    char *fuckstrcat(const char *str1, const char *str2) {
      char *dst = (char*) malloc(strlen(str1) + strlen(str2));
      strcpy(dst, str1);
      strcpy(dst + strlen(str1), str2);
      return dst;
    }

    /*
     * Configuration generators.
     */

    void addServerBlock() {
      sprintf(config_str, SERVER_BLOCK, "%s");
    }

    void addPort(int port) {
      sprintf(config_str, bufcpy(config_str), PORT);
      sprintf(config_str, bufcpy(config_str), port, "%s");
    }

    void addServeBlock() {
      sprintf(config_str, bufcpy(config_str), SERVE_BLOCK);
    }

    void addEchoBlock() {
      sprintf(config_str, bufcpy(config_str), ECHO_BLOCK);
    }

    /*
     * At this point our format string has two %s tokens in it. str must not
     * contain any format specifiers, should not contain any newlines, and
     * should not have a semicolon at the end.
     * This method can be called multiple times, and must be followed by a
     * call to finalizeInternalBlock.
     */
    void addBlockEntry(const char *str) {
      sprintf(config_str, bufcpy(config_str), fuckstrcat(str, ";\n%s"), "%s");
    }

    void finalizeInternalBlock() {
      sprintf(config_str, bufcpy(config_str), "", "%s");
    }

    void finalizeConfig() {
      sprintf(config_str, bufcpy(config_str), "");
    }

    bool configBuilds() {
      std::istringstream config_stream(config_str);
      config = new NginxConfig();
      if (!parser.Parse(&config_stream, config)) {
        ADD_FAILURE() << "Failed to parse configuration.";
      }
      bool ret = ConfigBuilder().build(*config, valid_config);
      delete config;
      return ret;
    }
  protected:
    virtual void SetUp() {
      config_str = getbuf(65536);
    }

    virtual void TearDown() {
      free(config_str);
      config_str = NULL;
    }
};

TEST_F(ConfigBuilderTest, NoPortShouldNotParse) {
  addServerBlock();
  finalizeConfig();
  ASSERT_FALSE(configBuilds());
}

TEST_F(ConfigBuilderTest, MultiplePortsShouldNotParse) {
  addServerBlock();
  addPort(80);
  addPort(8080);
  finalizeConfig();
  ASSERT_FALSE(configBuilds());
}

TEST_F(ConfigBuilderTest, PortOnlyShouldParse) {
  addServerBlock();
  addPort(80);
  finalizeConfig();
  ASSERT_TRUE(configBuilds());
}

TEST_F(ConfigBuilderTest, BadPortShouldFail) {
  // reserved port
  addServerBlock();
  addPort(0);
  finalizeConfig();
  ASSERT_FALSE(configBuilds());
  // out of range port
  addServerBlock();
  addPort(-1);
  finalizeConfig();
  ASSERT_FALSE(configBuilds());
  // out of range port
  addServerBlock();
  addPort(65536);
  finalizeConfig();
  ASSERT_FALSE(configBuilds());
}

TEST_F(ConfigBuilderTest, EmptyInnerBlocksShouldParse) {
  addServerBlock();
  addPort(80);
  addEchoBlock();
  finalizeInternalBlock();
  addServeBlock();
  finalizeInternalBlock();
  finalizeConfig();
  ASSERT_TRUE(configBuilds());
}

TEST_F(ConfigBuilderTest, PortLastShouldParse) {
  addServerBlock();
  addEchoBlock();
  finalizeInternalBlock();
  addServeBlock();
  finalizeInternalBlock();
  addPort(80);
  finalizeConfig();
  ASSERT_TRUE(configBuilds());
}

TEST_F(ConfigBuilderTest, DuplicateInnerBlocksShouldParse) {
  addServerBlock();
  addPort(80);
  for (int i = 0; i < 10; i++) {
    addEchoBlock();
    finalizeInternalBlock();
    addServeBlock();
    finalizeInternalBlock();
  }
  finalizeConfig();
  ASSERT_TRUE(configBuilds());
}

TEST_F(ConfigBuilderTest, DuplicateURIsShouldFail) {
  addServerBlock();
  addPort(80);
  addEchoBlock();
  addBlockEntry("/echo1");
  addBlockEntry("/echo1");
  finalizeInternalBlock();
  finalizeConfig();
  ASSERT_FALSE(configBuilds());

  addServerBlock();
  addPort(80);
  addServeBlock();
  addBlockEntry("/static1 /var/www/html");
  addBlockEntry("/static1 /opt/html");
  finalizeInternalBlock();
  finalizeConfig();
  ASSERT_FALSE(configBuilds());

  addServerBlock();
  addPort(80);
  addEchoBlock();
  addBlockEntry("/foo");
  finalizeInternalBlock();
  addServeBlock();
  addBlockEntry("/foo /var/www/html");
  finalizeInternalBlock();
  finalizeConfig();
  ASSERT_FALSE(configBuilds());
}
