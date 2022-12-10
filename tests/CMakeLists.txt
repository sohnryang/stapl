include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests lexer_test.cpp parser_test.cpp typechecker_test.cpp)
target_link_libraries(tests PRIVATE GTest::gtest_main PRIVATE Lexer PRIVATE Parser PRIVATE AST PRIVATE IRGen)
target_include_directories(tests PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_compile_options(tests PRIVATE "$<$<C_COMPILER_ID:Clang>:-fsanitize=address,leak>")
target_link_options(tests PRIVATE "$<$<C_COMPILER_ID:Clang>:-fsanitize=address,leak>")
include(GoogleTest)
gtest_discover_tests(tests)
