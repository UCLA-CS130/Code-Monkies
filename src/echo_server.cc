#include <cstdlib>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "config_parser.h"
#include "helpers.h"

#define USAGE "USAGE: %s config_file_path.\n"

void serve(int port);

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, USAGE, argv[0]);
    exit(1);
  }

  debugf("conf file: %s\n", argv[1]);

  NginxConfigParser parser;
  NginxConfig config;

  if (!parser.Parse(argv[1], &config)) {
    fprintf(stderr, "Failed to parse configuration file.\n");
    exit(1);
  }

  /*
   * TODO: rewrite this in a more resilient/extensible way.
   */
  int port = std::stoi(config.statements_[0]->tokens_[1]);

  debugf("server port: %d\n", port);

  serve(port);
  return 0;
}

void serve(int port)
{
  (void) port;
}
