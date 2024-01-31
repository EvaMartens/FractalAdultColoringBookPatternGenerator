cmake_minimum_required(VERSION 3.26)
project(NCBV2 C)

set(CMAKE_C_STANDARD 17)

add_executable(NCBV2 main.c
        transformList.c
        transformList.h
        branchList.c
        branchList.h
        callStackList.h
        callStackList.c
)
