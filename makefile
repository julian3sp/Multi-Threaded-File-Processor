# Makefile for the project 4

# Compiler and flags
proj4: proj4.cpp
	g++ -g -Wall -o proj4 proj4.cpp -lpthread

# Clean target to remove generated files
clean:
	rm -f proj4