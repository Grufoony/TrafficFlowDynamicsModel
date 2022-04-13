.PHONY: run wsl debug windows visual
run:
	clear
	./test.out matrix.dat vehicletype.dat 10000 4000 200
wsl:
	clear
	g++ -O3 main.cpp Graph.cpp Street.cpp Vehicle.cpp VehicleType.cpp -o test.out
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
	g++ ./source/main.cpp ./source/Graph.cpp ./source/Street.cpp ./source/Vehicle.cpp ./source/VehicleType.cpp -o test.out -Wall -Wextra -fsanitize=address
visual:
	clear
	python3 visual.py