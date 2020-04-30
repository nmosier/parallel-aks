cmake_minimum_required(VERSION 3.12)
project(cs416_project CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{HOME})
cmake_policy(SET CMP0074 NEW)

link_directories(SYSTEM "$ENV{HOME}/lib")
include_directories(SYSTEM "$ENV{HOME}/include")
# include_directories(${PWD})

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-m64 -Wall -O3")
# g++ on Ubuntu (used by Gradescope) is configured with position independent executable flag which
# causes link-time errors. Disable only in that setting.

find_package(PkgConfig REQUIRED)


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

find_package(CBLAS REQUIRED)
include_directories(${CBLAS_INCLUDE_DIRS})

find_package(CUB REQUIRED)
include_directories(${CUB_INCLUDE_DIRS})

# GMP Support
# find_package(GMP REQUIRED)
pkg_check_modules(GMP REQUIRED gmp gmpxx)
include_directories(${GMP_INCLUDE_DIRS})


# MPFR Support
pkg_check_modules(MPFR REQUIRED mpfr)

# Add current directory to include search path (so tests can find headers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

link_libraries(Threads::Threads ${GMP_LIBRARIES} ${MPFR_LIBRARIES})

add_executable(aks-serial 
  aks-serial.cc
  aks-util.cc
)

add_executable(aks-test
  aks-test.cc
  aks-util.cc
)

add_executable(totient
  totient.cc
  aks-util.cc
)

add_executable(polynomial
  polynomial.cc
)

add_executable(bit_container
  bit_container.cc
)
