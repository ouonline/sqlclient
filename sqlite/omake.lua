project = Project()

project:CreateStaticLibrary("sqlite_client_static"):AddDependencies(
    project:CreateDependency()
        :AddSourceFiles("*.cpp")
        :AddFlags({"-Wall", "-Werror", "-Wextra"})
        :AddStaticLibraries("../../../../sqlite3", "sqlite3")
        :AddSysLibraries({"pthread", "dl"}))

return project
