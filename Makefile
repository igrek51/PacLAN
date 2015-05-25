BIN = bin
CC = C:/MinGW/bin/mingw32-g++.exe
WINDRES = C:/MinGW/bin/windres.exe
CFLAGS = -c -Wall
LFLAGS = -s -mwindows -Wall
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lws2_32
PROJECT_NAME = PacLAN
STATIC_PROJECT_NAME = PacLAN-static
STATIC_LIB_NAME = paclan
SOURCES = app.cpp config.cpp game_map.cpp item.cpp log.cpp network_task.cpp pathfind.cpp thread.cpp timer.cpp animation.cpp game_cmd.cpp game_engine.cpp game_logic.cpp game_network.cpp ghost.cpp graphics.cpp graphics_draw.cpp main.cpp map.cpp network.cpp pacman.cpp player.cpp
SRC = .
STATIC_OBJ =  obj/graphics.cpp.o obj/graphics_draw.cpp.o obj/animation.cpp.o obj/player.cpp.o obj/pacman.cpp.o obj/ghost.cpp.o obj/item.cpp.o obj/map.cpp.o obj/pathfind.cpp.o obj/thread.cpp.o obj/timer.cpp.o obj/app.cpp.o obj/config.cpp.o obj/log.cpp.o
PROJECT_OBJ = obj/main.cpp.o obj/game_engine.cpp.o obj/game_logic.cpp.o obj/game_cmd.cpp.o obj/game_map.cpp.o obj/game_network.cpp.o obj/network.cpp.o obj/network_task.cpp.o obj/resource.rc.o


all: $(STATIC_OBJ) $(PROJECT_OBJ)
	$(CC) $(STATIC_OBJ) $(PROJECT_OBJ) $(LIBS) $(LFLAGS) -o $(BIN)/$(PROJECT_NAME)
	
static: $(STATIC_OBJ) $(PROJECT_OBJ)
	ar rcsv $(BIN)/$(STATIC_LIB_NAME).lib $(STATIC_OBJ) 
	$(CC) $(PROJECT_OBJ) $(LIBS) $(LFLAGS) -o $(BIN)/$(STATIC_PROJECT_NAME) -L$(BIN) -l$(STATIC_LIB_NAME)


obj/app.cpp.o: app.cpp
	$(CC) $(CFLAGS) app.cpp -o obj/app.cpp.o

obj/config.cpp.o: config.cpp
	$(CC) $(CFLAGS) config.cpp -o obj/config.cpp.o

obj/game_map.cpp.o: game_map.cpp
	$(CC) $(CFLAGS) game_map.cpp -o obj/game_map.cpp.o

obj/item.cpp.o: item.cpp
	$(CC) $(CFLAGS) item.cpp -o obj/item.cpp.o

obj/log.cpp.o: log.cpp
	$(CC) $(CFLAGS) log.cpp -o obj/log.cpp.o

obj/network_task.cpp.o: network_task.cpp
	$(CC) $(CFLAGS) network_task.cpp -o obj/network_task.cpp.o

obj/pathfind.cpp.o: pathfind.cpp
	$(CC) $(CFLAGS) pathfind.cpp -o obj/pathfind.cpp.o

obj/thread.cpp.o: thread.cpp
	$(CC) $(CFLAGS) thread.cpp -o obj/thread.cpp.o

obj/timer.cpp.o: timer.cpp
	$(CC) $(CFLAGS) timer.cpp -o obj/timer.cpp.o

obj/animation.cpp.o: animation.cpp
	$(CC) $(CFLAGS) animation.cpp -o obj/animation.cpp.o

obj/game_cmd.cpp.o: game_cmd.cpp
	$(CC) $(CFLAGS) game_cmd.cpp -o obj/game_cmd.cpp.o

obj/game_engine.cpp.o: game_engine.cpp
	$(CC) $(CFLAGS) game_engine.cpp -o obj/game_engine.cpp.o

obj/game_logic.cpp.o: game_logic.cpp
	$(CC) $(CFLAGS) game_logic.cpp -o obj/game_logic.cpp.o

obj/game_network.cpp.o: game_network.cpp
	$(CC) $(CFLAGS) game_network.cpp -o obj/game_network.cpp.o

obj/ghost.cpp.o: ghost.cpp
	$(CC) $(CFLAGS) ghost.cpp -o obj/ghost.cpp.o

obj/graphics.cpp.o: graphics.cpp
	$(CC) $(CFLAGS) graphics.cpp -o obj/graphics.cpp.o

obj/graphics_draw.cpp.o: graphics_draw.cpp
	$(CC) $(CFLAGS) graphics_draw.cpp -o obj/graphics_draw.cpp.o

obj/main.cpp.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o obj/main.cpp.o

obj/map.cpp.o: map.cpp
	$(CC) $(CFLAGS) map.cpp -o obj/map.cpp.o

obj/network.cpp.o: network.cpp
	$(CC) $(CFLAGS) network.cpp -o obj/network.cpp.o

obj/pacman.cpp.o: pacman.cpp
	$(CC) $(CFLAGS) pacman.cpp -o obj/pacman.cpp.o

obj/player.cpp.o: player.cpp
	$(CC) $(CFLAGS) player.cpp -o obj/player.cpp.o


obj/resource.rc.o: resource.rc
	$(WINDRES) resource.rc "obj/resource.rc.o"


clean:
	del /Q obj\*.o
	del /Q $(BIN)\$(PROJECT_NAME).exe
	del /Q $(BIN)\$(STATIC_PROJECT_NAME).exe
	del /Q $(BIN)\$(STATIC_LIB_NAME).lib
