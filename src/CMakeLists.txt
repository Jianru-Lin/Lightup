cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project (Lightup)
add_definitions(-DUNICODE -D_UNICODE)
include_directories(../include)

#test Path
add_executable (Test-Path Path.cpp ../test/Test-Path.cpp)

# test FileSystem
add_executable (Test-FileSystem FileSystem.cpp ../test/Test-FileSystem.cpp Path.cpp)

# test Timer
add_executable (Test-Timer Timer.cpp ../test/Test-Timer.cpp)

# test Process
add_executable (Test-Process Process.cpp ../test/Test-Process.cpp)