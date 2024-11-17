all: game

game: background.cpp src/setup/imageLoader.h src/setup/Global.h src/setup/jpompa.h jpompa.cpp bthomas.cpp nflanders.cpp melvir.cpp src/setup/melvir.h sbeebe.cpp src/setup/health.h health.cpp
	g++ background.cpp jpompa.cpp bthomas.cpp nflanders.cpp melvir.cpp sbeebe.cpp health.cpp -o game -Wall -lX11 -lGL -lGLU -lm libggfonts.a

clean:
	rm -f game
