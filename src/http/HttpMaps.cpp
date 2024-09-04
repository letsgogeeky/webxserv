#include "../../include/request/HttpMaps.hpp"

const std::unordered_map<std::string, HttpRequestMethod>
    HttpMaps::httpRequestMethodMap = initHttpRequestMethodMap();

const std::unordered_map<std::string, HttpRequestVersion>
    HttpMaps::httpRequestVersionMap = initHttpRequestVersionMap();

const std::set<std::string> HttpMaps::headerSet = initHeaderSet();
const std::set<std::string> HttpMaps::requiredHeaders1_1 =
    initRequiredHeaders1_1();

std::unordered_map<std::string, HttpRequestMethod>
HttpMaps::initHttpRequestMethodMap() {
  std::unordered_map<std::string, HttpRequestMethod> map;
  map["GET"] = HttpRequestMethod::GET;
  map["POST"] = HttpRequestMethod::POST;
  map["PUT"] = HttpRequestMethod::PUT;
  map["DELETE"] = HttpRequestMethod::DELETE;
  map["HEAD"] = HttpRequestMethod::HEAD;
  map["OPTIONS"] = HttpRequestMethod::OPTIONS;
  map["PATCH"] = HttpRequestMethod::PATCH;
  map["TRACE"] = HttpRequestMethod::TRACE;
  map["CONNECT"] = HttpRequestMethod::CONNECT;
  return map;
}

std::unordered_map<std::string, HttpRequestVersion>
HttpMaps::initHttpRequestVersionMap() {
  std::unordered_map<std::string, HttpRequestVersion> map;
  map["HTTP/1.0"] = HttpRequestVersion::HTTP_1_0;
  map["HTTP/1.1"] = HttpRequestVersion::HTTP_1_1;
  map["HTTP/2.0"] = HttpRequestVersion::HTTP_2_0;
  return map;
}

std::set<std::string> HttpMaps::initHeaderSet() {
  std::set<std::string> set;
  set.insert("Host");
  set.insert("User-Agent");
  set.insert("Accept");
  set.insert("Accept-Language");
  set.insert("Accept-Encoding");
  set.insert("Connection");
  set.insert("Upgrade-Insecure-Requests");
  set.insert("Cache-Control");
  set.insert("Content-Type");
  set.insert("Content-Length");
  set.insert("Origin");
  set.insert("Referer");
  set.insert("Cookie");
  set.insert("Set-Cookie");
  set.insert("Authorization");
  set.insert("Content-Disposition");
  set.insert("Expect");
  return set;
}

std::set<std::string> HttpMaps::initRequiredHeaders1_1() {
  std::set<std::string> set;
  set.insert("Host");
  return set;
}

std::string HttpMaps::getMimeType(const std::string &path) {
  size_t pos = path.find_last_of('.');
  if (pos == std::string::npos) {
    return "text/plain";
  }
  std::string extension = path.substr(pos + 1);
  std::unordered_map<std::string, std::string> mimeMap = {
      {"html", "text/html"},        {"htm", "text/html"},
      {"css", "text/css"},          {"js", "application/javascript"},
      {"json", "application/json"}, {"jpg", "image/jpeg"},
      {"jpeg", "image/jpeg"},       {"png", "image/png"},
      {"gif", "image/gif"},         {"ico", "image/x-icon"},
      {"pdf", "application/pdf"},   {"zip", "application/zip"},
      {"txt", "text/plain"}};

  auto it = mimeMap.find(extension);
  if (it != mimeMap.end()) {
    return it->second;
  }
  return "application/octet-stream";
}
