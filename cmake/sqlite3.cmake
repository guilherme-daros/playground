cmake_minimum_required(VERSION 3.28)
include(FetchContent)

# Use the latest 2026 version of SQLite amalgamation
set(SQLITE_VERSION "3510200")
set(SQLITE_URL "https://www.sqlite.org/2026/sqlite-amalgamation-${SQLITE_VERSION}.zip")

FetchContent_Declare(
  sqlite3_amalgamation
  URL ${SQLITE_URL}
)

FetchContent_MakeAvailable(sqlite3_amalgamation)

# Find the source files in the extracted directory
file(GLOB SQLITE_SOURCE_FILES "${sqlite3_amalgamation_SOURCE_DIR}/*.c")

# Create the sqlite3 static library
add_library(sqlite3 STATIC ${SQLITE_SOURCE_FILES})
target_include_directories(sqlite3 PUBLIC "${sqlite3_amalgamation_SOURCE_DIR}")

# Standard SQLite configurations
target_compile_definitions(sqlite3 PUBLIC SQLITE_OMIT_LOAD_EXTENSION)
set_target_properties(sqlite3 PROPERTIES C_STANDARD 99)

# Export as sb::sqlite3 for consistency if needed (though not required by workplan)
add_library(sb::sqlite3 ALIAS sqlite3)
