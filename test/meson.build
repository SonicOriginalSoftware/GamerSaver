moc_headers = [
]
qt_sources = [
]
q_resources = [
]

moc_files = qt.preprocess(
  moc_headers : moc_headers,
  include_directories : qt_includes,
  moc_sources : qt_sources,
  qresources: q_resources,
  dependencies: qt_deps
)

src_dir = '../src/'

oauth_sources = [
  moc_files,
  src_dir + 'oauth.cpp',
  qt_sources,
  'oauthtest.cpp',
]

oauthTest = executable(
  'OAuthTest',
  include_directories: [src_dir],
  sources: oauth_sources,
  dependencies: qt_deps
)

test(
  'OAuthNegativeTest',
  oauthTest,
  args: ['SHOULD_FAIL=true'],
  should_fail: true
)

test(
  'OAuthTest',
  oauthTest,
)
