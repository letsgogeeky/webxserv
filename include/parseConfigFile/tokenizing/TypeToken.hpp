#ifndef TYPE_TOKEN_HPP
#define TYPE_TOKEN_HPP

#include <iostream>

enum class TypeToken {
  WORKER_PROCESS = 0,
  PID = 1,
  ERROR_LOG = 2,
  GEO_IP_COUNTRY = 3,
  PROXY_CACHE = 4,
  PROXY_CACHE_USE_STALE = 5,
  GZIP = 6,
  GZIP_TYPES = 7,
  LIMIT_RED_ZONE = 8,
  PROXY_SET_HEADER = 9,
  PROXY_CACHE_VALID = 10,
  SERVER_NAME = 11,
  SSL_CERTIFICATE = 12,
  SSL_CERTIFICATE_KEY = 13,
  INDEX = 14,
  ROOT = 15,
  LISTEN = 16,
  URL_LOCATION = 17,
  INCLUDE = 18,
  PROXY_PASS = 19,
  ALIAS = 20,
  TRY_FILES = 21,
  ERROR_PAGE = 22,
  ACCESS_LOG = 23,
  DENY = 24,
  CGI = 25,
  REWRITE = 26,
  SERVER = 27,
  HTTP = 28,
  LOCATION = 29,
  OPEN_BRACKET = 30,
  CLOSING_BRACKET = 31,
  DEFAULT = 32
};

std::ostream &operator << (std::ostream &outStream, const TypeToken &type);

#endif
