get_filename_component(executableName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" executableName ${executableName})

add_executable(${executableName} "Main.cpp")

target_link_libraries(${executableName} PUBLIC sb::types)
target_link_libraries(${executableName} PUBLIC sb::event)
