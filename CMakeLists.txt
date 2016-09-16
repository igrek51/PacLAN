cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME paclan)
set(WORKDIR /mnt/d/Igrek/C++/PacLAN/)

set(YCMD_DIR /mnt/d/Igrek/C++/ycmd/bin/ycmd)
set(SOURCE_FILES src/main.cpp src/graphics/animations/animation.cpp src/app.cpp src/config.cpp src/logic/game_cmd.cpp src/logic/game_engine.cpp src/logic/game_logic.cpp src/logic/game_map.cpp src/network/game_network.cpp src/players/ghost.cpp src/graphics/graphics.cpp src/graphics/graphics_draw.cpp src/map/item.cpp src/language.cpp src/log/log.cpp src/map/map.cpp src/network/network.cpp src/network/network_task.cpp src/players/pacman.cpp src/map/pathfind/pathfind.cpp src/players/player.cpp src/threads/thread.cpp src/threads/timer.cpp src/threads/continuous_thread.cpp src/graphics/animations/death_animation.cpp src/map/pathfind/node.cpp src/map/pathfind/node.h src/map/pathfind/path.cpp src/map/pathfind/path.h src/utils.cpp src/utils.h src/network/network_connection.cpp src/network/network_connection.h)
set(SHIT_FILE /mnt/d/Igrek/C++/cmake/shit.cpp)

project(${PROJECT_NAME})



add_custom_target(ymake
		COMMAND ${YCMD_DIR} -w ${WORKDIR} --ymake)
add_custom_target(clean-all
        COMMAND ${YCMD_DIR} -w ${WORKDIR} --clean)
add_custom_target(rebuild
		COMMAND ${YCMD_DIR} -w ${WORKDIR} --rebuild)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} ymake)

#	SDL2
set(SDL2_INCLUDE_DIR /usr/include/SDL2)
include_directories(${SDL2_INCLUDE_DIR})