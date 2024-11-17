cmake_minimum_required(VERSION 3.14)
project(mysqlclient)

# ----- options ----- #

option(MYSQLCLIENT_BUILD_TESTS "build tests" ON)
option(MYSQLCLIENT_INSTALL "install headers and libs" ON)
option(MYSQLCLIENT_HOLD_DEPS "do not update existing deps" OFF)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ----- dependencies ----- #

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
if(MYSQLCLIENT_HOLD_DEPS)
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
endif()

if(NOT MYSQLCLIENT_DEPS_DIR)
    get_filename_component(MYSQLCLIENT_DEPS_DIR ${CMAKE_CURRENT_BINARY_DIR}/deps ABSOLUTE)
    message("-- `MYSQLCLIENT_DEPS_DIR` is not set. set `MYSQLCLIENT_DEPS_DIR` to `${MYSQLCLIENT_DEPS_DIR}`.")
endif()

# ----- targets ----- #

file(GLOB_RECURSE __MYSQLCLIENT_SRC__ ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(mysqlclient_static STATIC ${__MYSQLCLIENT_SRC__})
unset(__MYSQLCLIENT_SRC__)

target_include_directories(mysqlclient_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mysqlclient_static PUBLIC mysqlclient)

if(MSVC)
    target_compile_options(mysqlclient_static PRIVATE /W4)
else()
    target_compile_options(mysqlclient_static PRIVATE -Wall -Wextra -Werror)
endif()

# ----- installations ----- #

if(MYSQLCLIENT_INSTALL)
    install(DIRECTORY include DESTINATION .)
    install(TARGETS mysqlclient_static DESTINATION lib)
endif()

# ----- tests ----- #

if(MYSQLCLIENT_BUILD_TESTS)
    add_subdirectory(tests)
endif()
