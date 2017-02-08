#include <cstdlib>
#include <boost/asio.hpp>

#include "config.h"
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

  Config *valid_config;

  if (!ConfigBuilder().build(config, valid_config)) {
    fprintf(stderr, "Failed to build valid server configuration.\n");
    exit(1);
  }

  try {
    boost::asio::io_service io_service;
    Server s(io_service, valid_config->port_);
    io_service.run();
  } catch (std::exception &e) {
    fprintf(stderr, "io_service encountered exception: %s\n", e.what());
    exit(1);
  }

  return 0;
}
