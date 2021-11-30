project(nsu-os-labs)
cmake_minimum_required()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -std=c99")
set(MY_LIB_SOURCE labs-2/mylib/error_handling.c labs-2/mylib/mylib.c)

add_executable(lab24 labs-2/lab24/lab24.c labs-2/lab24/lab24_source.c labs-2/lab24/error_handling.c labs-2/lab24/mylib.c)
add_executable(test labs-2/mylib/test.c  ${MY_LIB_SOURCE})
