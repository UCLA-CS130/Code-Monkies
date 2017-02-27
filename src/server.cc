// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "dispatcher.h"
#include "server.h"
#include "helpers.h"
#include <iostream>

using helpers::debugf;

Server::Server(boost::asio::io_service& io_service, const NginxConfig *conf)
  : acceptor_(io_service, tcp::endpoint(tcp::v4(), Config::GetPort(conf))),
    socket_(io_service), conf_(conf)
{
  Dispatcher::Init(); // per the API spec, dispatchers should be initialized at
      // server instantiation
  debugf("Server::Server", "Server successfully instantiated.\n");
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
        debugf("Server::do_accept", "Accepted new connection.\n");
        std::make_shared<Session>(std::move(socket_), conf_)->start();
      	this->do_accept();
      });
}
