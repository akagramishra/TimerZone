all: compile run


compile:
	g++ main.cpp src/network.cpp src/app.cpp src/ui.cpp -o main.exe -I C:/raylib/raylib/src -L C:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -lcurl -lws2_32 -lwldap32 -lcrypt32 -mwindows
run:
	.\main.exe