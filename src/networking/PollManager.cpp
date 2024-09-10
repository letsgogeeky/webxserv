#include "../../include/networking/PollManager.hpp"

#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <vector>

PollManager::PollManager() {}

void PollManager::addSocket(int fd, int events) {
  struct pollfd pfd;
  pfd.fd = fd;
  pfd.events = events;
  pfd.revents = 0;
  _fds.push_back(pfd);
}

void PollManager::removeSocket(int fd) {
  if (fd < 0) {
    throw invalidFd(fd);
  }

  for (size_t i = 0; i < _fds.size(); ++i) {
    if (_fds[i].fd == fd) {
      std::swap(_fds[i], _fds.back());
      _fds.pop_back();
      break;
    }
  }
}

void PollManager::pollSockets() {
  int poll_count = poll(&_fds[0], _fds.size(), 0);
  if (poll_count < 0) {
    throw pollFailed(std::strerror(errno));
  }
}

std::vector<struct pollfd>& PollManager::getFds() { return _fds; }