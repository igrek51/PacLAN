BIN = bin
CC = g++
CFLAGS = -c -Wall -std=c++11
LFLAGS = -Wall -s
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lboost_system -lboost_thread
OUTPUT_NAME = paclan
OBJS = "obj/graphics_animations_animation.o" "obj/graphics_animations_death_animation.o" "obj/graphics_graphics.o" "obj/graphics_graphics_draw.o" "obj/app.o" "obj/config.o" "obj/language.o" "obj/log_log.o" "obj/logic_game_cmd.o" "obj/logic_game_engine.o" "obj/logic_game_logic.o" "obj/logic_game_map.o" "obj/main.o" "obj/map_item.o" "obj/map_map.o" "obj/map_pathfind_node.o" "obj/map_pathfind_path.o" "obj/map_pathfind_pathfind.o" "obj/network_game_network.o" "obj/network_network.o" "obj/network_network_connection.o" "obj/network_network_task.o" "obj/players_ghost.o" "obj/players_pacman.o" "obj/players_player.o" "obj/system.o" "obj/threads_continuous_thread.o" "obj/threads_thread.o" "obj/threads_timer.o" "obj/utils.o"


all: $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(LFLAGS) -o $(BIN)/$(OUTPUT_NAME)

clean:
	del /Q obj\*.o
	del /Q $(BIN)\$(OUTPUT_NAME)


obj/graphics/animations/animation.o: src/graphics/animations/animation.cpp
	$(CC) $(CFLAGS) src/graphics/animations/animation.cpp -o "obj/graphics/animations/animation.o"

obj/graphics/animations/death_animation.o: src/graphics/animations/death_animation.cpp
	$(CC) $(CFLAGS) src/graphics/animations/death_animation.cpp -o "obj/graphics/animations/death_animation.o"

obj/graphics/graphics.o: src/graphics/graphics.cpp
	$(CC) $(CFLAGS) src/graphics/graphics.cpp -o "obj/graphics/graphics.o"

obj/graphics/graphics_draw.o: src/graphics/graphics_draw.cpp
	$(CC) $(CFLAGS) src/graphics/graphics_draw.cpp -o "obj/graphics/graphics_draw.o"

obj/app.o: src/app.cpp
	$(CC) $(CFLAGS) src/app.cpp -o "obj/app.o"

obj/config.o: src/config.cpp
	$(CC) $(CFLAGS) src/config.cpp -o "obj/config.o"

obj/language.o: src/language.cpp
	$(CC) $(CFLAGS) src/language.cpp -o "obj/language.o"

obj/log/log.o: src/log/log.cpp
	$(CC) $(CFLAGS) src/log/log.cpp -o "obj/log/log.o"

obj/logic/game_cmd.o: src/logic/game_cmd.cpp
	$(CC) $(CFLAGS) src/logic/game_cmd.cpp -o "obj/logic/game_cmd.o"

obj/logic/game_engine.o: src/logic/game_engine.cpp
	$(CC) $(CFLAGS) src/logic/game_engine.cpp -o "obj/logic/game_engine.o"

obj/logic/game_logic.o: src/logic/game_logic.cpp
	$(CC) $(CFLAGS) src/logic/game_logic.cpp -o "obj/logic/game_logic.o"

obj/logic/game_map.o: src/logic/game_map.cpp
	$(CC) $(CFLAGS) src/logic/game_map.cpp -o "obj/logic/game_map.o"

obj/main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o "obj/main.o"

obj/map/item.o: src/map/item.cpp
	$(CC) $(CFLAGS) src/map/item.cpp -o "obj/map/item.o"

obj/map/map.o: src/map/map.cpp
	$(CC) $(CFLAGS) src/map/map.cpp -o "obj/map/map.o"

obj/map/pathfind/node.o: src/map/pathfind/node.cpp
	$(CC) $(CFLAGS) src/map/pathfind/node.cpp -o "obj/map/pathfind/node.o"

obj/map/pathfind/path.o: src/map/pathfind/path.cpp
	$(CC) $(CFLAGS) src/map/pathfind/path.cpp -o "obj/map/pathfind/path.o"

obj/map/pathfind/pathfind.o: src/map/pathfind/pathfind.cpp
	$(CC) $(CFLAGS) src/map/pathfind/pathfind.cpp -o "obj/map/pathfind/pathfind.o"

obj/network/game_network.o: src/network/game_network.cpp
	$(CC) $(CFLAGS) src/network/game_network.cpp -o "obj/network/game_network.o"

obj/network/network.o: src/network/network.cpp
	$(CC) $(CFLAGS) src/network/network.cpp -o "obj/network/network.o"

obj/network/network_connection.o: src/network/network_connection.cpp
	$(CC) $(CFLAGS) src/network/network_connection.cpp -o "obj/network/network_connection.o"

obj/network/network_task.o: src/network/network_task.cpp
	$(CC) $(CFLAGS) src/network/network_task.cpp -o "obj/network/network_task.o"

obj/players/ghost.o: src/players/ghost.cpp
	$(CC) $(CFLAGS) src/players/ghost.cpp -o "obj/players/ghost.o"

obj/players/pacman.o: src/players/pacman.cpp
	$(CC) $(CFLAGS) src/players/pacman.cpp -o "obj/players/pacman.o"

obj/players/player.o: src/players/player.cpp
	$(CC) $(CFLAGS) src/players/player.cpp -o "obj/players/player.o"

obj/system.o: src/system.cpp
	$(CC) $(CFLAGS) src/system.cpp -o "obj/system.o"

obj/threads/continuous_thread.o: src/threads/continuous_thread.cpp
	$(CC) $(CFLAGS) src/threads/continuous_thread.cpp -o "obj/threads/continuous_thread.o"

obj/threads/thread.o: src/threads/thread.cpp
	$(CC) $(CFLAGS) src/threads/thread.cpp -o "obj/threads/thread.o"

obj/threads/timer.o: src/threads/timer.cpp
	$(CC) $(CFLAGS) src/threads/timer.cpp -o "obj/threads/timer.o"

obj/utils.o: src/utils.cpp
	$(CC) $(CFLAGS) src/utils.cpp -o "obj/utils.o"

