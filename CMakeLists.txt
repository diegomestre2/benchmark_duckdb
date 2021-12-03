# CMakeLists.txt for the Cpp_exercises project Notes: To use a different
# compiler with CMake, run it as follows: cmake -D
# CMAKE_C_COMPILER=/path/to/your/cc -D CMAKE_CXX_COMPILER=/path/to/your/c++
# your_project_dir
#
cmake_minimum_required(VERSION 3.1...3.6)

# My own cmake Modules set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
# "${CMAKE_SOURCE_DIR}/cmake/Modules/"
# "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

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
           cmake ..
")
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
# find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
# find_package(Catch2 REQUIRED) # Uncomment if Catch2 is installed
set(CMAKE_VERBOSE_MAKEFILE OFF)

# ----------------------------
# General C/C++ build settings
# ----------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -O3 -Wall -Wpedantic -fPIC -ftest-coverage -Wno-dev")
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Og -g -march=native -fsanitize=address -Woverloaded-virtual -Wno-dev"
)
set(CMAKE_C_ARCHIVE_FINISH
    "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
set(CMAKE_CXX_ARCHIVE_FINISH
    "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# Determine install paths
set(INSTALL_LIB_DIR
    lib
    CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR
    bin
    CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR
    include
    CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR
    ${DEF_INSTALL_CMAKE_DIR}
    CACHE PATH "Installation directory for CMake files")

find_package(duckdb QUIET)
include(FetchContent)
if(DUCKDB_FOUND)
  # do nothing
else(DUCKDB_FOUND)
  message(STATUS "Fetching DuckDB...")
  FetchContent_Declare(
    duckdb
    GIT_REPOSITORY https://github.com/duckdb/duckdb.git
    GIT_TAG v0.2.6)
  FetchContent_MakeAvailable(duckdb)
endif()

include_directories(${DUCKDB_INCLUDE_DIRS})
set(LIBS ${LIBS} ${DUCKDB_LIBRARIES})
message(STATUS "DUCKDB_FOUND: ${DUCKDB_FOUND}")
message(STATUS "DUCKDB_LIBRARIES: ${DUCKDB_LIBRARIES}")
message(STATUS "DUCKDB_INCLUDE_DIRS: ${DUCKDB_INCLUDE_DIRS}")

# ------------------
# Add link_libraries
# ------------------
add_library(sources src/duckdb_util.cpp)
target_include_directories(
  sources
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE src)

# -----------------------
# Add sources separately
# -----------------------
set(MAIN main/main.cpp)

# Add Cpp files
add_library(bench_library SHARED ${MAIN})

# Add Hpp files
target_include_directories(
  bench_library PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                       $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

include_directories(${PROJECT_SOURCE_DIR}/third_party)
include_directories(third_party/duckdb/src/include)
link_directories(third_party/duckdb/build/release/src)
add_executable(main main/main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE duckdb)
