#project config

cmake_minimum_required(VERSION 3.28)

project(bar)

include("third_party/fmtlib.cmake")

#set(CMAKE_UNITY_BUILD ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(WIN32)

else()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -fno-exceptions -fno-rtti -fomit-frame-pointer -Wall -Wextra -O0")

endif()


# build bar
add_library(bar "src/beyond_all_repair.cpp")
target_link_libraries(bar fmt::fmt)
target_include_directories(bar PUBLIC "src/headers")

# destoer lib
target_include_directories(bar PUBLIC "destoer-lib/src/")

# build command line tool
add_executable(beyond_all_repair "src/main.cpp")
target_link_libraries(beyond_all_repair bar)



