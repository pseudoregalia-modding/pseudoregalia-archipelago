cmake_minimum_required(VERSION 3.18)

set(TARGET AP_Randomizer)
project(${TARGET})

add_library(${TARGET} SHARED "main.cpp")
target_include_directories(${TARGET} PRIVATE .)
target_link_libraries(${TARGET} PUBLIC ue4ss)