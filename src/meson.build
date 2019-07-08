moc_headers = [
  'oauth.h',
  'mainwindow.h',
]
qt_sources = [
  'mainwindow.cpp',
]
q_resources = [
  'resources.qrc',
]

moc_files = qt.preprocess(
  moc_headers : moc_headers,
  include_directories : qt_includes,
  moc_sources : qt_sources,
  qresources: q_resources,
  dependencies: qt_deps
)

sources = [
  moc_files,
  'oauth.cpp',
  'game.cpp',
  'main.cpp',
  qt_sources
]

executable(
  'GamerSaver',
  include_directories: [],
  sources: sources,
  dependencies: qt_deps
)

