cmake_minimum_required(VERSION 3.10)
project(pgsqlclient-test)

file(GLOB_RECURSE __PGSQLCLIENT_TESTS_SRC__ *.cpp)
add_executable(test_pgsqlclient ${__PGSQLCLIENT_TESTS_SRC__})
unset(__PGSQLCLIENT_TESTS_SRC__)

target_link_libraries(test_pgsqlclient PRIVATE pgsqlclient_static)

if(MSVC)
    target_compile_options(test_pgsqlclient PRIVATE /W4)
else()
    target_compile_options(test_pgsqlclient PRIVATE -Wall -Wextra -Werror)
endif()
