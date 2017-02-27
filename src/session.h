// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>

#include "config.h"
#include "helpers.h"
#include "api/response.h"

#define RESPONSE "HTTP/1.1 200 OK\r\n"\
  "Content-Length: %lu\r\n"\
  "Content-Type: %s\r\n"\
  "Connection: Closed\r\n"\
  "\r\n"\
  "%s"

using boost::asio::ip::tcp;


/* Base class for an HTTP session.
 * 
 * When the server accepts a client, it creates a Session
 * that performs the actions that the client requests. The
 * server config dictates what action the session should perform.
 * For example, an EchoSession will echo back the request of the 
 * client in the body of the response.
 */
class Session
: public std::enable_shared_from_this<Session>
{
  public:
    Session(tcp::socket socket, const NginxConfig *conf)
      : socket_(std::move(socket)), conf_(conf)
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
     * Process data received from the client, and choose the appropriate
     * response handler. For now this just calls do_write.
     */
    void process_response();

    /*
     * Write msg to the client.
     * This is a best-effort function. Failure and success look the same.
     */
    void do_write(const Response& res);

    tcp::socket socket_;
    const NginxConfig *conf_;
    static const std::size_t MAX_LENGTH = 65536;
    char data_[MAX_LENGTH];
};

#endif
