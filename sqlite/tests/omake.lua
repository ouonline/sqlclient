project = CreateProject()

project:CreateBinary("test_sqlite3"):AddDependencies(
    project:CreateDependency()
        :AddSourceFiles("*.cpp")
        :AddFlags("-Wall", "-Werror", "-Wextra")
        :AddStaticLibrary("..", "sqlite_client_static"))

return project
