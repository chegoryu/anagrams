cmake_minimum_required(VERSION 3.1)
project(anagram CXX C)

# find system ICU

find_package(PkgConfig)
pkg_search_module(ICUUC REQUIRED icu-uc)

# General configuration

set(CMAKE_CXX_STANDARD_REQUIRED 11)
set(CMAKE_C_STANDARD_REQUIRED 11)

if (CMAKE_COMPILER_IS_GNUCXX)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fdiagnostics-color=always -ggdb3 -pedantic")
  	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -fdiagnostics-color=always -ggdb3 -pedantic")
else()
	message("[Warning]: You are building without gnu c++ compiler, you may need to tune some things")
endif()

# Build facil

add_subdirectory(extern/facil.io)

# Build us.

set (ANAGRAM_SRC src/runner.cpp
                 src/server.c)

include_directories(include)
include_directories(${ICUUC_INCLUDE_DIRS})               
add_executable(anagram ${ANAGRAM_SRC})
target_link_libraries(anagram facil.io ${ICUUC_LIBRARIES})
#add_custom_command(TARGET anagram PRE_BUILD COMMAND "make libdump" WORKING_DIRECTORY "extern/facil.io")
