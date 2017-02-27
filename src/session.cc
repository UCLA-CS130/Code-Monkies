// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "api/request.h"
#include "api/request_handler.h"
#include "dispatcher.h"
#include "request_logger.h"
#include "session.h"
#include <math.h>

using helpers::debugf;

void Session::do_read()
{
  auto self(shared_from_this());
  // TODO: what if the client sends us > 64 KB?
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
  auto req = Request::Parse(data_);

  Response res;
  boost::system::error_code ec;

  RequestHandler *handler = nullptr;
  std::string handler_name;

  if (!Dispatcher::Dispatch(req.get(), conf_, handler_name, handler)) {
    debugf("Session::process_response", "Failed to dispatch to handler.\n");
    return;
  }

  NginxConfigStatement *info;

  if (!Config::GetHandlerInfo(conf_, req.get(), info)) {
    debugf("Session::process_response", "Failed to get handler info from "
        "configuration. Handler name: %s.\n");
    return;
  }

  handler->Init(info->tokens_[1], *conf_);
  handler->HandleRequest(*(std::move(req)), &res);

  RequestLogger::GetInstance().RegisterRequest(handler_name, *req,
      res.status());

  do_write(res);

  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  socket_.close();

  return;
}

void Session::do_write(const Response &res)
{
  auto self(shared_from_this());

  const std::string res_str = res.ToString();
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
