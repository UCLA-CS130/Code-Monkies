// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

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
    void do_read();

    /*
     * Write data to the client. On success, listen for more data.
     */
    void do_write(std::size_t length);

    tcp::socket socket_;
    enum { max_length = 65536 }; // TODO: why is this an enum?
    char data_[max_length];
};

#endif
