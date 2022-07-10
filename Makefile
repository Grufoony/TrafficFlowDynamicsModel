.PHONY: traveltime constant peaked periodic linux debug windows visual docs
traveltime:
	clear
	./test.wsl ./data/matrix.dat ./data/vehicletype.dat 300 200 15001
constant:
	clear
	./test.wsl ./data/matrix.dat ./data/vehicletype.dat 300 250 15001
peaked:
	clear
	./test.wsl ./data/matrix.dat ./data/vehicletype.dat 300 25000 12000
periodic:
	clear
	./test.wsl ./data/matrix.dat ./data/vehicletype.dat 300 2100 32400 55000
test:
	clear
	./test.wsl ./data/matrix_old.dat ./data/vehicletype_old.dat 300 0 0
linux:
	clear
	g++ -O3 ./main.cpp ./source/Graph.cpp ./source/Street.cpp ./source/Vehicle.cpp ./source/VehicleType.cpp -o test.wsl
windows:
	clear
	x86_64-w64-mingw32-g++ -O3 main.cpp Graph.cpp Street.cpp Vehicle.cpp VehicleType.cpp -o test.exe
debug:
	clang-format -i ./main.cpp
	clang-format -i ./source/Vehicle.cpp
	clang-format -i ./source/Vehicle.hpp
	clang-format -i ./source/VehicleType.cpp
	clang-format -i ./source/VehicleType.hpp
	clang-format -i ./source/Street.cpp
	clang-format -i ./source/Street.hpp
	clang-format -i ./source/Graph.cpp
	clang-format -i ./source/Graph.hpp
	clang-format -i ./utils/macro.cpp
	clear
	g++ -std=c++17 -O3 ./main.cpp ./source/Graph.cpp ./source/Street.cpp ./source/Vehicle.cpp ./source/VehicleType.cpp -o test.wsl -Wall -Wextra -fsanitize=address
visual:
	clear
	python3 ./utils/visual.py
docs:
	clear
	doxygen Doxyfile