#ifndef REQUEST_LOGGER_H
#define REQUEST_LOGGER_H

#include "api/response.h"
#include <ctime>
#include <list>

/*
 * Length various fields are capped at in response. Done to make responses look
 * pretty.
 */
#define HANDLER_NAME_LEN  24
#define URL_LEN           32

/*
 * Basic log entry.
 */
struct RequestInfo
{
  std::string time_finished;
  std::string handler_name;
  std::string url;
  int response_code;
};

/*
 * RequestLogger class.
 */
class RequestLogger
{
  public:
    static RequestLogger& GetInstance()
    {
      static RequestLogger r;
      return r;
    }

    void RegisterRequest(const std::string& handler_name,
        const Request& request, const Response::ResponseCode& code)
    {
      RequestInfo info;

      auto cur_time = std::time(nullptr);
      info.time_finished =
        std::string(std::asctime(std::gmtime(&cur_time)));
      // remove trailing newline
      info.time_finished.resize(info.time_finished.size() - 1);

      info.handler_name = handler_name;
      info.handler_name.resize(HANDLER_NAME_LEN, ' ');

      info.url = request.uri();
      info.url.resize(URL_LEN, ' ');

      info.response_code = code;

      request_log_.push_back(info);
    }

    const std::list<RequestInfo>* GetLog()
    {
      return &request_log_;
    }

  private:
    RequestLogger() {}

    /*
     * The log.
     */
    std::list<RequestInfo> request_log_;

};

#endif
