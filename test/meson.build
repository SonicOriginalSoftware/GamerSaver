src_dir = '../src/'

sources = [
  'gstest.cpp',
  src_dir + 'oauth.cpp',
  src_dir + 'googleoauth.cpp',
  src_dir + 'appmanifest.cpp',
  'OAuth/test_populategoogleendpoints.cpp',
]

test(
  'PopulateGoogleEndpoints',
  executable(
    'PopulateGoogleEndpoints',
    include_directories: [src_dir],
    sources: sources,
    dependencies: qt_deps,
    build_by_default: false,
  )
)

