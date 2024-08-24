#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <unistd.h>

#include "../../include/request/HttpRequestParser.hpp"
#include "../../include/response/HttpResponse.hpp"
#include "SocketExceptions.hpp"

class Client {
 public:
  Client(int fd);
  ~Client();

  int getFd() const;
  bool handleRequest();

 private:
  int fd;
  HttpRequestParser parser;
  HttpResponse response;
  bool sendDefaultFavicon();
  bool sendDefaultPage();
  bool handleContinue();
  bool execute();
};
#endif  // CLIENT_H
