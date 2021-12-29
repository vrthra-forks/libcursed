add_requires("ncursesw")

target("cursed")
    set_kind("static")
    set_languages("c++11")
    add_files("*.cpp")
    add_files("guts/*.cpp")
    add_packages("ncursesw")
    on_install(function() end)
