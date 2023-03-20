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
linux:
	clear
	g++ -O3 ./main.cpp ./src/Graph.cpp ./src/Street.cpp ./src/Vehicle.cpp ./src/VehicleType.cpp -o test.wsl
windows:
	clear
	x86_64-w64-mingw32-g++ -O3 main.cpp Graph.cpp Street.cpp Vehicle.cpp VehicleType.cpp -o test.exe
debug:
	clear
	g++ -std=c++20 -O3 ./main.cpp ./src/Graph.cpp ./src/Street.cpp ./src/Vehicle.cpp ./src/VehicleType.cpp -o test.wsl -Wall -Wextra -fsanitize=address
visual:
	clear
	python3 ./utils/visual.py
docs:
	clear
	clang-format -i ./*.cpp
	clang-format -i ./src/*.cpp
	clang-format -i ./src/*.hpp
	clang-format -i ./utils/*.cpp
	clang-format -i ./utils/*.hpp
	doxygen Doxyfile
test:
	clear
	g++ -std=c++20 -O3 ./test.cpp -o test.out
	./test.out
	rm -r ./test.out