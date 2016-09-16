mkdir obj
mkdir bin
g++ -c -o "obj/graphics_animations_animation.o" "src/graphics/animations/animation.cpp" -Wall -std=c++11
g++ -c -o "obj/graphics_animations_death_animation.o" "src/graphics/animations/death_animation.cpp" -Wall -std=c++11
g++ -c -o "obj/graphics_graphics.o" "src/graphics/graphics.cpp" -Wall -std=c++11
g++ -c -o "obj/graphics_graphics_draw.o" "src/graphics/graphics_draw.cpp" -Wall -std=c++11
g++ -c -o "obj/app.o" "src/app.cpp" -Wall -std=c++11
g++ -c -o "obj/config.o" "src/config.cpp" -Wall -std=c++11
g++ -c -o "obj/language.o" "src/language.cpp" -Wall -std=c++11
g++ -c -o "obj/log_log.o" "src/log/log.cpp" -Wall -std=c++11
g++ -c -o "obj/logic_game_cmd.o" "src/logic/game_cmd.cpp" -Wall -std=c++11
g++ -c -o "obj/logic_game_engine.o" "src/logic/game_engine.cpp" -Wall -std=c++11
g++ -c -o "obj/logic_game_logic.o" "src/logic/game_logic.cpp" -Wall -std=c++11
g++ -c -o "obj/logic_game_map.o" "src/logic/game_map.cpp" -Wall -std=c++11
g++ -c -o "obj/main.o" "src/main.cpp" -Wall -std=c++11
g++ -c -o "obj/map_item.o" "src/map/item.cpp" -Wall -std=c++11
g++ -c -o "obj/map_map.o" "src/map/map.cpp" -Wall -std=c++11
g++ -c -o "obj/map_pathfind_node.o" "src/map/pathfind/node.cpp" -Wall -std=c++11
g++ -c -o "obj/map_pathfind_path.o" "src/map/pathfind/path.cpp" -Wall -std=c++11
g++ -c -o "obj/map_pathfind_pathfind.o" "src/map/pathfind/pathfind.cpp" -Wall -std=c++11
g++ -c -o "obj/network_game_network.o" "src/network/game_network.cpp" -Wall -std=c++11
g++ -c -o "obj/network_network.o" "src/network/network.cpp" -Wall -std=c++11
g++ -c -o "obj/network_network_connection.o" "src/network/network_connection.cpp" -Wall -std=c++11
g++ -c -o "obj/network_network_task.o" "src/network/network_task.cpp" -Wall -std=c++11
g++ -c -o "obj/players_ghost.o" "src/players/ghost.cpp" -Wall -std=c++11
g++ -c -o "obj/players_pacman.o" "src/players/pacman.cpp" -Wall -std=c++11
g++ -c -o "obj/players_player.o" "src/players/player.cpp" -Wall -std=c++11
g++ -c -o "obj/threads_continuous_thread.o" "src/threads/continuous_thread.cpp" -Wall -std=c++11
g++ -c -o "obj/threads_thread.o" "src/threads/thread.cpp" -Wall -std=c++11
g++ -c -o "obj/threads_timer.o" "src/threads/timer.cpp" -Wall -std=c++11
g++ -c -o "obj/utils.o" "src/utils.cpp" -Wall -std=c++11
g++ -o "bin/paclan" "obj/graphics_animations_animation.o" "obj/graphics_animations_death_animation.o" "obj/graphics_graphics.o" "obj/graphics_graphics_draw.o" "obj/app.o" "obj/config.o" "obj/language.o" "obj/log_log.o" "obj/logic_game_cmd.o" "obj/logic_game_engine.o" "obj/logic_game_logic.o" "obj/logic_game_map.o" "obj/main.o" "obj/map_item.o" "obj/map_map.o" "obj/map_pathfind_node.o" "obj/map_pathfind_path.o" "obj/map_pathfind_pathfind.o" "obj/network_game_network.o" "obj/network_network.o" "obj/network_network_connection.o" "obj/network_network_task.o" "obj/players_ghost.o" "obj/players_pacman.o" "obj/players_player.o" "obj/threads_continuous_thread.o" "obj/threads_thread.o" "obj/threads_timer.o" "obj/utils.o" -lSDL2 -lSDL2_ttf -lSDL2_image -lboost_system -lboost_thread -Wall -s
