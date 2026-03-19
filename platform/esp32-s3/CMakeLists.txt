file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE SOURCES_APP "../../app/*.cpp")
file(GLOB_RECURSE SOURCES_SHARED "../shared/*.cpp")

idf_component_register(SRCS ${SOURCES} ${SOURCES_APP} ${SOURCES_SHARED}
                    INCLUDE_DIRS "../shared"
					INCLUDE_DIRS "./"
					INCLUDE_DIRS "../../"
					INCLUDE_DIRS "../Arduino_GFX/src"
					INCLUDE_DIRS "../Arduino_GFX/src/canvas"
					REQUIRES  espressif__arduino-esp32 Arduino_GFX)
