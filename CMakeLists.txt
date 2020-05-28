cmake_minimum_required(VERSION 3.16)
project(tpl2020 C)

set(CMAKE_C_STANDARD 11)

#Include all TPL files inside /examples folder
file(GLOB MY_FILES "*.tpl")
file(COPY ${MY_FILES}examples
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


add_executable(tpl2020 lexer.c runner.c parser.c parser.h globals.h semantic.c semantic.h)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
