.PHONY: run wsl debug windows visual
run:
	clear
	./test.out matrix.dat vehicletype.dat 0 10 10
wsl:
	clear
	g++ -O3 main.cpp Graph.cpp Street.cpp Vehicle.cpp VehicleType.cpp -o test.out
windows:
	clear
	x86_64-w64-mingw32-g++ -O3 main.cpp Graph.cpp Street.cpp Vehicle.cpp VehicleType.cpp -o test.exe
debug:
	clang-format -i main.cpp
	clang-format -i Vehicle.cpp
	clang-format -i Vehicle.hpp
	clang-format -i VehicleType.cpp
	clang-format -i VehicleType.hpp
	clang-format -i Street.cpp
	clang-format -i Street.hpp
	clang-format -i Graph.cpp
	clang-format -i Graph.hpp
	clear
	g++ main.cpp Graph.cpp Street.cpp Vehicle.cpp VehicleType.cpp -o test.out -Wall -Wextra -fsanitize=address
visual:
	clear
	python3 visual.py