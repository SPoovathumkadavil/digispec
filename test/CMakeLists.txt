cmake_minimum_required(VERSION 3.16)

project(digispecTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

enable_testing()

# ---- Tests ----

file(GLOB_RECURSE digispec_test_sources "src/*.cpp")
add_executable(digispec_test ${digispec_test_sources})
target_link_libraries(digispec_test 
  PRIVATE
  GTest::gtest_main
  digispec::digispec)
target_compile_features(digispec_test PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(digispec_test)

# ---- add resource files ----

file(GLOB_RECURSE test_resources "resources/*")
file(COPY ${test_resources}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)

# ---- End-of-file commands ----

add_folders(Test)
