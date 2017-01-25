// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// TODO refactor most of this code into session.cc

#ifndef SESSION_INCLUDED
#define SESSION_INCLUDED

#include <boost/asio.hpp>

#include "helpers.h"

#define RESPONSE "HTTP/1.1 200 OK\r\n"\
  "Content-Length: %lu\r\n"\
  "Content-Type: %s\r\n"\
  "Connection: Closed\r\n"\
  "\r\n"\
  "%s"

using boost::asio::ip::tcp;

class Session
: public std::enable_shared_from_this<Session>
{
  public:
    Session(tcp::socket socket)
      : socket_(std::move(socket))
    {
    }

    /*
     * Entry point to a per-client session. Each session consists of a single
     * read followed by a write.
     */
    void start()
    {
      do_read();
    }

  private:
    /*
     * Read data from the client. On success, write back a response.
     */
    void do_read()
    {
      auto self(shared_from_this());
      socket_.async_read_some(boost::asio::buffer(data_, max_length),
          [this, self](boost::system::error_code ec, std::size_t length)
          {
          if (!ec)
          {
          debugf("do_read got message of length %lu\n", length);
          do_write(length);
          }
          });
    }

    void do_write(std::size_t length)
    {
      auto self(shared_from_this());
      char *response = (char*) malloc(max_length);
      if (response == NULL) {
        debugf("do_write failed to allocate response buffer.\n");
        return;
      }

      // Apparently boost doesn't care to null terminate its strings, so we
      // have to do it ourselves.
      if (length < max_length) {
        data_[length] = 0;
      } else {
        // TODO: handle this
        data_[max_length] = 0;
      }

      int response_len = snprintf(response, max_length, RESPONSE,
          length, "text/plain", data_);

      debugf("do_write sending response of length %d\n", response_len);

      if (response_len >= max_length) {
        // Output was truncated. TODO: handle this
        debugf("do_write had to truncate response.\n");
      }
      boost::asio::async_write(socket_, boost::asio::buffer(response,
            response_len),
          [this, self](boost::system::error_code ec, std::size_t /*length*/)
          {
          if (!ec)
          {
          do_read();
          }
          });
    }

    tcp::socket socket_;
    enum { max_length = 65536 }; // TODO: why is this an enum?
    char data_[max_length];
};

#endif
