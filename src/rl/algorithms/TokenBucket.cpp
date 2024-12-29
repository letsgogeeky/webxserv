
#include "../../include/rl/algorithms/TokenBucket.hpp"

#include <algorithm>

TokenBucket::TokenBucket(int capacity, int rate, std::string timeUnit) {
  _capacity = capacity;
  _rate = rate;
  _tokens = capacity;
  _lastRefill = std::chrono::system_clock::now();
  _running = true;
  _timeUnit = timeUnit;
  _refillThread = std::thread(&TokenBucket::refill, this);
}

void TokenBucket::setTimeUnitInSeconds(std::string timeUnit) {
  if (timeUnit == "second") {
    _timeUnitInSeconds = 1;
  } else if (timeUnit == "minute") {
    _timeUnitInSeconds = 60;
  } else if (timeUnit == "hour") {
    _timeUnitInSeconds = 3600;
  } else if (timeUnit == "day") {
    _timeUnitInSeconds = 86400;
  } else {
    _timeUnitInSeconds = 1;
  }
}

bool TokenBucket::consume() {
  std::lock_guard<std::mutex> lock(_mutex);
  if (_tokens > 0) {
    _tokens -= 1;
    return true;
  }
  return false;
}

void TokenBucket::refill() {
  while (_running) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::lock_guard<std::mutex> lock(_mutex);
    auto now = std::chrono::system_clock::now();
    auto diff =
        std::chrono::duration_cast<std::chrono::seconds>(now - _lastRefill);
    _tokens =
        std::min(_capacity, static_cast<int>(_tokens + diff.count() * _rate));
    _lastRefill = now;
  }
}

TokenBucket::~TokenBucket() {
  _running = false;
  if (_refillThread.joinable()) {
    _refillThread.join();
  }
}
