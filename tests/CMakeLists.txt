include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1)
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(lexer_test lexer_test.cpp)
target_link_libraries(
  lexer_test
  PRIVATE GTest::gtest_main
  PRIVATE Lexer)
target_include_directories(lexer_test PRIVATE "${PROJECT_SOURCE_DIR}/include")
gtest_discover_tests(lexer_test)

add_executable(parser_test parser_test.cpp)
target_link_libraries(
  parser_test
  PRIVATE GTest::gtest_main
  PRIVATE Parser)
target_include_directories(parser_test PRIVATE "${PROJECT_SOURCE_DIR}/include")
gtest_discover_tests(parser_test)

add_executable(annotator_test annotator_test.cpp)
target_link_libraries(
  annotator_test
  PRIVATE GTest::gtest_main
  PRIVATE TypeChecker)
target_include_directories(annotator_test
                           PRIVATE "${PROJECT_SOURCE_DIR}/include")
gtest_discover_tests(annotator_test)
