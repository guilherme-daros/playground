create_static_lib("database" 
  ${CMAKE_CURRENT_LIST_DIR}/database/Handler.cpp
  )

target_link_libraries(database 
  sb::types
  sb::logger
  sqlite3)

