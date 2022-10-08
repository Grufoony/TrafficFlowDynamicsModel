#

g++ -std=c++11 -fPIC $(python3 -m pybind11 --includes) binding.cc -o Graph$(python3-config --extension-suffix)
