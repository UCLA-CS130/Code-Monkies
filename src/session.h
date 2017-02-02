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

    char *get_data() {
      return data_;
    }

  private:
    /*
     * Read data from the client. On success, write back a response.
     */
    void do_read();

    /*
     * Process data received from the client, and choose the appropriate
     * response handler. For now this just calls do_write.
     */
    void process_response(std::size_t length);

    /*
     * Write msg to the client. On success, listen for more data.
     */
    void do_write(const char *msg, std::size_t length);

    tcp::socket socket_;
    static const std::size_t MAX_LENGTH = 65536;
    char data_[MAX_LENGTH];
};

#endif
