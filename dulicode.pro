SOURCE        = src/
HEADERS       = src/AbstractComparator.h \
                src/AbstractFilter.h \
                src/Common.h \
                src/ComparatorFactory.h \
                src/ComparatorLevenshteinDistance.h \
                src/ComparatorLongestMatch.h \
                src/ComparatorUncommonStrings.h \
                src/CompareAlgo.h \
                src/ComparisonMatrix.h \
                src/difflib.h \
                src/FilterDeleteComments.h \
                src/FilterDeleteHash.h \
                src/FilterFactory.h \
                src/magic.h \
                src/ComparisionView.h \
                src/MatrixWindow.h \
                src/ComparatorDiffLib.hpp \
                src/ComparatorIdentical.hpp \
                src/FilterDelete.hpp \
                src/FilterDeleteWhitespace.hpp \
                src/FilterIdentity.hpp \
                src/FilterToLower.hpp \
                src/FilterSplitStructure.hpp \
                src/CodeFile.hpp


SOURCES       = src/Common.cpp  \
                src/ComparatorFactory.cpp \
                src/ComparatorLevenshteinDistance.cpp \
                src/ComparatorLongestMatch.cpp \
                src/ComparatorUncommonStrings.cpp \
                src/CompareAlgo.cpp \
                src/ComparisonMatrix.cpp \
                src/FilterDeleteComments.cpp \
                src/FilterDeleteHash.cpp \
                src/FilterFactory.cpp \
                src/Main.cpp \
                src/ComparisionView.cpp \
                src/MatrixWindow.cpp

INCLUDEPATH += /usr/include/boost/
LIBS += -L/usr/lib64/ -lboost_regex -lboost_iostreams -lboost_filesystem -lboost_system -lboost_program_options -l:libmagic.so.1
CONFIG += qt
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic -Werror -g
TARGET = duplicode
