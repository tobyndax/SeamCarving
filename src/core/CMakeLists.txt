#include the export definitions
include_directories(${CMAKE_SOURCE_DIR}/../include)
#Include SDL2
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/sdl2-2.0.5/include)
#Include self 
include_directories(${CMAKE_SOURCE_DIR}/core)

#Add the output directory
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

file (GLOB SRC_FILES "*.cpp")
file (GLOB HDR_FILES "*.h")

message(STATUS ${CMAKE_CXX_STANDARD})

add_library(core SHARED ${SRC_FILES} ${HDR_FILES})

target_link_libraries(core debug sdl2main.lib)
target_link_libraries(core optimized sdl2main.lib)

target_link_libraries(core debug sdl2.lib)
target_link_libraries(core optimized sdl2.lib)


add_definitions(-DEXPMODULE)
