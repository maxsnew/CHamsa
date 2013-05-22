SHELL=/bin/sh

CC=gcc

#these are for testing
#CFLAGS = -ggdb -pg -W -Wall -Winline 

#these are for maximum speed
CFLAGS=-g -O3 -fomit-frame-pointer -W -Wall -Winline \
#       -DDEBUG=0 -DNDEBUG=1 


.PHONY: all
all : ds unbwt bwt testlcp token


# deep-shallow suffix sort algorithm
token: tokenmain.o libtkex.a libds_ssort.a 
	$(CC) $(CFLAGS) -o token tokenmain.o libtkex.a libds_ssort.a libbwtlcp.a

libtkex.a: token.o TokenVector.o stack.o
	ar rc libtkex.a token.o TokenVector.o stack.o

ds: suftest2.o libds_ssort.a 
	$(CC) $(CFLAGS) -o ds suftest2.o libds_ssort.a

# archive containing the ds sort algorithm
libds_ssort.a: globals.o ds.o shallow.o deep2.o helped.o blind2.o
	ar rc libds_ssort.a globals.o ds.o shallow.o deep2.o helped.o blind2.o

# archive containing the bwt and lcp auxiliary routines 
libbwtlcp.a: bwt_aux.o lcp_aux.o
	ar rc libbwtlcp.a bwt_aux.o lcp_aux.o

# compare several linear time lcp algorithms
testlcp: testlcp.c libbwtlcp.a libds_ssort.a 
	 $(CC) $(CFLAGS) -o testlcp testlcp.c libbwtlcp.a libds_ssort.a 

# inverse bwt
unbwt: unbwt.c
	 $(CC) $(CFLAGS) -o unbwt unbwt.c 

# bwt using ds_ssort
bwt: bwt.c libds_ssort.a
	 $(CC) $(CFLAGS) -o bwt bwt.c libds_ssort.a 

# pattern rule for all objects files
%.o: %.c *.h
	$(CC) -c $(CFLAGS) $< -o $@

clean: 
	rm -f *.o *.a ds unbwt bwt testlcp token






