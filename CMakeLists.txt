cmake_minimum_required(VERSION 3.10)
project(ranbo)

# Define library
add_library(ranbo INTERFACE)
target_include_directories(ranbo INTERFACE src/)
add_library(steinwurf::ranbo ALIAS ranbo)

# Install headers
install(
  DIRECTORY ./src/ranbo
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  FILES_MATCHING
  PATTERN *.h)

# Is top level project?
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  # Build executables
  target_compile_features(bit_occurrences PUBLIC cxx_std_11)
  add_executable(bit_occurrences examples/bit_occurrences.cpp)

  add_executable(hello_world examples/hello_world.cpp)
  target_link_libraries(bit_occurrences ranbo)
  target_link_libraries(hello_world ranbo)
endif()
