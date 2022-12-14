CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors -g
INCLUDE_PATHS = -I"./libs" \
		-I"./libs/lua" \
		-I"./libs/imgui" \
		-I"./libs/imgui/backends" \
		-I/opt/homebrew/include 
LIBRARY_PATHS = -L/opt/homebrew/lib 
SRC_FILES = ./src/*.cpp \
	    ./src/game/*.cpp \
	    ./src/ecs/*.cpp \
	    ./src/log/*.cpp \
	    ./src/editor/*.cpp \
	    ./src/assetmgr/*.cpp \
	    ./src/scripting/*.cpp \
	    ./src/render/*.cpp \
	    ./libs/imgui/*.cpp \
	    ./libs/imgui/backends/imgui_impl_sdl.cpp \
	    ./libs/imgui/backends/imgui_impl_opengl2.cpp
LINKER_FLAGS = `sdl2-config --cflags` `sdl2-config --libs` -lSDL2_ttf -lSDL2_image -llua -framework OpenGL
BINARY_NAME = commanche2d


build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATHS) $(SRC_FILES) $(LINKER_FLAGS) $(LIBRARY_PATHS) -o commanche2d

run:
	./commanche2d

clean:
	rm renderer
