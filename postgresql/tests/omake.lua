project = Project()

project:CreateBinary("test_pgsql"):AddDependencies(
    project:CreateDependency()
        :AddSourceFiles("*.cpp")
        :AddFlags({"-Wall", "-Werror", "-Wextra"})
        :AddStaticLibraries("..", "pgsql_client_static"))

return project
