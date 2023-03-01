SET(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
SET(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

find_package(Boost REQUIRED COMPONENTS program_options)

message(STATUS "Found Boost ${Boost_VERSION}")

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 9.1.0
)
FetchContent_MakeAvailable(fmt)

add_library(Lexer lexer.cpp)
target_include_directories(Lexer PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_library(AST ast.cpp ast_printer.cpp ast_comparator.cpp)
target_include_directories(AST PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(AST PUBLIC fmt::fmt)

add_library(Parser parser.cpp)
target_include_directories(Parser PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(Parser PUBLIC Lexer PUBLIC AST)

add_library(IRGen irgen.cpp typechecker.cpp)
target_include_directories(IRGen PUBLIC "${PROJECT_SOURCE_DIR}/include" PUBLIC "${LLVM_INCLUDE_DIRS}")
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries(IRGen PUBLIC AST PUBLIC ${llvm_libs})

add_executable(staplc staplc.cpp)
target_include_directories(staplc PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(staplc PRIVATE ${Boost_LIBRARIES} PRIVATE IRGen PRIVATE AST PRIVATE Parser)
