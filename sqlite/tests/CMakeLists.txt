cmake_minimum_required(VERSION 3.10)
project(sqliteclient-test)

file(GLOB_RECURSE __SQLITECLIENT_TESTS_SRC__ *.cpp)
add_executable(test_sqliteclient ${__SQLITECLIENT_TESTS_SRC__})
unset(__SQLITECLIENT_TESTS_SRC__)

target_link_libraries(test_sqliteclient PRIVATE sqliteclient_static)

if(MSVC)
    target_compile_options(test_sqliteclient PRIVATE /W4)
else()
    target_compile_options(test_sqliteclient PRIVATE -Wall -Wextra -Werror)
endif()
