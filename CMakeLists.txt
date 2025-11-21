cmake_minimum_required(VERSION 3.10)
project(ArduinoCanvasSDL)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 CONFIG REQUIRED)

add_executable(arduino_canvas_sdl
    main.cpp
    ui_controller.cpp
    timer.cpp
    Arduino_GFX_SDL/src/arduino_gfx.cpp
    Arduino_GFX_SDL/src/arduino_g.cpp
    Arduino_GFX_SDL/src/canvas/arduino_canvas.cpp
)

include_directories(./)
include_directories(./Arduino_GFX_SDL/src/canvas)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0") # Для MSVC

target_link_libraries(arduino_canvas_sdl
    SDL2::SDL2
    SDL2::SDL2main
)


