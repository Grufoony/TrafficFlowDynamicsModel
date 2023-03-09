#

g++ -O3 -Wall -shared -std=c++20 -fPIC $(python3 -m pybind11 --includes) ./binding.cpp -o TrafficModel$(python3-config --extension-suffix)
