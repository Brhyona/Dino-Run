all: game

game: background.cpp imageLoader.h platform.cpp
	g++ background.cpp platform.cpp -o game -Wall -lX11 -lGL -lGLU -lm

clean:
	rm -f game