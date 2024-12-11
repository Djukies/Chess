cmake_minimum_required(VERSION 3.27)
project(Chess)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(libs/raylib)
include_directories(libs/raylib/src)

add_executable(Chess src/main.cpp
        src/Renderer/Renderer.cpp
        src/Renderer/Renderer.h
        src/Types.h
        src/Types.h
        src/Board/Board.cpp
        src/Board/Board.h
        src/Setup/Setup.cpp
        src/Setup/Setup.h
        src/Debug/Debug.cpp
        src/Debug/Debug.h
        src/GameLogic/GameLogic.cpp
        src/GameLogic/GameLogic.h)

add_compile_options("-g -O0")
target_link_libraries(Chess raylib)