project = CreateProject()

project:CreateBinary("test_pgsql"):AddDependencies(
    project:CreateDependency()
        :AddSourceFiles("*.cpp")
        :AddFlags("-Wall", "-Werror", "-Wextra")
        :AddStaticLibrary("..", "pgsql_client_static"))

return project
