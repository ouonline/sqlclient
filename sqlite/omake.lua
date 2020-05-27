project = CreateProject()

project:CreateStaticLibrary("sqlite_client_static"):AddDependencies(
    project:CreateDependency()
        :AddSourceFiles("*.cpp")
        :AddFlags("-Wall", "-Werror", "-Wextra")
        :AddStaticLibrary("../../../../sqlite3", "sqlite3")
        :AddSysLibraries("pthread", "dl"))

return project
