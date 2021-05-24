LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -I/usr/include/SDL2
INCLUDED_CLASSES = util.cpp game_loop.cpp area.cpp player.cpp areablocks.cpp renderer.cpp visibleobject.cpp stillobject.cpp person.cpp npc.cpp dialogtext.cpp

all: game

clean:
	rm game.exe

game: game_loop.cpp
	g++ $(INCLUDED_CLASSES) $(LINKER_FLAGS) -o game.exe
