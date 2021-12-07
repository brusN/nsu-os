project(nsu-os-labs)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
set(CMAKE_C_COMPILER_VERSION 11)
set(CMAKE_C_STANDARD 11)

#add_executable(lab10 labs-2/lab10/lab10.c labs-2/lab10/lab10_source.c labs-2/lab10/error_handling.c)
add_executable(lab14 labs-2/lab14/lab14.c labs-2/lab14/lab14_source.c labs-2/lab14/error_handling.c labs-2/lab14/mylib.c)
add_executable(test labs-2/test/test.c)