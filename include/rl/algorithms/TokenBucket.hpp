#pragma once
#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

#include "../ARateLimiter.hpp"

class TokenBucket : public ARateLimiter {
 public:
  // Rate in tokens per second
  // Capacity in tokens
  TokenBucket(int capacity, int rate, std::string timeUnit);
  bool consume();
  void refill();
  ~TokenBucket();

 private:
  void setTimeUnitInSeconds(std::string timeUnit);
  int _capacity;
  int _rate;
  std::atomic<int> _tokens;
  std::chrono::system_clock::time_point _lastRefill;
  std::mutex _mutex;
  std::thread _refillThread;
  std::atomic<bool> _running;
  std::string _timeUnit;
  uint _timeUnitInSeconds;
};
