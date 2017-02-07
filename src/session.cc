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
          debugf("Session::do_read", "do_read got message of length %lu\n",
            length);
          process_response(length);
        }
      });
}

void Session::process_response(std::size_t length)
{
  std::size_t capped_length = (length > MAX_LENGTH) ? MAX_LENGTH : length;
  int capped_length_digits = floor(log10(capped_length)) + 1;
  int response_overhead = strlen(RESPONSE) + capped_length_digits +
    strlen("text/plain");

  char *response = (char*) malloc(MAX_LENGTH);
  if (response == NULL) {
    debugf("Session::process_response", "do_write failed to allocate response "
        "buffer.\n");
    return;
  }

  // Apparently boost doesn't care to null terminate its strings, so we
  // have to do it ourselves.
  if (length < (MAX_LENGTH - response_overhead)) {
    data_[length] = 0;
  } else {
    // TODO: handle this. We'll have to send the response back in two parts.
  }

  std::size_t response_len = snprintf(response, MAX_LENGTH, RESPONSE,
      length, "text/plain", data_);

  debugf("Session::process_response", "process_response generated response of "
      "length %d\n", response_len);

  do_write(response, response_len);
  free(response);
}

void Session::do_write(const char *msg, std::size_t length)
{
  auto self(shared_from_this());

  debugf("Session::do_write", "do_write sending response of length %d\n",
      length);

  boost::asio::async_write(socket_, boost::asio::buffer(msg,
        length),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec) {
          do_read();
        }
      });
}
