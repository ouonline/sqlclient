project = Project()

project:CreateStaticLibrary("pgsql_client_static"):AddDependencies(
    project:CreateDependency()
        :AddSourceFiles("*.cpp")
        :AddFlags({"-Wall", "-Werror", "-Wextra"})
        :AddSysLibraries("pq"))

return project
