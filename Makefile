###############################################################################
#
# File:         Makefile
# Description:  Guess
# Author:       Zhichun Li
#               Northwestern Systems Research Group
#               Department of Computer Science
#               Northwestern University
# Language:     Makefile
# Package:      N/A
# Status:       Experimental (Do Not Distribute)
#
# (C) Copyright 2006, Northwestern University, all rights reserved.
#
###############################################################################

CXX = g++
MV = mv
CP = cp
RM = rm
MKDIR = mkdir
TAR = tar cvf
COMPRESS = gzip

INC = $(shell pkg-config --cflags sary)
#CXXFLAGS = -Wall -ggdb -c ${INC}
CXXFLAGS = -Wall -O3 -c ${INC}
LIBS = -lm $(shell pkg-config --libs sary) -LTokenEXAPI -ltkex -lds_ssort -lbwtlcp


TARGET = test_tracesary 
TARGET = siggen
STATICLIBS = 
SOURCES = test_tracesary.cpp  tracesary.cpp  utils.cpp 
SOURCES = siggen_main.cpp  tracesary.cpp  utils.cpp tokenExtract.cpp conjpattern.cpp sig_gen.cpp
SOURCES = tracesary.cpp  utils.cpp tokenExtract.cpp conjpattern.cpp sig_gen.cpp
HEADER = utils.h tracesary.h bitvec.h globals.h

OBJS = ${SOURCES:.cpp=.o}

all: ${TARGET} siggen_nonoise

#utils.o: utils.cpp ${HEADER}
#	${CXX} ${CXXFLAGS}  $<

#tracesary.o: tracesary.cpp ${HEADER}
#	${CXX} ${CXXFLAGS}  $<

#test_tracesary.o: test_tracesary.cpp ${HEADER}
#	${CXX} ${CXXFLAGS}  $<

#siggen.o: sigen.cpp ${HEADER}
#	${CXX} ${CXXFLAGS}  $<

${TARGET}: siggen_main.o ${OBJS}
	${CXX} -o $@ ${OBJS} siggen_main.o ${LIBS}

siggen_nonoise: siggen_nonoise.o ${OBJS}
	${CXX} -o $@ ${OBJS} siggen_nonoise.o ${LIBS}

%.o: %.cpp *.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	${RM} -f *.o *~ ${TARGET} siggen_nonoise

cleanAll: clean
