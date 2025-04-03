# This example comes from https://eliemichel.github.io/LearnWebGPU
cmake_minimum_required(VERSION 3.11.0...3.25.0)
project(WebGPU-RAII-Test VERSION 1.0.0 LANGUAGES C CXX)

include(FetchContent)

#################################################
# Utility functions
function(set_common_target_properties Target)
  # Use strict C++17 standard
  set_target_properties(${Target} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )

  # Treat warnings as errors
  set_target_properties(${Target} PROPERTIES COMPILE_WARNING_AS_ERROR ON)

  # Turn all warnings on
  if (MSVC)
    target_compile_options(${Target} PRIVATE /W4)
  else()
    target_compile_options(${Target} PRIVATE -Wall -Wextra -pedantic)
  endif()
endfunction()

#################################################
# Dependencies
FetchContent_Declare(
  webgpu
  URL https://github.com/eliemichel/WebGPU-distribution/archive/refs/tags/main-v0.3.0-alpha.zip
  URL_HASH MD5=ea1195dc6c7661c36aa13ea5b734b86e
)
FetchContent_MakeAvailable(webgpu)

#################################################
# Test target
add_executable(webgpu-raii-test)
set_common_target_properties(webgpu-raii-test)

target_sources(webgpu-raii-test PRIVATE
  test.cpp
)

# Add webgpu-raii.hpp to include dirs
target_include_directories(webgpu-raii-test PRIVATE ..)

# Link to WebGPU
target_link_libraries(webgpu-raii-test PRIVATE webgpu)

# Copy wgpu-native or Dawn dll/so/dylib next to the executable
target_copy_webgpu_binaries(webgpu-raii-test)
