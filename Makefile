all:
	g++ -std=c++11 main.cpp emulator.cpp `pkg-config --cflags --libs sdl2` -o chip8  
clean:
	rm -rf *o