project = Project()

project:CreateStaticLibrary("mysql_client_static"):AddDependencies(
    project:CreateDependency()
        :AddSourceFiles("*.cpp")
        :AddFlags({"-Wall", "-Werror", "-Wextra"})
        :AddSysLibraries("mysqlclient"))

return project
