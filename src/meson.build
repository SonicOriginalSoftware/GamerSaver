moc_headers = [
  'mainwindow.h',
]
sources = [
  'mainwindow.cpp',
]
q_resources = [
  'resources.qrc',
]

moc_files = qt.preprocess(
  moc_headers : moc_headers,
  moc_sources : sources,
  qresources: q_resources,
  dependencies: qt_deps
)

sources += [
  moc_files,
  'oauth.cpp',
  'game.cpp',
  'main.cpp',
]

GamerSaver = executable(
  'GamerSaver',
  include_directories: [],
  sources: sources,
  dependencies: [qt_deps],
  gui_app: true,
)
