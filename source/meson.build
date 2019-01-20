# Local includes
inc_dirs += include_directories('include')

# Qt dependency
qt5 = import('qt5')
qt5_widgets = dependency('Qt5Widgets')
qt5_includes = [
    '/usr/include/qt/',
    '/usr/include/qt/QtWidgets',
]
qt5_headers = [
    'src/mainwindow.h',
    'src/networkmanager.h'
]
qt5_sources = [
    'src/mainwindow.cpp',
    #'src/networkmanager.cpp',
]
qt5_deps = [qt5_widgets]
moc_files = qt5.preprocess(
    include_directories : [qt5_includes],
    moc_headers : [qt5_headers],
    moc_sources : ['src/mainwindow.cpp'],
)
sources += [
    moc_files
]
deps += qt5_deps

# Final sources list
sources += [
    'src/game.cpp',
    'src/main.cpp',
    qt5_sources
]

executable(
    'GamerSaver',
    include_directories: inc_dirs,
    sources: sources,
    dependencies: deps
)
