#ifndef CONFIG_H
#define CONFIG_H

#include <list>
#include <unordered_map>
#include <unordered_set>

#include "api/request.h"
#include "config_parser.h"

#define PORT_MAX 65535

/*
 * Helper methods to validate and navigate an NginxConfig.
 */
namespace Config {
  /*
   * Check whether conf conforms to class's style.
   */
  bool Validate(const NginxConfig *conf);

  /*
   * Return port from conf.
   * No need to return a boolean since conf is assumed to have been validated
   * by the time this is called.
   */
  unsigned short GetPort(const NginxConfig *conf);

  /*
   * Given a request, find the handler with the longest matching URI prefix
   * and return the NginxConfigStatement corresponding to that block.
   */
  bool GetHandlerInfo(const NginxConfig *conf,
      const Request *req, NginxConfigStatement *&info);

  /*
   * Return NginxConfigStatement corresponding to each handler.
   */
  void GetAllHandlerInfo(const NginxConfig *conf,
      std::list<NginxConfigStatement const *> &info);
};

#endif
