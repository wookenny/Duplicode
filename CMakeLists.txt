cmake_minimum_required(VERSION 2.6)

#project name
project(duplicode)

#specific compiler settings
set(CMAKE_CXX_FLAGS "-O3 -m64 -g -fstack-protector -funwind-tables -fasynchronous-unwind-tables -std=c++11 -I.")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS " -lboost_regex -lboost_iostreams -lboost_filesystem -lboost_system -lboost_program_options -l:libmagic.so.1  -L. -L/usr/lib64")

# collect all sources (copy from 'ls -1 *.cpp')
SET(DuplicodeSources
src/gnuplot-iostream.h
src/difflibTG.hpp
src/FilterIdentity.hpp
src/FilterToLower.hpp
src/FilterDelete.hpp
src/FilterDeleteComments.h
src/FilterDeleteComments.cpp
src/FilterDeleteWhitespace.hpp
src/FilterFactory.h
src/FilterFactory.cpp
src/magic.h
src/Main.cpp
src/Common.h
src/Common.cpp
src/CompareAlgo.h
src/CompareAlgo.cpp
src/ComparisonMatrix.h
src/ComparisonMatrix.cpp
src/AbstractTestComparator.h 
src/AbstractTestFilter.h 
src/ComparatorDiffLib.hpp
src/ComparatorIdentical.hpp
src/ComparatorLevenshteinDistance.h
src/ComparatorLevenshteinDistance.cpp
src/ComparatorLongestMatch.h
src/ComparatorLongestMatch.cpp
src/ComparatorFactory.h
src/ComparatorFactory.cpp
)


# display status message for important variables
MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "building with flags: " )
MESSAGE( STATUS "${CMAKE_CXX_FLAGS}" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS )

#define executable
ADD_EXECUTABLE(duplicode ${DuplicodeSources} )
