#include <cstdlib>
#include <boost/asio.hpp>

#include "config_parser.h"
#include "server.h"

#define USAGE "USAGE: %s config_file_path.\n"
#define PORT_MAX 65535

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
   * Right now we assume that the config file looks like "somestring 80;".
   * This will fail gruesomely if anything is trivially different.
   */
  int port = std::stoi(config.statements_[0]->tokens_[1]);
  debugf("server port: %d\n", port);

  if (port < 1 || port > PORT_MAX) {
    fprintf(stderr, "Port %d is out of range\n", port);
    exit(1);
  }

  try {
    boost::asio::io_service io_service;
    Server s(io_service, port);
    io_service.run();
  } catch (std::exception &e) {
    fprintf(stderr, "io_service encountered exception: %s\n", e.what());
    exit(1);
  }

  return 0;
}
