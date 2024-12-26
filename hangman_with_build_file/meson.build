project('hangman', 'cpp', default_options: [ 'cpp_std=c++23' ])

cpp_flags = [ '-O3', '-pipe' ]

src_files = files('main.cc', 'word.cc')

executable('hangman', src_files, cpp_args : cpp_flags)
