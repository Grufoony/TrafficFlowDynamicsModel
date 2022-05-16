.PHONY: run wsl debug windows visual test periodictest
run:
	clear
	./test.wsl matrix.dat vehicletype_old.dat 2000 20000 10001
periodictest:
	clear
	./test.wsl matrix.dat vehicletype_old.dat 6900 1300 32400 55000
test:
	clear
	./test.wsl matrix_old.dat vehicletype_old.dat 0 0 0
wsl:
	clear
	g++ -O3 ./source/main.cpp ./source/Graph.cpp ./source/Street.cpp ./source/Vehicle.cpp ./source/VehicleType.cpp -o test.wsl
windows:
	clear
	x86_64-w64-mingw32-g++ -O3 main.cpp Graph.cpp Street.cpp Vehicle.cpp VehicleType.cpp -o test.exe
debug:
	clang-format -i ./source/main.cpp
	clang-format -i ./source/Vehicle.cpp
	clang-format -i ./source/Vehicle.hpp
	clang-format -i ./source/VehicleType.cpp
	clang-format -i ./source/VehicleType.hpp
	clang-format -i ./source/Street.cpp
	clang-format -i ./source/Street.hpp
	clang-format -i ./source/Graph.cpp
	clang-format -i ./source/Graph.hpp
	clear
	g++ -std=c++17 -O3 ./source/main.cpp ./source/Graph.cpp ./source/Street.cpp ./source/Vehicle.cpp ./source/VehicleType.cpp -o test.wsl -Wall -Wextra -fsanitize=address
visual:
	clear
	python3 visual.py