# .clangd в корне проекта
CompileFlags:
  Compiler: "C:/Program Files/LLVM/bin/clang++.exe"
  Add: [
    "-std=c++23",
    "-IC:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.42.34433/include",
    "-IC:/Program Files/LLVM/lib/clang/21/include",
    "-IC:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/ucrt"
    #"-fmodules-cache-path=./.clangd_module_cache"
  ]
  Remove: [
    #"-fmodules-ts",
    #"-fbuiltin-module-map"
  ]

Diagnostics:
  ClangTidy: false
  UnusedIncludes: false

Index:
  Background: Skip