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
 * FILE:     tracesary.h
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
#ifndef _INCL_TRACESARY_H
#define _INCL_TRACESARY_H

#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>

#include "globals.h"
#include "bitvec.h"
#include "utils.h"

#include <sary/searcher.h>

using namespace std;

class DataStream {
  public:
	DataStream(string streamfilename) {
		streamfname=streamfilename + "/data";
		offset_fname = streamfilename + "/offsets";

		numsample= (unsigned int)(getfilesize(offset_fname) /4 );
		length = (unsigned int)(getfilesize(streamfname));

		idx2off= new unsigned int[numsample+1];
		fstream f;
		unsigned int i=0;
		unsigned int offset;

		f.open((offset_fname.c_str()), ios::in | ios::binary);
		if (!f)
		{
			cerr <<"Error open the file " <<offset_fname << endl;
			exit(EXIT_ERROR);
		}

		while (f.read((char*)&offset,sizeof(unsigned int))) {
			idx2off[i]=offset;
			i++;
		}
		//possible problem i!=numsample 
		//error offset file
		f.close();	
		idx2off[numsample]=length;	
	}

	~DataStream() {
		delete[] idx2off;
//		printf ("CALL BASE CLASS\n");
	}

	string streamfname;	
	string offset_fname;

	unsigned int numsample;
	unsigned int *idx2off;
	unsigned int length;
};


class SusDataStream : public DataStream {
  public:
	//Constructor
	SusDataStream (string streamfilename)
	:DataStream(streamfilename)
	{}


	//Destructor
	~SusDataStream() {}

	//Member Functions
	void loadsample(list<string*> & sample)
	{
		fstream f;
		unsigned int i=0;

		f.open((streamfname.c_str()), ios::in | ios::binary);
		if (!f)
		{
			cerr <<"Error open the file " <<streamfname << endl;
			exit(EXIT_ERROR);
		}

		unsigned int maxslen=0;
		unsigned int slen;
		for (i=0; i<numsample; i++)
		{
			slen=idx2off[i+1]-idx2off[i];
			if (slen>maxslen) maxslen=slen;
		}
		maxslen+=5; //add 5 for safe 
		char * buf = new char[maxslen];
		for (i=0; i<numsample; i++)
		{
			slen=idx2off[i+1]-idx2off[i];
			if (!f.read(buf,slen))
			{
				cerr <<"Bug, inconsistency between data and offset of " <<streamfname <<endl;
				exit(EXIT_ERROR);
			}
			string * tmpstring = new string(buf,slen);
			sample.push_back(tmpstring);
		}

		delete[] buf;
		f.close();	
	}

};

//TODO if need implement it for testfp_match
/*class StreamTrace : public DataStream {
  public:
	

};*/

class TraceSary : public DataStream{

  public:
	//Constructor
	TraceSary (string streamfilename);

	//Destructor
	~TraceSary();

	//Member Functions
	unsigned int offset2index(unsigned int offset,unsigned int start=0);
	unsigned int token_occurrence(char* token, unsigned int tokenlen);
	unsigned int token_unique_occurrence(char* token, unsigned int tokenlen);
	void token_samplevector(vector <unsigned short> & svec, const char* token, unsigned int tokenlen);

  //private:
	string sary_fname;

	SarySearcher* sary;
	
	unsigned int sarylength;

	

	

};



#ifdef __IMPL_TRACESARY
	bit_vec*  token_sigvector(list <TraceSary*> & normallist, unsigned int vlen, 
		const char* token, unsigned int tokenlen, unsigned int token_rptnum);
	unsigned int caln_totalsample(list <TraceSary*> & normalist);
#else
	extern bit_vec*  token_sigvector(list <TraceSary*> & normallist, unsigned int vlen, 
		const char* token, unsigned int tokenlen, unsigned int token_rptnum);
	extern unsigned int caln_totalsample(list <TraceSary*> & normalist);
#endif
//static unsigned int caln_totalsample(list <TraceSary> & normalist);
#endif
