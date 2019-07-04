project('GamerSaver', 'cpp', version: '0.2.0')

deps = []
inc_dirs = []
sources = []

src_dir = ''

# Local includes
#inc_dirs += include_directories('../include')

# Qt dependency
qt5 = import('qt5')
qt5_widgets = dependency('Qt5Widgets')
qt5_includes = [
    '/usr/include/qt/',
    '/usr/include/qt/QtWidgets',
]
qt5_headers = [
    src_dir + 'mainwindow.h',
    src_dir + 'networkmanager.h'
]
qt5_sources = [
    src_dir + 'mainwindow.cpp',
    #'src/networkmanager.cpp',
]
qt5_deps = [qt5_widgets]
moc_files = qt5.preprocess(
    include_directories : [qt5_includes],
    moc_headers : [qt5_headers],
    moc_sources : [src_dir + 'mainwindow.cpp'],
)
sources += [
    moc_files
]
deps += qt5_deps

# Final sources list
sources += [
    src_dir + 'game.cpp',
    src_dir + 'main.cpp',
    qt5_sources
]

executable(
    'GamerSaver',
    include_directories: inc_dirs,
    sources: sources,
    dependencies: deps
)
