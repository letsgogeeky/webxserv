#pragma once
#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

class TokenBucket {
 public:
  // Rate in tokens per second
  // Capacity in tokens
  TokenBucket(int capacity, int rate);
  bool consume(int tokens);
  void refill();
  ~TokenBucket();

 private:
  int _capacity;
  int _rate;
  std::atomic<int> _tokens;
  std::chrono::system_clock::time_point _lastRefill;
  std::mutex _mutex;
  std::thread _refillThread;
  std::atomic<bool> _running;
};