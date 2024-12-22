all:
	g++ -I src/include -L src/lib/x64 -o build/fluidSimulation fluidSimulation.cpp -lSDL2main -lSDL2
	copy src\lib\x64\SDL2.dll build\
