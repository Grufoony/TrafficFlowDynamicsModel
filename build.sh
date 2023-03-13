#

echo "Building TrafficModel"
g++ -O3 -Wall -shared -std=c++20 -fPIC $(python3 -m pybind11 --includes) ./src/binding.cpp -o TrafficModel$(python3-config --extension-suffix)
echo "DONE"
echo "Building SparseMatrix"
g++ -O3 -Wall -shared -std=c++20 -fPIC $(python3 -m pybind11 --includes) ./utils/binding.cpp -o SparseMatrix$(python3-config --extension-suffix)
echo "DONE"
mv *.so ~/.local/lib/python*/site-packages