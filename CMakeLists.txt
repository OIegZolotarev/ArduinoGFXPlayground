cmake_minimum_required(VERSION 3.16)


set(CMAKE_CXX_STANDARD 17)



if(ESP_PLATFORM)

    set(EXTRA_COMPONENT_DIRS "platform")
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(InstrumentClusterClient)
        

else()

    include_directories(./)
    include_directories(./platform/shared)
    include_directories(./platform/Arduino_GFX/src)

    project(InstrumentClusterClient)

    find_package(SDL2 CONFIG REQUIRED)
    find_package(asio CONFIG REQUIRED)
  

    add_executable(arduino_canvas_sdl
    
    platform/shared/net_interface.cpp
    "app/ui_controller.cpp"
    "app/ui_widget.cpp"
    "app/ui_keyboard.cpp"
    "app/ui_media_player_widget.cpp"
    "app/ui_settings_widget.cpp"
    "app/ui_wifi_network_selector.cpp"
    "app/ui_numeric_editor.cpp"
    "app/ui_tune_widget.cpp"
    "platform/shared/platform.h" 

    "platform/pc/timer.cpp"
    "platform/pc/main.cpp"
    "platform/pc/wifi_serial.cpp"
    "platform/pc/net_wifi_serial_interface.cpp"
    "platform/pc/Arduino_GFX_SDL/src/arduino_gfx.cpp"
    "platform/pc/Arduino_GFX_SDL/src/arduino_g.cpp"
    "platform/pc/Arduino_GFX_SDL/src/canvas/arduino_canvas.cpp"    
    "platform/pc/platform_pc.cpp"  
    "platform/pc/platform_pc.h" "app/ui_tune_widget.h")

    
    include_directories(./platform/pc)
    include_directories(./platform/pc/Arduino_GFX_SDL/src/canvas)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0") # Для MSVC


    include_directories(${asio_INCLUDE_DIRS})                              


    target_compile_definitions(arduino_canvas_sdl PRIVATE _WIN32_WINNT=0x0601)

    target_link_libraries(arduino_canvas_sdl
    SDL2::SDL2
    SDL2::SDL2main
    asio::asio
    )

endif()