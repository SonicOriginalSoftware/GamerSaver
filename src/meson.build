moc_headers = [
]
sources = [
]

moc_files = qt.preprocess(
  moc_headers : moc_headers,
  moc_sources : sources,
  qresources: 'resources.qrc',
  dependencies: qt_deps
)

sources += [
  'OAuth/oauthloopbackserver.cpp',
  'OAuth/oauthnetaccess.cpp',
  'OAuth/oauth.cpp',
  'OAuth/Google/googleoauth.cpp',
  'gsoauth.cpp',
  'appmanifest.cpp',
  'game.cpp',
  'main.cpp',
  'mainwindow.cpp',
  moc_files,
]

GamerSaver = executable(
  'GamerSaver',
  include_directories: [],
  sources: sources,
  dependencies: [qt_deps],
  gui_app: true,
)

# Windows output shared libraries
if build_machine.system() == 'windows'
  open_ssl_path = find_program('openssl').path().to_lower().strip('openssl.exe')
  configure_file(
    input: open_ssl_path + 'libssl-1_1-x64.dll',
    output: 'libssl-1_1-x64.dll',
    copy: true,
  )
  configure_file(
    input: open_ssl_path + 'libcrypto-1_1-x64.dll',
    output: 'libcrypto-1_1-x64.dll',
    copy: true,
  )
  configure_file(
    input: open_ssl_path + 'openssl.exe',
    output: 'openssl.exe',
    copy: true,
  )
endif
