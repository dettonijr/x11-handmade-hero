cmake_minimum_required(VERSION 2.8)
project (X11Tests)

find_package(X11)

include_directories(${X11_INCLUDE_DIR})

add_library(X11Wrap X11Display.cpp X11Window.cpp Framebuffer.cpp Obj.cpp Color.cpp TGAFile.cpp)
add_executable(main main.cpp)
add_executable(life life.cpp)
add_executable(tiny tiny.cpp)
add_executable(tga tga.cpp)
add_definitions(-std=c++11 -g)
target_link_libraries(main X11Wrap ${X11_LIBRARIES})
target_link_libraries(life X11Wrap ${X11_LIBRARIES})
target_link_libraries(tiny X11Wrap ${X11_LIBRARIES})
target_link_libraries(tga X11Wrap ${X11_LIBRARIES})
