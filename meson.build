project(
  'GamerSaver',
  'cpp',
  version: '0.2.0',
  default_options: [
    'buildtype=debugoptimized',
    'cpp_std=c++17'
  ],
)

qt = import('qt5')
qt_deps = dependency('qt5', main: true, modules: ['Core', 'Gui', 'Widgets', 'Network'])

#subdir('test')
subdir('src')
