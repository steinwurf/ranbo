cmake_minimum_required(VERSION 3.10)
project(ranbo)

# Include common CMake settings
include("${STEINWURF_RESOLVE}/toolchains/common_settings.cmake")

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
  add_executable(bit_occurrences examples/bit_occurrences.cpp)
  target_compile_features(bit_occurrences PUBLIC cxx_std_11)

  add_executable(hello_world examples/hello_world.cpp)
  target_link_libraries(bit_occurrences ranbo)
  target_link_libraries(hello_world ranbo)

  if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
  # For Windows: Prevent overriding the parent project's compiler/linker
  # settings
  set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
  endif()

  # Google Test dependency
  add_subdirectory("${STEINWURF_RESOLVE}/gtest-source" gtest_main
    EXCLUDE_FROM_ALL)

  # Define tests
  file(GLOB_RECURSE ranbo_test_sources test/*.cpp)
  add_executable(ranbo_tests ${ranbo_test_sources})
  target_compile_features(ranbo_tests PUBLIC cxx_std_14)
  target_link_libraries(ranbo_tests steinwurf::ranbo
    gtest_main
)

  enable_testing()
  add_test(ranbo_tests ranbo_tests)

endif()
