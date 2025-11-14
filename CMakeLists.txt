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
        src/GameLogic/GameLogic.h
        src/GameLogic/MoveGeneration/MoveGeneration.cpp
        src/GameLogic/MoveGeneration/MoveGeneration.h
        src/GameLogic/PrecomputedData/PrecomputedData.cpp
        src/GameLogic/PrecomputedData/PrecomputedData.h
        src/GameLogic/BitboardUtility/BitboardUtility.h
        src/Bot/Algorithm/Algorithm.cpp
        src/Bot/Algorithm/Algorithm.h
        src/Board/Zobrist.cpp
        src/Board/Zobrist.h
        src/Bot/Algorithm/Stockfish/Stockfish.cpp
        src/Bot/Algorithm/Stockfish/Stockfish.h
        src/Bot/UCI/UCI.cpp
        src/Bot/UCI/UCI.h
        src/Bot/Bot.cpp)

add_compile_options("-g -O0")
target_link_libraries(Chess raylib)