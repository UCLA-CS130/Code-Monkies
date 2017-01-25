// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// TODO refactor most of this code into server.cc

#ifndef SERVER_INCLUDED
#define SERVER_INCLUDED

#include "session.h"

using boost::asio::ip::tcp;

class Server
{
  public:
    Server(boost::asio::io_service& io_service, short port)
      : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      socket_(io_service)
  {
    do_accept();
  }

  private:
    void do_accept()
    {
      acceptor_.async_accept(socket_,
          [this](boost::system::error_code ec)
          {
          if (!ec)
          {
          std::make_shared<Session>(std::move(socket_))->start();
          }

          do_accept();
          });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

#endif
