src_dir = '../src/'

moc_headers = [
]
sources = [
]
q_resources = [
  src_dir + 'resources.qrc',
]

moc_files = qt.preprocess(
  moc_headers : moc_headers,
  moc_sources : sources,
  qresources: q_resources,
  dependencies: qt_deps
)

sources += [
  moc_files,
  src_dir + 'oauth.cpp',
  'oauthtest.cpp',
]

OAuthNegativeTest = executable(
  'OAuthNegativeTest',
  include_directories: [src_dir],
  sources: sources,
  dependencies: qt_deps,
  build_by_default: false,
  cpp_args: ['-DSHOULD_FAIL=true'],
)

OAuthTest = executable(
  'OAuthTest',
  include_directories: [src_dir],
  sources: sources,
  dependencies: qt_deps,
  build_by_default: false,
)

test(
  'OAuthNegativeTest',
  OAuthNegativeTest,
  should_fail: true
)

test(
  'OAuthTest',
  OAuthTest,
)
