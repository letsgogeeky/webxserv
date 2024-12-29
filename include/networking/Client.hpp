#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <memory>

#include "../../include/Event.hpp"
#include "../../include/request/HttpRequestParser.hpp"
#include "../../include/response/HttpResponse.hpp"
#include "../../include/rl/ARateLimiter.hpp"
#include "ServerContext.hpp"
#include "SocketExceptions.hpp"

class Client {
 public:
  Client(int fd, std::shared_ptr<ServerContext> context);
  // Client(sockaddr_in address, int fd,
  //        std::shared_ptr<ServerContext> context);
  ~Client();

  int getFd() const;
  bool handleRequest();

  bool isReadyForResponse();
  bool isReadyForRequest();
  bool shouldSendContinue();
  bool shouldCloseConnection(bool force);
  void setReadyForResponse(bool ready);
  bool execute();

 private:
  int _fd;
  // sockaddr_in _address;
  std::shared_ptr<ServerContext> _context;
  HttpRequestParser _parser;
  HttpResponse _response;
  bool _shouldSendContinue;
  bool _isReadyForResponse;
  bool _isReadyForRequest;
  bool _shouldCloseConnection;
  std::chrono::system_clock::time_point _lastRequestTime;

  std::shared_ptr<ARateLimiter> _rateLimiter;

  bool sendDefaultFavicon();
  bool sendWebDocument();
  bool sendDirectoryListings(const std::string& path);
  bool sendErrorPage(int status);
  std::string getErrorPagePath(int status);
  int getStatusCodeForErrorPage(const std::string& path);

  std::string generateDirectoryListing(const std::string& path,
                                       const std::string& requestUri,
                                       const std::string& inject);
  //   bool handleContinue();
  bool handleRedirect();
  void reset();
};

#endif  // CLIENT_H
