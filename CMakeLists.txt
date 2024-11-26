cmake_minimum_required(VERSION 3.13)
project(NullCheckAnalyzer)

find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(NullCheckAnalyzer NullCheckAnalyzer.cpp)
target_link_libraries(NullCheckAnalyzer
    clangTooling
    clangBasic
    clangASTMatchers
    clangAST
    clangFrontend
    clangSerialization
    clangEdit
    clangLex
)
