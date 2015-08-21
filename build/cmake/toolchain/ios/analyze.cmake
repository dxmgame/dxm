# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED POLLY_ANALYZE_CMAKE_)
  return()
else()
  set(POLLY_ANALYZE_CMAKE_ 1)
endif()

set(
    POLLY_TOOLCHAIN_NAME
    "Clang static analyzer / c++11 support"
)
set(POLLY_TOOLCHAIN_TAG "analyze")

include("${CMAKE_CURRENT_LIST_DIR}/utilities/polly_common.cmake")

include("${CMAKE_CURRENT_LIST_DIR}/flags/cxx11.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/flags/disable_warnings.cmake")

set(CMAKE_CXX_COMPILER "${CMAKE_CURRENT_LIST_DIR}/scripts/clang-analyze.sh")
