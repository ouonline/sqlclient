cmake_minimum_required(VERSION 3.10)
project(mysqlclient-test)

file(GLOB_RECURSE __MYSQLCLIENT_TESTS_SRC__ *.cpp)
add_executable(test_mysqlclient ${__MYSQLCLIENT_TESTS_SRC__})
unset(__MYSQLCLIENT_TESTS_SRC__)

target_link_libraries(test_mysqlclient PRIVATE mysqlclient_static)

if(MSVC)
    target_compile_options(test_mysqlclient PRIVATE /W4)
else()
    target_compile_options(test_mysqlclient PRIVATE -Wall -Wextra -Werror)
endif()
