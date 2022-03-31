.PHONY: run wsl debug windows
run:
	clear
	./test.out matrix.dat vehicletype.dat 10000 1000 1000
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