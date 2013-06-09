/*
 **************************************************************************
 *          
 *             N o r t h w e s t e r n     U n i v e r s i t y             
 *                                                                         
 * Hamsa - Polymorphic Worm Signature Generation Project -- Zhichun Li     
 *                                                       -- Mananan Sanghi 
 *                                                       -- Brian Chavez   
 *                                                       -- Yan Chen       
 *                                                       -- Ming-Kang Kao  
 *                                                                         
 * FILE:     tracesary.cpp
 * DATE:     October 31, 2005                                              
 * AUTHOR:   Zhichun Li, lizc@cs.northwestern.edu                      
 * PROJECT:  Hamsa                                                         
 *              
 *                                                                         
 * The Northwestern University
 * All rights reserved.
 * Redistribution in source and binary forms, with or without
 * modification, are NOT permitted without permission from the authors:
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
 */
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>

#define __IMPL_TRACESARY
#include "tracesary.h"
#include "utils.h"

using namespace std;
/*
 * Lots of potential optimizations could be done
 * 1) cache the token bitvector cross different iteration
 * 2) cache sample vector, so for the token with different token_rptnum, can be faster
 * 3) use direct map instead of log(n) binary search map, but waste 4n space!
 *
 */
TraceSary::TraceSary(string streamfilename)
:DataStream(streamfilename)
{
	

	sary_fname=streamfilename + "/data";

	sarylength = (unsigned int)(getfilesize(sary_fname));
	sary=sary_searcher_new(sary_fname.c_str());

	if (sary==NULL) {
		cerr << "Could not open sarray: " << sary_fname <<endl;
	}



}


TraceSary::~TraceSary()
{
	sary_searcher_destroy(sary);
	//DataStream::~DataStream();
}


unsigned int TraceSary::offset2index(unsigned int offset,unsigned int start)
{
	unsigned int end,current,thisone;

	if (offset > sarylength) {
		cerr << "suffix's offset out of the bound of the orignal string!" << endl;
		exit(1);
	}
	
	end=numsample-1;
	if (offset >= idx2off[end]) return end;

	while (1) {
		current= (unsigned int)((end+start)/2);
		thisone= idx2off[current];
		
		if (offset < thisone) {
			end=current;
		} else {
			start=current;
		}

		if (end == (start+1)) {
			if (offset >= idx2off[start] && offset < idx2off[end]) {
				return start;
			} else {
				cerr << "BUG! TraceSary::offset2index: end==start+1" << endl;
				exit(1);
			}

		} else if (end == start) {
			if (offset >= idx2off[start] and offset< idx2off[start+1]) {
				return start;
			} else {
				cerr << "BUG! TraceSary::offset2index: end==start" << endl;
				exit(1);
			}
		}
				
	}

	return start;

}	

unsigned int TraceSary::token_occurrence(char* token, unsigned int tokenlen)
{
	if (! sary_searcher_search2(sary,token,tokenlen)) {
		return 0;
	} else {
		return sary_searcher_count_occurrences(sary);
	}

}

unsigned int TraceSary::token_unique_occurrence(char* token, unsigned int tokenlen)
{
	if (! sary_searcher_search2(sary,token,tokenlen)) {
		return 0;
	} 

	sary_searcher_sort_occurrences(sary);
	unsigned int count = sary_searcher_count_occurrences(sary);

	unsigned int nextone=0;
	unsigned int uniquecount=0;
	unsigned int streami=0;
	unsigned int fileoffset=0;

	for (unsigned int i=0; i<count; i++) {
		fileoffset = sary_searcher_get_next_offset(sary);
		assert(fileoffset>=0);

		if (fileoffset >= idx2off[nextone]) {
			uniquecount+=1;

			streami=offset2index(fileoffset);
			nextone=streami+1;

			if (nextone == numsample) break;

		}
	}

	return uniquecount;
}

void TraceSary::token_samplevector(vector <unsigned short> & svec, const char* token, unsigned int tokenlen)
{
	unsigned int i;
	svec.clear();
	svec.reserve(numsample);
	svec.resize(numsample);
	for (i=0;i<numsample;i++) {
		svec[i]=0;
	}
	
	if (! sary_searcher_search2(sary,token,tokenlen)) {
		return;
	}

	unsigned int count = sary_searcher_count_occurrences(sary);
	
	unsigned int current=0;
	unsigned int nextone=0;
	unsigned int offset;

	for (i=0;i<count;i++) {
		offset=sary_searcher_get_next_offset(sary);
		assert(offset>=0);
		
		if (offset >= idx2off[current] && offset < idx2off[nextone]) {
			++svec[current];
		} else {
			current=offset2index(offset);
			nextone=current+1;
			++svec[current];
		}
	}


}
/*
 * Calculate the sample vector and bit vector
 * Output the bit vector
 */
bit_vec* token_sigvector(list <TraceSary*> & normallist, unsigned int vlen, const char* token, unsigned int tokenlen, unsigned int token_rptnum)
{
	vector<unsigned short> samvec;
	bit_vec* bvec = new bit_vec(vlen);
	
	unsigned int i=0;
	list<TraceSary*>::iterator liter;
	vector<unsigned short>::iterator viter;
	
	for (liter=normallist.begin(); liter!=normallist.end(); ++liter) {
		(*liter)->token_samplevector(samvec,token,tokenlen);
		for (viter=samvec.begin(); viter!=samvec.end(); ++viter) {
			if ((*viter) >= token_rptnum) {
				bvec->setbit(i);
			} else {
				bvec->clearbit(i);
			}
			i++;
		}
	}

	return bvec;
}
/*
 * Calculate the total # of samples of all the data in the normallist,
 * so that we only need to calculate once
 */
unsigned int caln_totalsample(list <TraceSary*> & normallist)
{
	unsigned int sum=0;
	list<TraceSary*>::const_iterator liter;

	for (liter=normallist.begin(); liter!=normallist.end(); ++liter) {
		sum+=(*liter)->numsample;
	}

	return sum;
}

	
	
