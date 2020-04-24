# Adapted from PyTorch: https://github.com/pytorch/pytorch/blob/master/cmake/Modules/FindCUB.cmake
# Try to find the CUB library and headers.
#  CUB_FOUND        - system has CUB
#  CUB_INCLUDE_DIRS - the CUB include directory

find_path(CUB_INCLUDE_DIRS
  NAMES cub/cub.cuh
  HINTS $ENV{CUB_ROOT}
  DOC "The directory where CUB includes reside"
)
mark_as_advanced(CUB_INCLUDE_DIRS)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CUB
  DEFAULT_MSG
  CUB_INCLUDE_DIRS
)