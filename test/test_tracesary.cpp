#include <fstream>
#include <iostream>
#include <string.h>

#define SFILE	"/home3/ND/Webcapture1121127986.0"

using namespace std;

#include "bitvec.h"
#include "tracesary.h"

void testsary(char *token) {
	
	TraceSary ts1(SFILE);
	
	printf ("test token occurence of token \"north\" :%d\n",ts1.token_occurrence(token,strlen(token)));
	printf ("test token unique occurence of token \"north\" :%d\n",ts1.token_unique_occurrence(token,strlen(token)));

	list<TraceSary*> tslist;
	tslist.clear();
	tslist.push_back(&ts1);

	unsigned int totallen=caln_totalsample(tslist);
	bit_vec *bvec=token_sigvector(tslist,totallen,token,strlen(token),1);

	cout << "# of 1: " << bvec->cntone() <<endl;

}

int main (int argc, char* argv[]) {

	testsary(argv[1]);
	return 0;

}
