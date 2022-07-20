all:
	g++ -Isrc/Include -Lsrc/lib -o main _main.cpp src/headers/*.cpp -lmingw32 -lSDL2_ttf -lSDL2_mixer -lSDL2main -lSDL2  -lSDL2_image 