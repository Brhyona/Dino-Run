all: game

game: background.cpp imageLoader.h
	g++ background.cpp platform.cpp -o game -Wall -lX11 -lGL -lGLU -lm

clean:
	rm -f game