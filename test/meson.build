src_dir = '../src/'

moc_headers = [
  src_dir + 'oauth.h',
]
qt_sources = [
]
q_resources = [
  src_dir + 'resources.qrc',
]

moc_files = qt.preprocess(
  moc_headers : moc_headers,
  include_directories : qt_includes,
  moc_sources : qt_sources,
  qresources: q_resources,
  dependencies: qt_deps
)

oauth_sources = [
  moc_files,
  src_dir + 'oauth.cpp',
  qt_sources,
  'oauthtest.cpp',
]

oauthNegativeTest = executable(
  'OAuthNegativeTest',
  include_directories: [src_dir],
  sources: oauth_sources,
  dependencies: qt_deps,
  build_by_default: false,
  cpp_args: ['-DSHOULD_FAIL=true'],
)

oauthTest = executable(
  'OAuthTest',
  include_directories: [src_dir],
  sources: oauth_sources,
  dependencies: qt_deps,
  build_by_default: false,
)

test(
  'OAuthNegativeTest',
  oauthNegativeTest,
  should_fail: true
)

test(
  'OAuthTest',
  oauthTest,
)
