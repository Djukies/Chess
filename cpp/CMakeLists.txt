cmake_minimum_required(VERSION 3.27)
project(Chess CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Debug flags
add_compile_options(-g -O0 -Wall -Wextra -Wpedantic)

# ==========================================
# Raylib (GUI only)
# ==========================================
add_subdirectory(libs/raylib)
include_directories(libs/raylib/src)

# ==========================================
# Helper: collect source files recursively
# ==========================================
file(GLOB_RECURSE COMMON_SRC       CONFIGURE_DEPENDS "src/Common/*.cpp")
file(GLOB_RECURSE COMMON_HDR       CONFIGURE_DEPENDS "src/Common/*.h")

file(GLOB_RECURSE ENGINE_SRC       CONFIGURE_DEPENDS "src/Engine/*.cpp")
file(GLOB_RECURSE ENGINE_HDR       CONFIGURE_DEPENDS "src/Engine/*.h")

file(GLOB_RECURSE GUI_SRC          CONFIGURE_DEPENDS "src/GUI/*.cpp")
file(GLOB_RECURSE GUI_HDR          CONFIGURE_DEPENDS "src/GUI/*.h")

# ==========================================
# ENGINE EXECUTABLE
# ==========================================
add_executable(chess_engine
        ${ENGINE_SRC}
        ${ENGINE_HDR}
        ${COMMON_SRC}
        ${COMMON_HDR}
#        ${GUI_SRC}
#        ${GUI_HDR}
)

target_include_directories(chess_engine PUBLIC
        src/
)

# No raylib here! Engine is fully independent.
target_compile_definitions(chess_engine PRIVATE ENGINE_BUILD=1)

# ==========================================
# GUI EXECUTABLE
# ==========================================
add_executable(chess_gui
        ${GUI_SRC}
        ${GUI_HDR}
        ${COMMON_SRC}
        ${COMMON_HDR}
#        ${ENGINE_SRC}
#        ${ENGINE_HDR}
)

target_include_directories(chess_gui PUBLIC
        src/
)

target_link_libraries(chess_gui raylib)
target_compile_definitions(chess_gui PRIVATE GUI_BUILD=1)