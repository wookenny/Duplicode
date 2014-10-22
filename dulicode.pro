SOURCE        = src/
HEADERS       = src/AbstractTestComparator.h \
                src/AbstractTestFilter.h \
                src/Common.h \
                src/ComparatorFactory.h \
                src/ComparatorLevenshteinDistance.h \
                src/ComparatorLongestMatch.h \
                src/CompareAlgo.h \
                src/ComparisonMatrix.h \
                src/difflib.h \
                src/FilterDeleteComments.h \
                src/FilterFactory.h \
                src/gnuplot-iostream.h \
                src/magic.h \
                src/renderarea.h \
                src/window.h \
                src/ComparatorDiffLib.hpp \
                src/ComparatorIdentical.hpp \
                src/FilterDelete.hpp \
                src/FilterDeleteWhitespace.hpp \
                src/FilterIdentity.hpp \
                src/FilterToLower.hpp \


SOURCES       = src/Common.cpp  \
                src/ComparatorFactory.cpp \
                src/ComparatorLevenshteinDistance.cpp \
                src/ComparatorLongestMatch.cpp \
                src/CompareAlgo.cpp \
                src/ComparisonMatrix.cpp \
                src/FilterDeleteComments.cpp \
                src/FilterFactory.cpp \
                src/Main.cpp \
                src/renderarea.cpp \
                src/window.cpp

INCLUDEPATH += /usr/include/boost/
LIBS += -L/usr/lib64/ -lboost_regex -lboost_iostreams -lboost_filesystem -lboost_system -lboost_program_options -l:libmagic.so.1
CONFIG += qt
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic
TARGET = duplicode
