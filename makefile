main: src/main.c lib/cast.h lib/castB.h
	clang src/main.c -o main -lSDL3
	./main
