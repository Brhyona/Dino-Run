all: game

game: background.cpp src/setup/imageLoader.h src/setup/Global.h src/setup/jpompa.h jpompa.cpp
	g++ background.cpp jpompa.cpp -o game -Wall -lX11 -lGL -lGLU -lm

clean:
	rm -f game