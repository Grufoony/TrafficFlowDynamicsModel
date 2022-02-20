#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

namespace net {

class Network {
private:
  int lenght;
  int height;
  std::vector<std::vector<Street>> net;

public:
  Network(int, int);
};

} // namespace net

#endif