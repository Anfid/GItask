let g:ale_cpp_clang_options = '-std=c++11 -Wall -Iinclude'
let g:ale_cpp_clangtidy_options = '-Iinclude'
let g:ale_cpp_clangcheck_options = '-Iinclude'
let g:ale_linters = {'cpp': ['clang', 'cppcheck', 'clangtidy', 'clangcheck']}
let g:ale_cpp_cppcheck_options = '-j2 --enable=performance,portability,warning,style --inconclusive --project=build/compile_commands.json'
let g:ncm2_pyclang#library_path = '/usr/lib/llvm-6.0/lib/libclang.so.1'
