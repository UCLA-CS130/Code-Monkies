#include <cstdlib>
#include <boost/asio.hpp>

#include "config.h"
#include "config_parser.h"
#include "server.h"
#include "helpers.h"

#define USAGE "USAGE: %s config_file_path.\n"

using helpers::debugf;

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, USAGE, argv[0]);
    exit(1);
  }

  debugf("main", "conf file: %s\n", argv[1]);

  NginxConfigParser parser;
  NginxConfig config;

  if (!parser.Parse(argv[1], &config)) {
    fprintf(stderr, "Failed to parse configuration file.\n");
    exit(1);
  }

  ConfigBuilder cbuilder;

  if (config.statements_.size() != 1) {
    fprintf(stderr, "Incorrect number of top-level statements in server "
        "configuration file. Got %lu, expected 1.\n",
        config.statements_.size());
    exit(1);
  }

  if (config.statements_[0]->child_block_ == NULL) {
    fprintf(stderr, "Expected child block for top-level statement.\n");
    exit(1);
  }

  for (auto const& statement :
      config.statements_[0]->child_block_->statements_) {
    size_t len = statement->tokens_.size();
    if (len == 0) {
      fprintf(stderr, "Invalid configuration file.\n");
      exit(1);
    } else if (len == 1) {
      /*
       * Must be a serve block or an echo block.
       */
      if (statement->tokens_[0] == "serve") {
        for (auto const& tuple : statement->child_block_->statements_) {
          if (tuple->tokens_.size() != 2) {
            fprintf(stderr, "Invalid serve tuple: has length %lu, should have "
                "length 2.\n", tuple->tokens_.size());
            exit(1);
          }
          if (!cbuilder.addFileUriMapping(tuple->tokens_[0],
                tuple->tokens_[1])) {
            fprintf(stderr, "Invalid serve tuple.\n");
            exit(1);
          }
        }
      } else if (statement->tokens_[0] == "echo") {
        for (auto const& uri : statement->child_block_->statements_) {
          if (uri->tokens_.size() != 1) {
            fprintf(stderr, "Invalid echo URI: received %lu tokens, should "
                "have received 1.\n", uri->tokens_.size());
            exit(1);
          }
          if (!cbuilder.addEchoUri(uri->tokens_[0])) {
            fprintf(stderr, "Invalid serve tuple.\n");
            exit(1);
          }
        }
      } else {
        fprintf(stderr, "Unexpected inner block: %s.\n",
            statement->tokens_[0].c_str());
        exit(1);
      }
    } else if (len == 2) {
      /*
       * Must be a port number
       */
      if (statement->tokens_[0] == "listen") {
        if (!cbuilder.setPort(std::stoi(statement->tokens_[1]))) {
          fprintf(stderr, "Received invalid port.\n");
          exit(1);
        }
      } else {
        fprintf(stderr, "Unexpected inner statement: %s.\n",
            statement->ToString(0).c_str());
        exit(1);
      }
    } else {
      fprintf(stderr, "Unexpected inner statement: %s.\n",
          statement->ToString(0).c_str());
      exit(1);
    }
  }

  Config *conf;
  if (!cbuilder.build(conf)) {
    fprintf(stderr, "Failed to build valid server configuration.\n");
    exit(1);
  }

  try {
    boost::asio::io_service io_service;
    Server s(io_service, conf->port);
    io_service.run();
  } catch (std::exception &e) {
    fprintf(stderr, "io_service encountered exception: %s\n", e.what());
    exit(1);
  }

  return 0;
}
