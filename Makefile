BIN = bin
CC = g++
CFLAGS = -c -Wall -std=c++11
LFLAGS = -Wall -s
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lboost_system -lboost_thread
OUTPUT_NAME = paclan
OBJS = "obj/graphics_animations_animation.o" "obj/graphics_animations_death_animation.o" "obj/graphics_graphics.o" "obj/graphics_graphics_draw.o" "obj/app.o" "obj/config.o" "obj/language.o" "obj/log_log.o" "obj/logic_game_cmd.o" "obj/logic_game_engine.o" "obj/logic_game_logic.o" "obj/logic_game_map.o" "obj/main.o" "obj/map_item.o" "obj/map_map.o" "obj/map_pathfind_node.o" "obj/map_pathfind_path.o" "obj/map_pathfind_pathfind.o" "obj/network_game_network.o" "obj/network_network.o" "obj/network_network_connection.o" "obj/network_network_task.o" "obj/players_ghost.o" "obj/players_pacman.o" "obj/players_player.o" "obj/threads_continuous_thread.o" "obj/threads_thread.o" "obj/threads_timer.o" "obj/utils.o"


all: $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(LFLAGS) -o $(BIN)/$(OUTPUT_NAME)

clean:
	rm obj/*.o
	rm $(BIN)/$(OUTPUT_NAME)


"obj/graphics_animations_animation.o":
	$(CC) $(CFLAGS) "src/graphics/animations/animation.cpp" -o "obj/graphics_animations_animation.o"

"obj/graphics_animations_death_animation.o":
	$(CC) $(CFLAGS) "src/graphics/animations/death_animation.cpp" -o "obj/graphics_animations_death_animation.o"

"obj/graphics_graphics.o":
	$(CC) $(CFLAGS) "src/graphics/graphics.cpp" -o "obj/graphics_graphics.o"

"obj/graphics_graphics_draw.o":
	$(CC) $(CFLAGS) "src/graphics/graphics_draw.cpp" -o "obj/graphics_graphics_draw.o"

"obj/app.o":
	$(CC) $(CFLAGS) "src/app.cpp" -o "obj/app.o"

"obj/config.o":
	$(CC) $(CFLAGS) "src/config.cpp" -o "obj/config.o"

"obj/language.o":
	$(CC) $(CFLAGS) "src/language.cpp" -o "obj/language.o"

"obj/log_log.o":
	$(CC) $(CFLAGS) "src/log/log.cpp" -o "obj/log_log.o"

"obj/logic_game_cmd.o":
	$(CC) $(CFLAGS) "src/logic/game_cmd.cpp" -o "obj/logic_game_cmd.o"

"obj/logic_game_engine.o":
	$(CC) $(CFLAGS) "src/logic/game_engine.cpp" -o "obj/logic_game_engine.o"

"obj/logic_game_logic.o":
	$(CC) $(CFLAGS) "src/logic/game_logic.cpp" -o "obj/logic_game_logic.o"

"obj/logic_game_map.o":
	$(CC) $(CFLAGS) "src/logic/game_map.cpp" -o "obj/logic_game_map.o"

"obj/main.o":
	$(CC) $(CFLAGS) "src/main.cpp" -o "obj/main.o"

"obj/map_item.o":
	$(CC) $(CFLAGS) "src/map/item.cpp" -o "obj/map_item.o"

"obj/map_map.o":
	$(CC) $(CFLAGS) "src/map/map.cpp" -o "obj/map_map.o"

"obj/map_pathfind_node.o":
	$(CC) $(CFLAGS) "src/map/pathfind/node.cpp" -o "obj/map_pathfind_node.o"

"obj/map_pathfind_path.o":
	$(CC) $(CFLAGS) "src/map/pathfind/path.cpp" -o "obj/map_pathfind_path.o"

"obj/map_pathfind_pathfind.o":
	$(CC) $(CFLAGS) "src/map/pathfind/pathfind.cpp" -o "obj/map_pathfind_pathfind.o"

"obj/network_game_network.o":
	$(CC) $(CFLAGS) "src/network/game_network.cpp" -o "obj/network_game_network.o"

"obj/network_network.o":
	$(CC) $(CFLAGS) "src/network/network.cpp" -o "obj/network_network.o"

"obj/network_network_connection.o":
	$(CC) $(CFLAGS) "src/network/network_connection.cpp" -o "obj/network_network_connection.o"

"obj/network_network_task.o":
	$(CC) $(CFLAGS) "src/network/network_task.cpp" -o "obj/network_network_task.o"

"obj/players_ghost.o":
	$(CC) $(CFLAGS) "src/players/ghost.cpp" -o "obj/players_ghost.o"

"obj/players_pacman.o":
	$(CC) $(CFLAGS) "src/players/pacman.cpp" -o "obj/players_pacman.o"

"obj/players_player.o":
	$(CC) $(CFLAGS) "src/players/player.cpp" -o "obj/players_player.o"

"obj/threads_continuous_thread.o":
	$(CC) $(CFLAGS) "src/threads/continuous_thread.cpp" -o "obj/threads_continuous_thread.o"

"obj/threads_thread.o":
	$(CC) $(CFLAGS) "src/threads/thread.cpp" -o "obj/threads_thread.o"

"obj/threads_timer.o":
	$(CC) $(CFLAGS) "src/threads/timer.cpp" -o "obj/threads_timer.o"

"obj/utils.o":
	$(CC) $(CFLAGS) "src/utils.cpp" -o "obj/utils.o"

