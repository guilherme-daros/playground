function(create_lib DIRNAME)
  add_library(${DIRNAME} INTERFACE)
  target_include_directories(${DIRNAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
  add_library(sb::${DIRNAME} ALIAS ${DIRNAME})
endfunction()


