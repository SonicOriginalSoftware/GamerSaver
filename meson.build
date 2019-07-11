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

# TODO Use if statements and a check of the meson command flag
# to determine if the test targets should be built

# subdir('test')
subdir('src')
