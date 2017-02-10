#include "config.h"
#include "helpers.h"

#include <sstream>

using helpers::debugf;

std::string Config::toString() const
{
  std::ostringstream out;
  out << "port: " << port_ << std::endl;
  out << "echo uris:" << std::endl;
  for (auto const& uri : echo_uris_) {
    out << "  " << uri << std::endl;
  }
  out << "file uri mappings:" << std::endl;
  for (auto const& file_uri_mapping : file_uri_mappings_) {
    out << "  " << file_uri_mapping.first;
    out << " -> " << file_uri_mapping.second << std::endl;
  }
  return out.str();
}

ConfigBuilder::ConfigBuilder()
{
  port_ = -1;
}

bool ConfigBuilder::build(const NginxConfig &config, Config *&conf)
{
  if (config.statements_.size() != 1) {
    debugf("ConfigBuilder::build", "Incorrect number of top-level statements "
        "in server configuration file. Got %lu, expected 1.\n",
        config.statements_.size());
    return false;
  }

  if (config.statements_[0]->child_block_ == NULL) {
    debugf("ConfigBuilder::build", "Expected child block for top-level "
        "statement.\n");
    return false;
  }

  for (auto const& statement :
      config.statements_[0]->child_block_->statements_) {
    size_t len = statement->tokens_.size();
    if (len == 0) {
      debugf("ConfigBuilder::build", "Invalid configuration file.\n");
      return false;
    } else if (len == 1) {
      /*
       * Must be a serve block or an echo block.
       */
      if (statement->tokens_[0] == "serve") {
        for (auto const& tuple : statement->child_block_->statements_) {
          if (tuple->tokens_.size() != 2) {
            debugf("ConfigBuilder::build", "Invalid serve tuple: has length "
                "%lu, should have length 2.\n", tuple->tokens_.size());
            return false;
          }
          if (!addFileUriMapping(tuple->tokens_[0],
                tuple->tokens_[1])) {
            debugf("ConfigBuilder::build", "Invalid serve tuple.\n");
            return false;
          }
        }
      } else if (statement->tokens_[0] == "echo") {
        for (auto const& uri : statement->child_block_->statements_) {
          if (uri->tokens_.size() != 1) {
            debugf("ConfigBuilder::build", "Invalid echo URI: received %lu "
                "tokens, should have received 1.\n", uri->tokens_.size());
            return false;
          }
          if (!addEchoUri(uri->tokens_[0])) {
            debugf("ConfigBuilder::build", "Invalid echo URI.\n");
            return false;
          }
        }
      } else {
        debugf("ConfigBuilder::build", "Unexpected inner block: %s.\n",
            statement->tokens_[0].c_str());
        return false;
      }
    } else if (len == 2) {
      /*
       * Must be a port number
       */
      if (statement->tokens_[0] == "listen") {
        if (!setPort(std::stoi(statement->tokens_[1]))) {
          debugf("ConfigBuilder::build", "Received invalid port.\n");
          return false;
        }
      } else {
        debugf("ConfigBuilder::build", "Unexpected inner statement: %s.\n",
            statement->ToString(0).c_str());
        return false;
      }
    } else {
      debugf("ConfigBuilder::build", "Unexpected inner statement: %s.\n",
          statement->ToString(0).c_str());
      return false;
    }
  }

  if (!build(conf)) {
    debugf("ConfigBuilder::build", "Failed to build valid server "
        "configuration.\n");
    return false;
  }

  return true;
}

bool ConfigBuilder::setPort(int port)
{
  if (port < 1 || port > PORT_MAX) {
    debugf("ConfigBuilder::setPort", "Port out of range. Port: %d.\n", port);
    return false;
  }
  if (port_ != -1) {
    debugf("ConfigBuilder::setPort", "Cannot set multiple ports. Old port: "
        "%d, new port: %d.\n", port_, port);
    return false;
  }
  port_ = port;
  return true;
}

bool ConfigBuilder::addEchoUri(std::string uri)
{
  auto have_echo_uri = echo_uris_.find(uri);
  auto have_serve_uri = file_uri_mappings_.find(uri);
  if (have_echo_uri == echo_uris_.end() &&
      have_serve_uri == file_uri_mappings_.end()) {
    // TODO error check this
    echo_uris_.emplace(uri);
    return true;
  } else {
    debugf("ConfigBuilder::addEchoUri", "Echo URI already specified. URI: "
        "%s.\n", uri.c_str());
    return false;
  }
}

bool ConfigBuilder::addFileUriMapping(std::string uri, std::string path)
{
  auto have_echo_uri = echo_uris_.find(uri);
  auto have_serve_uri = file_uri_mappings_.find(uri);
  if (have_echo_uri == echo_uris_.end() &&
      have_serve_uri == file_uri_mappings_.end()) {
    // TODO error check this
    file_uri_mappings_.emplace(uri, path);
    return true;
  } else {
    debugf("ConfigBuilder::addFileUriMapping", "File URI already specified. "
        "URI: %s.\n", uri.c_str());
    return false;
  }
}

bool ConfigBuilder::build(Config *&config)
{
  if (port_ == -1) {
    debugf("ConfigBuilder::build", "Port never set.\n");
    return false;
  }

  config = new (std::nothrow) Config((short) port_, echo_uris_,
      file_uri_mappings_);
  if (config == NULL) {
    debugf("ConfigBuilder::build", "Failed to allocate config.\n");
    return false;
  }

  return true;
}
