#include "config.h"
#include "helpers.h"

using helpers::debugf;

Config::Config(const short port, const std::unordered_set<std::string> &echo_uris,
    const std::unordered_map<std::string, std::string> &file_uri_mappings)
  : port(port), echo_uris(echo_uris), file_uri_mappings(file_uri_mappings)
{}

ConfigBuilder::ConfigBuilder()
{
  port = -1;
}

bool ConfigBuilder::setPort(int port)
{
  if (port < 1 || port > PORT_MAX) {
    debugf("ConfigBuilder::setPort", "Port out of range. Port: %d.\n", port);
    return false;
  }
  this->port = port;
  return true;
}

bool ConfigBuilder::addEchoUri(std::string uri)
{
  auto got = echo_uris.find(uri);
  if (got == echo_uris.end()) {
    // TODO error check this
    echo_uris.emplace(uri);
    return true;
  } else {
    debugf("ConfigBuilder::addEchoUri", "Echo URI already specified. URI: "
        "%s.\n", uri.c_str());
    return false;
  }
}

bool ConfigBuilder::addFileUriMapping(std::string uri, std::string path)
{
  auto got = file_uri_mappings.find(uri);
  if (got == file_uri_mappings.end()) {
    // TODO error check this
    file_uri_mappings.emplace(uri, path);
    return true;
  } else {
    debugf("ConfigBuilder::addFileUriMapping", "File URI already specified. "
        "URI: %s.\n", uri.c_str());
    return false;
  }
}

bool ConfigBuilder::build(Config *&config)
{
  if (port == -1) {
    debugf("ConfigBuilder::build", "Port never set.\n");
    return false;
  }

  config = new (std::nothrow) Config((short) port, echo_uris,
      file_uri_mappings);
  if (config == NULL) {
    debugf("ConfigBuilder::build", "Failed to allocate config.\n");
    return false;
  }

  return true;
}
