cmake_minimum_required(VERSION 3.1)

project(paint)

file(GLOB SRC "paint/*.cpp")

if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
endif(WIN32)

file(GLOB SRC "src/*.cpp")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
add_executable(paint ${SRC})
target_link_libraries(paint sfml-graphics sfml-audio)