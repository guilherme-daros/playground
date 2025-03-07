function(create_interface_lib DIRNAME)
  add_library(${DIRNAME} INTERFACE)
  target_include_directories(${DIRNAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
  add_library(sb::${DIRNAME} ALIAS ${DIRNAME})
endfunction()

function(create_static_lib DIRNAME SOURCES)
  add_library(${DIRNAME} STATIC ${SOURCES})
  target_include_directories(${DIRNAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
  add_library(sb::${DIRNAME} ALIAS ${DIRNAME})
endfunction()
