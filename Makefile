all:
	clang-format -i main.cpp
	clang-format -i Vehicle.cpp
	clang-format -i Vehicle.hpp
	clang-format -i VehicleType.cpp
	clang-format -i VehicleType.hpp
	clang-format -i Street.cpp
	clang-format -i Street.hpp
	clang-format -i Network.cpp
	clang-format -i Network.hpp
	clear
	g++ main.cpp Street.cpp Vehicle.cpp VehicleType.cpp -o test.out -Wall -Wextra