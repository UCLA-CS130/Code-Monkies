#include <cstdlib>
#include <boost/asio.hpp>

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

  debugf("main", "Configuration is syntactically valid.\n");

  if (!Config::Validate(&config)) {
    debugf("main", "Configuration is semantically invalid.\n");
    exit(1);
  }

  debugf("main", "Configuration is semantically valid.\n");

  try {
    boost::asio::io_service io_service;
    Server s(io_service, &config);
    io_service.run();
  } catch (std::exception &e) {
    fprintf(stderr, "io_service encountered exception: %s\n", e.what());
    exit(1);
  }

  return 0;
}
