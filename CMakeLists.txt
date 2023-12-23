# CMakeLists.txt

cmake_minimum_required(VERSION 3.12)

project(Chess)

# Add -Ofast flag for Debug build
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DtBUG} -Ofast")


add_executable(my_program main.cpp 
    bitboard.cpp attack_tables.cpp 
    randomNum.cpp bitgamestate.cpp
    time.cpp search.cpp)
