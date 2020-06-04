project = CreateProject()

project:CreateBinary("test_mysql"):AddDependencies(
    project:CreateDependency()
        :AddSourceFiles("*.cpp")
        :AddFlags("-Wall", "-Werror", "-Wextra")
        :AddStaticLibrary("..", "mysql_client_static"))

return project
