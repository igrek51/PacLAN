cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME paclan)
set(WORKDIR /mnt/d/Igrek/C++/PacLAN/)

set(YCMD_DIR /mnt/d/Igrek/C++/ycmd/bin/ycmd)
set(SOURCE_FILES src/main.cpp src/animation.cpp src/app.cpp src/config.cpp src/game_cmd.cpp src/game_engine.cpp src/game_logic.cpp src/game_map.cpp src/game_network.cpp src/ghost.cpp src/graphics.cpp src/graphics_draw.cpp src/item.cpp src/language.cpp src/log.cpp src/map.cpp src/network.cpp src/network_task.cpp src/pacman.cpp src/pathfind.cpp src/player.cpp src/thread.cpp src/timer.cpp src/system.cpp src/continuous_thread.cpp)
set(SHIT_FILE /mnt/d/Igrek/C++/cmake/shit.cpp)

project(${PROJECT_NAME})



add_custom_target(ymake
		COMMAND ${YCMD_DIR} -w ${WORKDIR} --ymake)
add_custom_target(clean-all
        COMMAND ${YCMD_DIR} -w ${WORKDIR} --clean)
add_custom_target(really-clean
        COMMAND ${YCMD_DIR} -w ${WORKDIR} --clean)
add_custom_target(rebuild
		COMMAND ${YCMD_DIR} -w ${WORKDIR} --clean
        COMMAND ${YCMD_DIR} -w ${WORKDIR} --ymake)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} ymake)

#	SDL2
set(SDL2_INCLUDE_DIR /usr/include/SDL2)
include_directories(${SDL2_INCLUDE_DIR})