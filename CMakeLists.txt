project(nsu-os-labs)
cmake_minimum_required()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -std=c99")

add_executable(lab24 labs-2/lab24/lab24.c labs-2/lab24/lab24_source.c labs-2/lab24/lab24_source_threads_tasks.c labs-2/lab24/error_handling.c labs-2/lab24/mylib.c)
add_executable(test labs-2/test/test.c labs-2/test/error_handling.c labs-2/test/mylib.c)
