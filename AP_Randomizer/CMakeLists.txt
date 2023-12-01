cmake_minimum_required(VERSION 3.18)

set(TARGET AP_Randomizer)
project(${TARGET})

add_library(${TARGET} SHARED
"main.cpp"
"src/Client.cpp"
"src/Engine.cpp"
"src/GameData.cpp"
"src/Logger.cpp" 
"src/StringOps.cpp" 
"src/Timer.cpp" 
"src/UnrealConsole.cpp")

target_include_directories(${TARGET} PRIVATE "include")
target_include_directories(${TARGET} PRIVATE "dependencies/apclientpp")
target_include_directories(${TARGET} PRIVATE "dependencies/json/include")
target_include_directories(${TARGET} PRIVATE "dependencies/valijson/include")
target_include_directories(${TARGET} PRIVATE "dependencies/websocketpp")
target_include_directories(${TARGET} PRIVATE "dependencies/wswrap/include")
target_include_directories(${TARGET} PRIVATE "dependencies/asio/include")
target_include_directories(${TARGET} PRIVATE "dependencies/openssl/include")
target_link_libraries(${TARGET} PUBLIC UE4SS)
target_link_libraries(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openssl/lib/libcrypto.lib)
target_link_libraries(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openssl/lib/libssl.lib)

target_compile_features(${TARGET} PUBLIC cxx_std_20)

# Require Windows Vista at minimum (needed for apclient)
# Defining WIN32_LEAN_AND_MEAN because we use windows.h for some small key controls
target_compile_definitions(${TARGET} PRIVATE _WIN32_WINNT=0x0600)

# Make MSVC actually report the correct c++ standard
target_compile_options(${TARGET} PRIVATE /Zc:__cplusplus)