cmake_minimum_required(VERSION 3.12)
project(cs416_project CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
cmake_policy(SET CMP0074 NEW)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-m64 -Wall -O3")
# g++ on Ubuntu (used by Gradescope) is configured with position independent executable flag which
# causes link-time errors. Disable only in that setting.

OPTION(DEFINE_VERBOSE
  "Build the project using verbose code"
  OFF)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# CUDA Support
include(CheckLanguage)
check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
  enable_language(CUDA)
else()
  message(STATUS "Could not find the CUDA compiler and toolkit. If you have CUDA-compatible GPU install the CUDA compiler/toolkit.")
endif()

find_package(CBLAS)
if (NOT CBLAS_FOUND)
    message(FATAL_ERROR "Could not find CBLAS library. Make sure you have completed the 'Getting Started' instructions on Canvas. Exiting.")
endif()
include_directories(${CBLAS_INCLUDE_DIRS})

find_package(CUB)
if (NOT CUB_FOUND)
  message(FATAL_ERROR, "Could not CUB library. Make sure you have completed the 'Getting Started' instructions on Canvas. Exiting.")
endif()
include_directories(${CUB_INCLUDE_DIRS})

# Add current directory to include search path (so tests can find headers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Link threading library to all executables in this assignment (needed for C++ threads)
link_libraries(Threads::Threads)  

add_executable(aks-serial 
  aks-serial.cc
)
