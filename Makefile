all: tile

tile: tile.cpp
	g++ -Wall -o tile -g tile.cpp `pkg-config vipsCC-7.10 --cflags --libs`
