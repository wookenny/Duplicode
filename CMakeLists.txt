cmake_minimum_required(VERSION 2.6)

#project name
project(Duplicode)

#specific compiler settings
set(CMAKE_CXX_FLAGS "-m64 -fstack-protector -funwind-tables -fasynchronous-unwind-tables -std=c++11 -I.")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS " -pthread -L.")

# collect all sources (copy from 'ls -1 *.cpp')
SET(DuplicodeSources
FilterIdentity.hpp
FilterDelete.hpp
Main.cpp
CompareAlgo.h
CompareAlgo.cpp
AbstractTetComparator.h 
AbstractTetFilter.h 
ComparatorIdentical.hpp
)


# display status message for important variables
MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "building with flags: " )
MESSAGE( STATUS "${CMAKE_CXX_FLAGS}" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS )

#define executable
ADD_EXECUTABLE(Duplicode ${DuplicodeSources} )
