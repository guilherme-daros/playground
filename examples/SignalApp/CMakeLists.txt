cmake_minimum_required(VERSION 3.16)

project(SignalApp)

add_executable(SignalApp Main.cpp)

target_link_libraries(SignalApp PRIVATE event logger types system)
