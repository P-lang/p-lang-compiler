cmake_minimum_required(VERSION 3.8)
project(PLangCompiler)

set(CMAKE_CXX_STANDARD 17)

set(GCC_COVERAGE_COMPILE_FLAGS "-static-libgcc -static-libstdc++")
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

set(SOURCE_FILES main.cpp CommonTypes.h Lexer.cpp Lexer.h Parser.cpp Parser.h)
add_executable(PLangCompiler ${SOURCE_FILES})