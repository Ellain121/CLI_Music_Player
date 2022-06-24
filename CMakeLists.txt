cmake_minimum_required(VERSION 3.0.2)
project(cli_music_player)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-ggdb)

set(PROJECT_LINK_LIBS libncurses.so.6 libbass.so)
link_directories(/lib/x86_64-gnu)
link_directories(/usr/local/lib)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(cli_music_player ${SOURCES})
target_link_libraries(cli_music_player ${PROJECT_LINK_LIBS})
