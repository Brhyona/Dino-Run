all: game

game: background.cpp src/setup/imageLoader.h src/setup/Global.h src/setup/jpompa.h jpompa.cpp bthomas.cpp nflanders.cpp melvir.cpp src/setup/melvir.h sbeebe.cpp src/setup/health.h src/setup/oal.h
	g++ background.cpp jpompa.cpp bthomas.cpp nflanders.cpp melvir.cpp sbeebe.cpp -o game -Wall -lX11 -lGL -lGLU -lm libggfonts.a -lopenal -lalut -DUSE_OPENAL_SOUND -DGGWINDOWS 

clean:
	rm -f game
