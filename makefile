all: main.cpp block.cpp
	g++ main.cpp block.cpp -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -o tetris
clean:
	rm -rf *.o
