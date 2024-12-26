
#include "../../include/rl/algorithms/TokenBucket.hpp"

TokenBucket::TokenBucket(int capacity, int rate) {
  _capacity = capacity;
  _rate = rate;
  _tokens = capacity;
  _lastRefill = std::chrono::system_clock::now();
  _running = true;
  _refillThread = std::thread(&TokenBucket::refill, this);
}

bool TokenBucket::consume(int tokens) {
  std::lock_guard<std::mutex> lock(_mutex);
  if (_tokens >= tokens) {
    _tokens -= tokens;
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
