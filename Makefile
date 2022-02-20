all:
	clang-format -i main.cpp
	clang-format -i network.cpp
	clang-format -i network.hpp
	clear
	g++ main.cpp network.cpp -o test.out -Wall -Wextra