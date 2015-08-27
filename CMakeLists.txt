cmake_minimum_required(VERSION 3.2)
project(MiniGC)

include_directories(include)

add_executable(MiniGC ${SOURCE_FILES} main.c include/allocator.h src/allocator.c)