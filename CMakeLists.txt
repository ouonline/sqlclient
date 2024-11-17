if(SQLCLIENT_ENABLE_MYSQL)
    add_subdirectory(mysql)
endif()

if(SQLCLIENT_ENABLE_POSTGRESQL)
    add_subdirectory(postgresql)
endif()

if(SQLCLIENT_ENABLE_SQLITE)
    add_subdirectory(sqlite)
endif()
