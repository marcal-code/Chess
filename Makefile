
# Makefile made for mingw32-make targetting windows operating system.
# Make sure you edit the proper path of LIBS and INCLUDE dir of SDL2.
# Assuming SDL2_image lib and include are in the same dir as SDL2 lib and include.

CC = g++
CFLAGS = -std=c++17 -Wall -Werror
INCLUDES = -IC:/dev-libs/SDL2/include
LIBS = -LC:/dev-libs/SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

SOURCES = ./src/main.cpp \
          ./src/sdl_handler.cpp \
          ./src/board.cpp \
          ./src/chess.cpp

EXECUTABLE = chess.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o $@ $(LIBS)

clean:
	del $(EXECUTABLE)
