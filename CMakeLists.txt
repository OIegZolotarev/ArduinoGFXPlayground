cmake_minimum_required(VERSION 3.10)
project(ArduinoCanvasSDL)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
  

add_executable(arduino_canvas_sdl
    main.cpp
    net_interface.cpp
    ui_controller.cpp
    ui_widget.cpp
    ui_media_player_widget.cpp
    ui_settings_widget.cpp
    ui_numeric_editor.cpp
    timer.cpp
    wifi_serial.cpp
    net_wifi_serial_interface.cpp


    Arduino_GFX_SDL/src/arduino_gfx.cpp
    Arduino_GFX_SDL/src/arduino_g.cpp
    Arduino_GFX_SDL/src/canvas/arduino_canvas.cpp    
    )

include_directories(./)
include_directories(./Arduino_GFX_SDL/src/canvas)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0") # Для MSVC


include_directories(${asio_INCLUDE_DIRS})                              


target_compile_definitions(arduino_canvas_sdl PRIVATE _WIN32_WINNT=0x0601)

target_link_libraries(arduino_canvas_sdl
    SDL2::SDL2
    SDL2::SDL2main
    asio::asio
)


