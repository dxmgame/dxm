# Copyright (c) 2013, Ruslan Baratov
# All rights reserved.

if(DEFINED POLLY_LIBRARY_STD_LIBCXX_CMAKE)
  return()
else()
  set(POLLY_LIBRARY_STD_LIBCXX_CMAKE 1)
endif()

if(CMAKE_CXX_FLAGS)
    string(FIND ${CMAKE_CXX_FLAGS} "-stdlib=libc++" CXX_FLAGS_POS)
endif()
if((NOT CXX_FLAGS_POS) OR (CXX_FLAGS_POS LESS 0))
    set(
        CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -stdlib=libc++"
        CACHE
        STRING
        "C++ compiler flags"
        FORCE
    )
endif()

if(CMAKE_EXE_LINKER_FLAGS)
    string(FIND ${CMAKE_EXE_LINKER_FLAGS} "-stdlib=libc++ -ObjC" EXE_LINKER_FLAGS_POS)
endif()
if((NOT EXE_LINKER_FLAGS_POS) OR (EXE_LINKER_FLAGS_POS LESS 0))
    set(
        CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -ObjC"
        CACHE
        STRING
        "C++ linker flags"
        FORCE
    )
endif()

if(CMAKE_SHARED_LINKER_FLAGS)
    string(FIND ${CMAKE_SHARED_LINKER_FLAGS} "-stdlib=libc++" SHARED_LINKER_FLAGS_POS)
endif()
if((NOT SHARED_LINKER_FLAGS_POS) OR (SHARED_LINKER_FLAGS_POS LESS 0))
    set(
        CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++"
        CACHE
        STRING
        "Shared library linker flags"
        FORCE
    )
endif()
