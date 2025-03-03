cmake_minimum_required(VERSION 3.28)
project(SQLite3Static)

include(FetchContent)

set(SQLITE_SOURCE_DIR "${CMAKE_BINARY_DIR}/sqlite3_src")

execute_process(
  COMMAND bash -c "curl -s https://www.sqlite.org/download.html | grep -oP 'sqlite-amalgamation-\\K[0-9]+' | head -1"
  OUTPUT_VARIABLE SQLITE_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (SQLITE_VERSION STREQUAL "" AND NOT EXISTS ${CMAKE_BINARY_DIR}/sqlite3_src)
  message(FATAL_ERROR "Failed to fetch the latest SQLite3 version. Check your internet connection.")
endif()

function(FetchSqlite VERSION)
  set(SQLITE_URL "https://www.sqlite.org/2025/sqlite-amalgamation-${VERSION}.zip")

  message(STATUS "Downloading SQLite3...")
  execute_process(
      COMMAND wget --quiet ${SQLITE_URL}
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
      RESULT_VARIABLE EXTRACT_RESULT
  )

  message(STATUS "Extracting SQLite3...")
  execute_process(
    COMMAND unzip sqlite-amalgamation-${VERSION}.zip -d sqlite3_src
    OUTPUT_QUIET
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    RESULT_VARIABLE EXTRACT_RESULT
  )
   
  if (NOT EXTRACT_RESULT EQUAL 0)
      message(FATAL_ERROR "Failed to extract SQLite3")
  endif()

  message(STATUS "SQLite3 extracted successfully!")
  execute_process(
    COMMAND rm ${CMAKE_BINARY_DIR}/sqlite-amalgamation-${VERSION}.zip
    OUTPUT_QUIET
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )

endfunction()

if(NOT EXISTS ${CMAKE_BINARY_DIR}/sqlite3_src)
  FetchSqlite(${SQLITE_VERSION})
endif()


file(GLOB_RECURSE SQLITE_SOURCE_FILES "${SQLITE_SOURCE_DIR}/*.c")

add_library(sqlite3 STATIC ${SQLITE_SOURCE_FILES})
target_include_directories(sqlite3 PUBLIC "${SQLITE_SOURCE_DIR}/sqlite-amalgamation-${SQLITE_VERSION}")

target_compile_definitions(sqlite3 PUBLIC SQLITE_OMIT_LOAD_EXTENSION)
set_target_properties(sqlite3 PROPERTIES C_STANDARD 99)

