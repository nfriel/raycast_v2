main: src/main.c lib/start.h lib/loop.h lib/cast.h
	clang src/main.c -o main -lSDL3 -std=c18
	./main
