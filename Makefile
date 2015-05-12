all:
	clang++-3.5 -std=c++11 test.cpp ../gtest/gtest-all.o -lpthread -o test.out
	clang++-3.5 -std=c++11 menu.cpp -o menu.out
