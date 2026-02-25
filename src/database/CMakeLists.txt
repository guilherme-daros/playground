include("${CMAKE_SOURCE_DIR}/cmake/sqlite3.cmake" PUBLIC)

create_static_lib("database" 
  Handler.cpp
  )

add_subdirectory(tables)

target_link_libraries(database 
  PUBLIC
  sb::types
  sb::logger
  sqlite3)
