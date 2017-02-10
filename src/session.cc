// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "session.h"
#include <math.h>

using helpers::debugf;

void Session::do_read()
{
  auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(data_, MAX_LENGTH),
      [this, self](boost::system::error_code ec, std::size_t length)
      {
        if (!ec) {
          debugf("Session::do_read", "Got message of length %lu.\n",
            length);
          process_response();
        }
      });
}

void Session::process_response()
{
  Request req;
  req.consume(data_);
  Response *res;
  std::string uri = req.getUri();
  boost::system::error_code ec;

  // determine whether we have an echo handler or a response handler

  RequestHandler *handler = NULL;

  std::pair<const std::string, std::string> const *longestPrefixMapping = NULL;

  for (auto &echoUri : conf_->echo_uris_) {
    if (uri == echoUri) { // guaranteed to be true at most once
      handler = new (std::nothrow) EchoRequestHandler();
      if (handler == NULL) {
        goto err;
      }
    }
  }

  /*
   * We may have multiple choices on where to serve files from when looking
   * over fileUriMappings. Consider this case:
   *  serve {
   *    /static       /var/www/html;
   *    /static/foo   /opt/html;
   *  }
   * If we receive a request asking for /static/foo/index.html, we could match
   * against the /static URI root and look for the file 'foo/index.html' under
   * /var/www/html. We could also match against /static/foo and look for the
   * file 'index.html' under /opt/html. To disambiguate this situation, I've
   * made a policy decision to match the longest URI root - e.g., in this case,
   * we would choose /static/foo as the URI root instead of just /static.
   */

  for (auto &fileUriMapping : conf_->file_uri_mappings_) {
    if (uri.find(fileUriMapping.first) == 0) {
      // Match against the mapping with the longest URI root.
      if (longestPrefixMapping == NULL ||
          (fileUriMapping.first.length() >
           longestPrefixMapping->first.length())) {
        longestPrefixMapping = &fileUriMapping;
      }
    }
  }

  if (longestPrefixMapping != NULL) {
    std::string uri_root = longestPrefixMapping->first;
    std::string root_dir = longestPrefixMapping->second;
    std::string uri_remainder = uri.substr(uri_root.length(), uri.length());
    std::string file_path = root_dir + uri_remainder;
    debugf("Session::process_response", "uri_root: %s\n", uri_root.c_str());
    debugf("Session::process_response", "root_dir: %s\n", root_dir.c_str());
    debugf("Session::process_response", "uri_remainder: %s\n",
        uri_remainder.c_str());
    debugf("Session::process_response", "file_path: %s\n", file_path.c_str());
    handler = new (std::nothrow) FileRequestHandler(file_path);
    if (handler == NULL) {
      goto err;
    }
  }

  // No URI root matches against the given URI. Return 404.
  if (handler == NULL) {
    handler = new (std::nothrow) NotFoundHandler();
    if (handler == NULL) {
      goto err;
    }
  }

  // TODO: I think this writing pattern could potentially fall out of
  // sequence. Not sure if that's an issue.
  while (!handler->doneHandling()) {
    if (!handler->handle(req, res)) {
      debugf("Session::process_response", "Failed to handle request.\n");
      break;
    }
    do_write(res);
  }

  if (handler != NULL) {
    delete handler;
  }

  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  socket_.close();

  return;
err:
  if (handler == NULL) {
    debugf("Session::process_response", "Failed to allocate handler when "
        "processing request. Cannot serve client.\n");
  }
}

void Session::do_write(const Response *res)
{
  auto self(shared_from_this());

  const std::string res_str = res->build();
  const char *res_cstr = res_str.c_str();
  size_t len = res_str.length();

  debugf("Session::do_write", "do_write sending response of length %d\n",
      len);

  boost::asio::async_write(socket_, boost::asio::buffer(res_cstr,
        len),
      [this, self](boost::system::error_code ec, std::size_t length)
      {
        if (!ec) {
          debugf("Session::do_write", "Wrote buffer of length %lu to "
            "client.\n", length);
          return;
        } else {
          debugf("Session::do_write", "Failed to write buffer to client. Got "
            "error code: %d: %s.\n", ec.value(), ec.message().c_str());
        }
      });
}
