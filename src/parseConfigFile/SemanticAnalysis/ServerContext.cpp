#include "ServerContext.hpp"

#include <cstring>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

ServerContext::ServerContext() {
  _listenValue.clear();
  _serverNameValue.clear();
  _sslCertificateValue.clear();
  _sslCertificateKeyValue.clear();
  _indexValue.clear();
  _rootValue.clear();
  _clientMaxBodySizeValue = SIZE_MAX;
  _requestTimeoutValue = 60;
  _isSetClientMaxBodySizeValue = false;
  _rateLimitValue = false;
  _rateLimitUnitValue.clear();
  _rateLimitRequestsPerUnitValue = 0;
  _rateLimitBurstValue = 0;
  _rateLimitAlgorithmValue.clear();
}

void ServerContext::printServerContent() const noexcept {
  std::cout << "-Server-\n";
  printTypeFormat("client_max_body_size", _clientMaxBodySizeValue);
  printTypeFormat("ssl_certificate", _sslCertificateValue);
  printTypeFormat("ssl_certificate_key", _sslCertificateKeyValue);
  printTypeFormat("root", _rootValue);
  printTypeFormat("request_timeout", _requestTimeoutValue);
  printTypeFormat("upload_dir", _uploadDirValue);
  if (_rateLimitValue) {
    printTypeFormat("rate_limit", _rateLimitValue);
    printTypeFormat("rate_limit_unit", _rateLimitUnitValue);
    printTypeFormat("rate_limit_requests_per_unit",
                    _rateLimitRequestsPerUnitValue);
    printTypeFormat("rate_limit_burst", _rateLimitBurstValue);
    printTypeFormat("rate_limit_algorithm", _rateLimitAlgorithmValue);
  }
  vectorPrint(_indexValue, "index");
  vectorPrint(_serverNameValue, "server_name");
  listenPrint();
  errorPagePrint();

  auto it_location = _locationContext.begin();

  std::cout << std::endl;
  while (it_location != _locationContext.end()) {
    (*it_location)->printLocation();
    it_location++;
  }
}

void ServerContext::vectorPrint(
    const std::vector<std::pair<std::string, int>> &vec,
    const std::string &type) const noexcept {
  int i = 0;
  for (auto it = vec.begin(); it != vec.end(); it++) {
    std::cout << "[" << i << "] " << type << ": [" << (*it).first << "] ["
              << (*it).second << "]\n";
    i++;
  }
};

void ServerContext::listenPrint() const noexcept {
  int i = 0;
  for (auto it = _listenValue.begin(); it != _listenValue.end(); it++) {
    std::cout << "[" << i << "] Listen Value: [" << *it << "]\n";
    i++;
  }
  i = 0;
  for (auto it = _portWithAddressListenValue.begin();
       it != _portWithAddressListenValue.end(); it++) {
    std::cout << "[" << i << "] Listen IP and Port: [" << (*it).first << "] ["
              << (*it).second << "]\n";
  }
}

void ServerContext::errorPagePrint() const noexcept {
  int i = 0;
  for (auto it = _errorPageValue.begin(); it != _errorPageValue.end(); it++) {
    std::cout << "[" << i << "] Error page status code and file: ["
              << (*it).first << "] [" << (*it).second << "]\n";
  }
}
