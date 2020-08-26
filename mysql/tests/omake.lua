project = Project()

project:CreateBinary("test_mysql"):AddDependencies(
    project:CreateDependency()
        :AddSourceFiles("*.cpp")
        :AddFlags({"-Wall", "-Werror", "-Wextra"})
        :AddStaticLibraries("..", "mysql_client_static"))

return project
