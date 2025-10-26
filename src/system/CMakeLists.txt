create_static_lib("system" "SignalManager.cpp")
target_include_directories(system PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(system PUBLIC event logger types)
