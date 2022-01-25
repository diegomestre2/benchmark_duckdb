# To use a different compiler with CMake, run it as follows: cmake -D
# CMAKE_C_COMPILER=/path/to/your/cc -D CMAKE_CXX_COMPILER=/path/to/your/c++
cmake_minimum_required(VERSION 3.1...3.6)

# -----------------------------------
# Project name, version & build type
# -----------------------------------
project(
  	CppProjectExample
  	DESCRIPTION "An example project with CMake"
  	LANGUAGES CXX)

# Forces Build type release----------------------------
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  	set(CMAKE_BUILD_TYPE
      	"Release"
      	CACHE STRING "" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Disables in-source builds to prevent source tree corruption.
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  	message(
    	FATAL_ERROR
      	"
		FATAL: In-source builds are not allowed.
       	You should create a separate directory for build files.
       	Ex:
           	mkdir build/debug
           	cd build/debug
           	cmake ..")
endif()

# Only do these if this is the main project, and not if it is included through
# add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  	# Optionally set things like CMAKE_CXX_STANDARD,
  	# CMAKE_POSITION_INDEPENDENT_CODE here

  	# Let's ensure -std=c++xx instead of -std=g++xx
  	set(CMAKE_CXX_EXTENSIONS OFF)

  	# Let's nicely support folders in IDE's
  	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  	# Testing only available if this is the main app Note this needs to be done in
  	# the main CMakeLists since it calls enable_testing, which must be in the main
  	# CMakeLists.
  	include(CTest)

  	# Docs only available if this is the main app
  	find_package(Doxygen)
  	if(Doxygen_FOUND)
    	add_subdirectory(docs)
  	else()
    	message(STATUS "Doxygen not found, not building docs")
  	endif()
endif()

# Accumulator library This is header only, so could be replaced with git
# submodules or FetchContent
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(CMAKE_VERBOSE_MAKEFILE OFF)

# ------------
# Dependencies
# ------------
# find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

# Fetch Dependencies
include(FetchContent)
message(STATUS "Fetching DuckDB...")
FetchContent_Declare(
  	duckdb
  	GIT_REPOSITORY https://github.com/duckdb/duckdb.git
  	GIT_TAG v0.3.1)
FetchContent_MakeAvailable(duckdb)
message("-- Duckdb Available")

# Find CCACHE (Speedup recompilation)
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

# ----------------------------
# General C/C++ build settings
# ----------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -O3 -Wall -Wpedantic -fPIC -ftest-coverage")
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Og -g -march=native -fsanitize=address -Woverloaded-virtual"
)
set(CMAKE_C_ARCHIVE_FINISH
    "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
set(CMAKE_CXX_ARCHIVE_FINISH
    "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# -------------------------------------------
# IDEs should put the headers in a nice place
# -------------------------------------------
source_group(
  	TREE "${PROJECT_SOURCE_DIR}/include"
  	PREFIX "Header Files"
  	FILES ${HEADER_LIST})

# ------------------
# Add and link_libraries
# ------------------
#include_directories(include) # include hpp files globally
set(DUCKDB_UTIL src/duckdb_util.cpp) # set alias for cpp files
add_library(duckdb_util_lib STATIC ${DUCKDB_UTIL}) # add cpp files to lib
target_link_libraries(duckdb_util_lib duckdb) # add dependencies to lib
target_include_directories( # Add Hpp files to scope
	duckdb_util_lib
  	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         	$<INSTALL_INTERFACE:include>
  	PRIVATE src)

# -----------------------
# Add sources for executor separately
# -----------------------
set(MAIN main/main.cpp) # set alias for cpp files
add_executable(main ${MAIN}) # add executor file
target_link_libraries(main duckdb_util_lib) # link all libraries to the target executor
