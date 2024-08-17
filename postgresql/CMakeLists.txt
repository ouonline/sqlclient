cmake_minimum_required(VERSION 3.14)
project(pgsqlclient)

# ----- options ----- #

option(PGSQLCLIENT_BUILD_TESTS "build tests" ON)
option(PGSQLCLIENT_INSTALL "install headers and libs" ON)
option(PGSQLCLIENT_HOLD_DEPS "do not update existing deps" OFF)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ----- dependencies ----- #

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
if(PGSQLCLIENT_HOLD_DEPS)
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
endif()

if(NOT PGSQLCLIENT_DEPS_DIR)
    get_filename_component(PGSQLCLIENT_DEPS_DIR ${CMAKE_CURRENT_BINARY_DIR}/deps ABSOLUTE)
    message("-- `PGSQLCLIENT_DEPS_DIR` is not set. set `PGSQLCLIENT_DEPS_DIR` to `${PGSQLCLIENT_DEPS_DIR}`.")
endif()

# ----- targets ----- #

file(GLOB_RECURSE __PGSQLCLIENT_SRC__ ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(pgsqlclient_static STATIC ${__PGSQLCLIENT_SRC__})
unset(__PGSQLCLIENT_SRC__)

target_include_directories(pgsqlclient_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(pgsqlclient_static PUBLIC pq)

if(MSVC)
    target_compile_options(pgsqlclient_static PRIVATE /W4)
else()
    target_compile_options(pgsqlclient_static PRIVATE -Wall -Wextra -Werror)
endif()

# ----- installations ----- #

if(PGSQLCLIENT_INSTALL)
    install(DIRECTORY include DESTINATION .)
    install(TARGETS pgsqlclient_static DESTINATION lib)
endif()

# ----- tests ----- #

if(PGSQLCLIENT_BUILD_TESTS)
    add_subdirectory(tests)
endif()
