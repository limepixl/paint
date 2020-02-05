cmake_minimum_required(VERSION 3.1)

project(paint)

file(GLOB SRC "paint/*.cpp")

if(MSVC)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /W4")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall")
endif()

file(GLOB SRC "src/*.cpp")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
add_executable(paint ${SRC})
target_link_libraries(paint sfml-graphics sfml-audio)