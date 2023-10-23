cmake_minimum_required(VERSION 3.18)

set(TARGET AP_Randomizer)
project(${TARGET})

add_library(${TARGET} SHARED "main.cpp" "src/Client.cpp" "src/Engine.cpp" "src/GameData.cpp" "src/Logger.cpp" "src/Timer.cpp" "src/UnrealConsole.cpp")
target_include_directories(${TARGET} PRIVATE "include")
target_include_directories(${TARGET} PRIVATE "dependencies/apcpp")
target_link_libraries(${TARGET} PUBLIC UE4SS)
target_link_libraries(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/apcpp/APCpp.lib)