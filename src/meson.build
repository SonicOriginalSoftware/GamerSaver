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
  'oauthloopbackserver.cpp',
  'oauthnetaccess.cpp',
  'oauth.cpp',
  'googleoauth.cpp',
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
