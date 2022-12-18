cmake_minimum_required(VERSION 3.19)
project(Stapl)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

if(${BUILD_DOCS})
  add_subdirectory(docs)
endif()
