#include "../../include/rl/RateLimiterFactory.hpp"

#include "../../include/rl/algorithms/TokenBucket.hpp"

std::shared_ptr<ARateLimiter> RateLimiterFactory::createRateLimiter(
    const std::string &algorithm, int capacity, int rate,
    std::string timeUnit) {
  /*
   * This function creates a rate limiter based on the algorithm provided
   * @param algorithm: The algorithm to use for rate limiting valid values are
   * (token_bucket)
   * @param capacity: The capacity of the rate limiter
   * @param rate: The rate at which the rate limiter should be refilled
   * @param timeUnit: The time unit for the rate limiter valid values are
   * (second, minute, hour, day)
   */
  if (algorithm == "token_bucket") {
    return std::make_shared<TokenBucket>(capacity, rate, timeUnit);
  }
  return nullptr;
}
