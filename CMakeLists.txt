cmake_minimum_required(VERSION 3.7.2)
project(cs327)

set(CMAKE_CXX_STANDARD 14)

include_directories(BEFORE termbox/src)
include_directories(BEFORE termbox/include)

#add_executable(Homework3 HW/hw3/fib.c)
#add_executable(Homework4 HW/hw4/pascal.c)
#add_executable(Homework5 HW/hw5/count.c)
#add_executable(Homework6 HW/hw6/mark.c)
#add_executable(Homework7 HW/hw7/intstack.cpp)
#add_executable(Homework8 HW/hw8/myexpr.cpp)
#add_executable(Project1 Projects/Part1/Part1.c)
#add_executable(Project2 Projects/Part2/Part2.c Projects/Part2/Checkers.h)
#add_executable(Project3 Projects/Part3/Part3.c Projects/Part2/Checkers.h)
add_executable(Project4 Projects/Part4/Part4.c)
#add_executable(Testing Testing.c)

#target_link_libraries(Project4 ‎/Library/Developer/CommandLineTools/usr/lib/libtermbox.a)
target_link_libraries(Project4 /Users/kskrueger/Documents/Classes/CS327/Code/cs327/libtermbox.a)