CXX = g++
RM = rm -f
LDLIBS = 
CCFLAGS = -Wall -g -Wpedantic -std=c++11

# e. g. .exe for win
EXEC_EXT=
#separator only replaced when deleting files, \ on windows.
SEP = $(strip /)
TARGET = moot$(EXEC_EXT)
