CC = g++
NAME = calc
CXXFLAGS = -std=c++20 -o $(NAME) -lncurses

all:
	$(CC) *.cpp $(CXXFLAGS)

debug:
	$(CC) *.cpp $(CXXFLAGS) -g
