#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>

#include "SocketExceptions.hpp"

namespace net {

class Socket {
 public:
  Socket(int domain, int type, int protocol);
  ~Socket();

  void bindSocket(int port);
  void listenSocket(int backlog);
  int acceptConnection(struct sockaddr_in *address, socklen_t *addrlen);
  void setSocketOption(int level, int optname, int optval);
  int readData(int sockfd, char *buffer, size_t size);
  int sendData(int sockfd, const char *buffer, size_t size);
  void setNonBlocking();
  int getSocketFd() const;

 private:
  int sockFd_;
};

}  // namespace net

#endif
