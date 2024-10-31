#include "Server.hpp"

int main() {
  std::unique_ptr<ConfigFile> config = std::make_unique<ConfigFile>();
  Server server(std::move(config));
  server.run();
}
