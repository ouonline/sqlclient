cmake_minimum_required(VERSION 3.14)
project(sqliteclient)

# ----- options ----- #

option(SQLITECLIENT_BUILD_TESTS "build tests" ON)
option(SQLITECLIENT_INSTALL "install headers and libs" ON)
option(SQLITECLIENT_HOLD_DEPS "do not update existing deps" OFF)

if(NOT SQLITECLIENT_DEP_SQLITE_INCS)
    message(FATAL_ERROR "`SQLITECLIENT_DEP_SQLITE_INCS` is not set.")
endif()
if(NOT SQLITECLIENT_DEP_SQLITE_LIBS)
    message(FATAL_ERROR "`SQLITECLIENT_DEP_SQLITE_LIBS` is not set.")
endif()

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ----- dependencies ----- #

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
if(SQLITECLIENT_HOLD_DEPS)
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
endif()

if(NOT SQLITECLIENT_DEPS_DIR)
    get_filename_component(SQLITECLIENT_DEPS_DIR ${CMAKE_CURRENT_BINARY_DIR}/deps ABSOLUTE)
    message("-- `SQLITECLIENT_DEPS_DIR` is not set. set `SQLITECLIENT_DEPS_DIR` to `${SQLITECLIENT_DEPS_DIR}`.")
endif()

# ----- targets ----- #

file(GLOB_RECURSE __SQLITECLIENT_SRC__ ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(sqliteclient_static STATIC ${__SQLITECLIENT_SRC__})
unset(__SQLITECLIENT_SRC__)

target_include_directories(sqliteclient_static PUBLIC
    ${SQLITECLIENT_DEP_SQLITE_INCS}
    ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(sqliteclient_static PUBLIC ${SQLITECLIENT_DEP_SQLITE_LIBS})

if(MSVC)
    target_compile_options(sqliteclient_static PRIVATE /W4)
else()
    target_compile_options(sqliteclient_static PRIVATE -Wall -Wextra -Werror)
endif()

# ----- installations ----- #

if(SQLITECLIENT_INSTALL)
    install(DIRECTORY include DESTINATION .)
    install(TARGETS sqliteclient_static DESTINATION lib)
endif()

# ----- tests ----- #

if(SQLITECLIENT_BUILD_TESTS)
    add_subdirectory(tests)
endif()
