project = Project()

project:CreateBinary("test_sqlite3"):AddDependencies(
    project:CreateDependency()
        :AddSourceFiles("*.cpp")
        :AddFlags({"-Wall", "-Werror", "-Wextra"})
        :AddStaticLibraries("..", "sqlite_client_static"))

return project
