all: compile run

compile:
	g++ main.cpp -o main.exe -I C:/raylib/raylib/src -L C:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm

run:
	.\main.exe