// Base Rate Limiter class

#pragma once
#include <string>

class ARateLimiter {
 public:
  virtual bool consume() = 0;
  virtual ~ARateLimiter() = default;
};
