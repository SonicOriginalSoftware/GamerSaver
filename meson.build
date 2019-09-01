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

# Windows output shared libraries
#if build_machine.system() == 'windows'
  # OpenSSL required libraries
  #open_ssl_path = find_program('openssl').path().to_lower().strip('openssl.exe')
  #configure_file(
    #input: open_ssl_path + 'libssl-1_1-x64.dll',
    #output: 'libssl-1_1-x64.dll',
    #copy: true,
  #)
#endif

#subdir('test')
subdir('src')
