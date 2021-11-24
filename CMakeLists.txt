project(nsu-os-labs)
cmake_minimum_required()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -std=c99")
set(MY_LIB_SOURCE labs-2/mylib/error_handling.c labs-2/mylib/mylib.c)

add_executable(test labs-2/mylib/test.c  ${MY_LIB_SOURCE})
#add_executable(lab1 labs-2/lab1/lab1.c)
#add_executable(lab2 labs-2/lab2/lab2.c)
#add_executable(lab8 labs-2/lab8/lab8.c labs-2/lab8/lab8_source.c ${MY_LIB_SOURCE})
#add_executable(lab10 labs-2/lab10/lab10.c labs-2/lab10/error_handling.c labs-2/lab10/mylib.c labs-2/lab10/lab10_source.c)
add_executable(lab24 labs-2/lab24/lab24.c labs-2/lab24/lab24_source.c labs-2/lab24/error_handling.c)
