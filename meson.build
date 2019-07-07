project(
  'GamerSaver',
  'cpp',
  version: '0.2.0',
  default_options: [
    'buildtype=debugoptimized',
    'cpp_std=c++17'
  ],
)

qt_includes = [
  '/usr/include/qt/',
  '/usr/include/qt/QtWidgets',
  '/usr/include/qt/QtNetwork/QNetworkAccessManager',
]

qt = import('qt5')
qt_deps = dependency('qt5', modules: ['Widgets', 'Network'])

subdir('test')
subdir('src')

