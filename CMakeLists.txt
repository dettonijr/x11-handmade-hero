cmake_minimum_required(VERSION 3.0)
project (X11Tests)

find_package(X11)

include_directories(${X11_INCLUDE_DIR})

add_library(X11Wrap X11Display.cpp X11Window.cpp Framebuffer.cpp)
add_executable(main main.cpp)
target_link_libraries(main X11Wrap ${X11_LIBRARIES})
