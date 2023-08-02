include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1)
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests lexer_test.cpp parser_test.cpp annotator_test.cpp)
target_link_libraries(
  tests
  PRIVATE GTest::gtest_main
  PRIVATE Lexer
  PRIVATE Parser
  PRIVATE AST
  PRIVATE TypeChecker)
target_include_directories(tests PRIVATE "${PROJECT_SOURCE_DIR}/include")
include(GoogleTest)
gtest_discover_tests(tests)
