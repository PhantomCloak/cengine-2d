CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors -g
INCLUDE_PATHS = -I"./libs" \
		-I"./libs/lua" \
		-I"./libs/imgui" \
		-I"./libs/SimpleJSON" \
		-I"./libs/box2d/include" \
		-I"./libs/imgui/backends" \
		-I/opt/homebrew/include \
		-I./libs/glad/include \
		-I./libs/stb \
		`pkg-config --cflags glfw3` \
		`pkg-config --cflags-only-I freetype2`
LIBRARY_PATHS = -L/opt/homebrew/lib
SRC_FILES = ./libs/glad/src/glad.c \
	    ./src/*.cpp \
	    ./src/game/*.cpp \
	    ./src/io/*.cpp \
	    ./src/ecs/*.cpp \
	    ./src/log/*.cpp \
	    ./src/core/*.cpp \
	    ./src/physics/*.cpp \
	    ./src/systems/*.cpp \
	    ./src/editor/*.cpp \
	    ./src/scene/*.cpp \
	    ./src/common/*.cpp \
	    ./src/assetmgr/*.cpp \
	    ./src/scripting/*.cpp \
	    ./src/render/*.cpp \
	    ./src/render/backends/render_opengl.cpp \
	    ./src/render/opengl/*.cpp \
	    ./libs/imgui/*.cpp \
	    ./libs/box2d/src/*/*.cpp \
	    ./libs/imgui/backends/imgui_impl_glfw.cpp \
	    ./libs/imgui/backends/imgui_impl_opengl3.cpp
LINKER_FLAGS = `pkg-config --static --libs glfw3` `pkg-config --static --libs freetype2` -llua -framework IOKit -framework Cocoa -framework OpenGL
BINARY_NAME = commanche2d


build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATHS) $(SRC_FILES) $(LINKER_FLAGS) $(LIBRARY_PATHS) -o commanche2d
dev:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATHS) $(SRC_FILES) $(LINKER_FLAGS) $(LIBRARY_PATHS) -DEDITOR=1 -o commanche2d
run:
	./commanche2d

clean:
	rm renderer
