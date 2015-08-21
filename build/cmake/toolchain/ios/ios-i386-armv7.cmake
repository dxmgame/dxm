# Copyright (c) 2013-2014, Ruslan Baratov
# All rights reserved.

if(DEFINED POLLY_IOS_I386_ARMV7_CMAKE)
  return()
else()
  set(POLLY_IOS_I386_ARMV7_CMAKE 1)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/utilities/polly_clear_environment_variables.cmake")

set(
    POLLY_TOOLCHAIN_NAME
    "iOS Universal (iphoneos + iphonesimulator) / i386 / armv7 / c++11 support"
)
set(POLLY_TOOLCHAIN_TAG "ios-i386-armv7")

include("${CMAKE_CURRENT_LIST_DIR}/utilities/polly_common.cmake")

include(polly_fatal_error)

# Fix try_compile
set(MACOSX_BUNDLE_GUI_IDENTIFIER com.example)
set(CMAKE_MACOSX_BUNDLE YES)
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")

set(IPHONEOS_ARCHS armv7)
set(IPHONESIMULATOR_ARCHS i386)

# support for hunter (github.com/ruslo/hunter)
set(HUNTER_CMAKE_GENERATOR Xcode)

include("${CMAKE_CURRENT_LIST_DIR}/os/iphone.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/flags/cxx11.cmake")
