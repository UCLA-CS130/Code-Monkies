// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "server.h"
#include <iostream>

const int PORT_MAX = 65535;

Server::Server(boost::asio::io_service& io_service, int port)
  : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    socket_(io_service)
{ 
  if (port < 1 || port > PORT_MAX) {
    fprintf(stderr, "Port %d is out of range\n", port);
    exit(1);
  }
  this->do_accept();
}

void Server::do_accept()
{
  acceptor_.async_accept(socket_,
      [this](boost::system::error_code ec)
      {
        if (ec) {
      		std::cerr << ec.message() << std::endl;
      		exit(-1);
        }
        std::make_shared<Session>(std::move(socket_))->start();
      	this->do_accept();
      });
}
