all:
	clang-format -i main.cpp
	clang-format -i vehicle.cpp
	clang-format -i vehicle.hpp
	clang-format -i street.cpp
	clang-format -i street.hpp
	clang-format -i network.cpp
	clang-format -i network.hpp
	clear
	g++ main.cpp network.cpp street.cpp vehicle.cpp -o test.out -Wall -Wextra