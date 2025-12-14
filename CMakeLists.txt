cmake_minimum_required(VERSION 3.10)
project(ArduinoCanvasSDL)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
  

add_executable(arduino_canvas_sdl
    
    platform_shared/net_interface.cpp
    "app/ui_controller.cpp"
    "app/ui_widget.cpp"
    "app/ui_media_player_widget.cpp"
    "app/ui_settings_widget.cpp"
    "app/ui_numeric_editor.cpp"
    "platform_pc/timer.cpp"

    platform_pc/main.cpp
    platform_pc/wifi_serial.cpp
    platform_pc/net_wifi_serial_interface.cpp

    platform_pc/Arduino_GFX_SDL/src/arduino_gfx.cpp
    platform_pc/Arduino_GFX_SDL/src/arduino_g.cpp
    platform_pc/Arduino_GFX_SDL/src/canvas/arduino_canvas.cpp    
     "platform_shared/platform.h" "platform_pc/platform_pc.cpp"  "platform_pc/platform_pc.h")

include_directories(./)
include_directories(./platform_shared)
include_directories(./platform_pc)
include_directories(./platform_pc/Arduino_GFX_SDL/src/canvas)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0") # Для MSVC


include_directories(${asio_INCLUDE_DIRS})                              


target_compile_definitions(arduino_canvas_sdl PRIVATE _WIN32_WINNT=0x0601)

target_link_libraries(arduino_canvas_sdl
    SDL2::SDL2
    SDL2::SDL2main
    asio::asio
)


