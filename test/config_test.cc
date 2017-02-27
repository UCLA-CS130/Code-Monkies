#include <sstream>
#include <string.h>
#include <strings.h>

#include "gtest/gtest.h"
#include "config.h"
#include "helpers.h"

#define PORT "port %d;\n%s"
#define HANDLER_BLOCK "path %s %s {\n%s\n}\n%s"
#define DEFAULT_BLOCK "default %s {\n%s\n}\n%s"

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

    void addInitialBlock() {
      sprintf(config_str, "%s", "%s");
    }

    void addPort(int port) {
      sprintf(config_str, bufcpy(config_str), PORT);
      sprintf(config_str, bufcpy(config_str), port, "%s");
    }

    void addHandlerBlock(const char *uri_prefix, const char *handler_name) {
      sprintf(config_str, bufcpy(config_str), HANDLER_BLOCK);
      sprintf(config_str, bufcpy(config_str), uri_prefix, handler_name, "%s",
          "%s");
    }

    void addDefaultBlock(const char *handler_name) {
      sprintf(config_str, bufcpy(config_str), DEFAULT_BLOCK);
      sprintf(config_str, bufcpy(config_str), handler_name, "%s", "%s");
    }

    /*
     * After adding any block, our format string has two %s tokens in it - one
     * inside the block, and one trailing the block. str must not contain any
     * format specifiers, should not contain any newlines, and should not have
     * a semicolon at the end.
     * This method can be called multiple times, and must be followed by a
     * call to finalizeBlock.
     */
    void addBlockEntry(const char *str) {
      sprintf(config_str, bufcpy(config_str), fuckstrcat(str, ";\n%s"), "%s");
    }

    void finalizeBlock() {
      sprintf(config_str, bufcpy(config_str), "", "%s");
    }

    void finalizeConfig() {
      sprintf(config_str, bufcpy(config_str), "");
    }

    bool configIsValid() {
      std::istringstream config_stream(config_str);
      config = new NginxConfig();
      if (!parser.Parse(&config_stream, config)) {
        ADD_FAILURE() << "Failed to parse configuration:\n.";
      }
      printf("a\n");
      bool ret = Config::Validate(config);
      printf("b\n");
      printf("ret: %d\n", ret);
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
  addInitialBlock();
  addHandlerBlock("/echo", "EchoHandler");
  finalizeBlock();
  finalizeConfig();
  ASSERT_FALSE(configIsValid());
}

TEST_F(ConfigBuilderTest, MultiplePortsShouldNotParse) {
  addInitialBlock();
  addPort(80);
  addPort(8080);
  finalizeConfig();
  ASSERT_FALSE(configIsValid());
}

TEST_F(ConfigBuilderTest, PortOnlyShouldParse) {
  addInitialBlock();
  addPort(80);
  finalizeConfig();
  ASSERT_TRUE(configIsValid());
}

TEST_F(ConfigBuilderTest, BadPortShouldFail) {
  // reserved port
  addInitialBlock();
  addPort(0);
  finalizeConfig();
  ASSERT_FALSE(configIsValid());
  // out of range port
  addInitialBlock();
  addPort(-1);
  finalizeConfig();
  ASSERT_FALSE(configIsValid());
  // out of range port
  addInitialBlock();
  addPort(65536);
  finalizeConfig();
  ASSERT_FALSE(configIsValid());
}

TEST_F(ConfigBuilderTest, PortLastShouldParse) {
  addInitialBlock();
  addHandlerBlock("/echo", "EchoHandler");
  finalizeBlock();
  addHandlerBlock("/static", "StaticHandler");
  addBlockEntry("root /foo");
  finalizeBlock();
  addHandlerBlock("/status", "StatusHandler");
  finalizeBlock();
  addPort(80);
  finalizeConfig();
  ASSERT_TRUE(configIsValid());
}

TEST_F(ConfigBuilderTest, EchoBlockTest) {
  // empty body should parse
  addInitialBlock();
  addPort(80);
  addHandlerBlock("/echo", "EchoHandler");
  finalizeBlock();
  finalizeConfig();
  ASSERT_TRUE(configIsValid());
  // non-empty body should not parse
  // TODO
  /*
  addInitialBlock();
  addPort(80);
  addHandlerBlock("/echo", "EchoHandler");
  addBlockEntry("root /foo");
  finalizeBlock();
  finalizeConfig();
  ASSERT_FALSE(configIsValid());
  */
}

TEST_F(ConfigBuilderTest, StaticBlockTest) {
  // empty body should not parse
  addInitialBlock();
  addPort(80);
  addHandlerBlock("/static", "StaticHandler");
  finalizeBlock();
  finalizeConfig();
  ASSERT_FALSE(configIsValid());
  // valid body should parse
  addInitialBlock();
  addPort(80);
  addHandlerBlock("/static", "StaticHandler");
  addBlockEntry("root /foo");
  finalizeBlock();
  finalizeConfig();
  ASSERT_TRUE(configIsValid());
  // first word must be "root"
  addInitialBlock();
  addPort(80);
  addHandlerBlock("/static", "StaticHandler");
  addBlockEntry("foo /bar");
  finalizeBlock();
  finalizeConfig();
  printf("%s\n", config_str);
  ASSERT_FALSE(configIsValid());
  // must only have 2 words
  addInitialBlock();
  addPort(80);
  addHandlerBlock("/static", "StaticHandler");
  addBlockEntry("foo /bar /baz");
  finalizeBlock();
  finalizeConfig();
  ASSERT_FALSE(configIsValid());
  // must only have 2 words
  addInitialBlock();
  addPort(80);
  addHandlerBlock("/static", "StaticHandler");
  addBlockEntry("foo");
  finalizeBlock();
  finalizeConfig();
  ASSERT_FALSE(configIsValid());
}

TEST_F(ConfigBuilderTest, DuplicateURIsShouldFail) {
}
