moc_headers = [
  'oauth.h',
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
  dependencies: qt_deps,
  gui_app: true,
)

# Qt5 Output shared libraries
if build_machine.system() == 'windows'
  qt_bin_path = find_program('qmake').path().to_lower().strip('qmake.exe')

  configure_file(
    input: qt_bin_path + 'Qt5Core.dll',
    output: 'Qt5Core.dll',
    copy: true,
  )
  configure_file(
    input: qt_bin_path + 'Qt5Gui.dll',
    output: 'Qt5Gui.dll',
    copy: true,
  )
  configure_file(
    input: qt_bin_path + 'Qt5Widgets.dll',
    output: 'Qt5Widgets.dll',
    copy: true,
  )
  configure_file(
    input: qt_bin_path + 'Qt5Network.dll',
    output: 'Qt5Network.dll',
    copy: true,
  )
endif
