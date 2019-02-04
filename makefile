all: main.cpp block.cpp
	g++ main.cpp block.cpp -lallegro -lallegro_primitives -o tetris
clean:
	rm -rf *.o
