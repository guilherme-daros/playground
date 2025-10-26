include("cmake/sqlite3.cmake" PUBLIC)

create_static_lib("database" 
  ${CMAKE_CURRENT_LIST_DIR}/database/Handler.cpp
  )

add_subdirectory(database/tables)
add_subdirectory(types)
add_subdirectory(system)


target_link_libraries(database 
  sb::types
  sb::logger
  sqlite3)



