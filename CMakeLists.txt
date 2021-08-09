cmake_minimum_required(VERSION 3.1)

project(
    DUNGEON_BATTLE
    VERSION 1.0
    LANGUAGES C
)

find_package(SDL2 REQUIRED)

SET(SOURCES
    src/main.c
    src/game.c
    src/level.c
    src/renderer.c
    src/world.c
)

add_subdirectory(
    third_party/uthash
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE uthash)
