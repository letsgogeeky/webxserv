#include <ARateLimiter.hpp>
#include <memory>
#include <string>

class RateLimiterFactory {
 public:
  static std::shared_ptr<ARateLimiter> createRateLimiter(
      const std::string &algorithm, int capacity, int rate,
      std::string timeUnit);

 private:
  RateLimiterFactory() = delete;
  RateLimiterFactory(const RateLimiterFactory &) = delete;
  RateLimiterFactory &operator=(const RateLimiterFactory &) = delete;
};
