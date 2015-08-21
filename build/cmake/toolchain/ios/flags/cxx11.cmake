# Copyright (c) 2013, Ruslan Baratov
# All rights reserved.

if(DEFINED POLLY_FLAGS_CXX11_CMAKE)
  return()
else()
  set(POLLY_FLAGS_CXX11_CMAKE 1)
endif()

if(CMAKE_CXX_FLAGS)
	string(FIND ${CMAKE_CXX_FLAGS} "-std=c++11 -Wno-c++11-narrowing" CXX_FLAGS_POS)
endif()
if((NOT CXX_FLAGS_POS) OR (CXX_FLAGS_POS LESS 0))
	set(
    	CMAKE_CXX_FLAGS
    	"${CMAKE_CXX_FLAGS} -std=c++11 -Wno-c++11-narrowing"
    	CACHE
    	STRING
    	"C++ compiler flags"
    	FORCE
	)
endif()