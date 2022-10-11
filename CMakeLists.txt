cmake_minimum_required(VERSION 3.5)

project(residtest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(residtest main.cpp)
target_link_libraries(residtest libresid.a)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpulse -lpulse-simple" )

install(TARGETS residtest
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

