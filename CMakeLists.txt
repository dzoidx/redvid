configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

cmake_minimum_required(VERSION 3.8)
project(redvid)

set(CMAKE_CXX_STANDARD 17)

add_library(redvid_core RESP/Encoder.cpp RESP/Encoder.h RESP/Decoder.h RESP/Decoder.cpp net/net.h net/net.cpp)

set(SOURCE_FILES main.cpp)
add_executable(redvid ${SOURCE_FILES})
target_link_libraries(redvid redvid_core)
target_link_libraries(redvid Ws2_32)

file(GLOB TEST_SRC_FILES test/*.cpp)
add_executable(redvid_test ${TEST_SRC_FILES})
target_link_libraries(redvid_test gtest_main redvid_core)
add_test(NAME redvid_test COMMAND example)