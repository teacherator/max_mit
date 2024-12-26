project('wordle', 'cpp', default_options: [ 'cpp_std=c++23' ])

cpp_flags = [ '-O3', '-pipe' ]

src_files = files('main.cc', 'word.cc', 'secret_word.cc')

executable('wordle', src_files, cpp_args : cpp_flags)
