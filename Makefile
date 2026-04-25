all: compile run


compile:
	g++ main.cpp -o main.exe -I C:/raylib/raylib/src -L C:/raylib/raylib/src -I C:/Curl/include -L C:/Curl/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lcurl

run:
	.\main.exe