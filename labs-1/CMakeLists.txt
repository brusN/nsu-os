cmake_minimum_required(VERSION 3.17)
project(labs-1 C)

set(CMAKE_C_STANDARD 99)

set(LAB2_SOURCE lab2_os/main.c)
set(LAB4_SOURCE lab4_os/main.c lab4_os/list.c)

add_executable(lab2_os ${LAB2_SOURCE})
add_executable(lab4_os ${LAB4_SOURCE})
