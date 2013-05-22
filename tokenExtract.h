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
 * FILE:     tokenExtract.h
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
#ifndef _INCL_TOKENEXTRACTION_H
#define _INCL_TOKENEXTRACTION_H

#include "TokenEXAPI/TokenVector.h"
#include "TokenEXAPI/ds_ssort.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>

#include "conjpattern.h"

using namespace std;

class SusTrace {
  public:
	//Constructor
	SusTrace(list <string*> & sample)
	{
		int overshoot = init_ds_ssort(500,2000);

		numsample=sample.size();
		idx2off= new unsigned int[numsample+1];

		length=0;
		list<string*>::const_iterator liter;

		for (liter=sample.begin(); liter!=sample.end(); ++liter)
		{
			length+=(*liter)->length();
		}

		//printf("SusTrace::constructor:: length=%u, numsample=%u\n",length, numsample);

		data = new char[length+overshoot];
		if (!data)
		{
			cerr << "Cannot allocate memory for suspicious pool object!" <<endl;
			exit(EXIT_ERROR);
		}

		char * pos=data;
		unsigned int start=0;
		unsigned int i=0;
		bzero(data,length+overshoot);
		for (liter=sample.begin(); liter!=sample.end(); ++liter)
		{
			//register unsigned int l=(*liter)->length();
			unsigned int l=(*liter)->length();
			memcpy(pos,(*liter)->c_str(),l);
			pos+=l;
			idx2off[i]=start;
			i++;
			start+=l;
		}
		idx2off[numsample]=length;
	}

	//Destructor
	~SusTrace()
	{
		delete[] idx2off;
		delete[] data;
	}
	//Member Functions
	void extract_ptntoken (vector<Tokenobj*> & tkobjlist, unsigned int min_token_len, unsigned int min_token_occ);
	void extract_uniptn (vector<Tokenobj> & tkobjlist, unsigned int min_token_len);

//  private:
	unsigned int numsample;
	unsigned int *idx2off;
	char * data;
	unsigned int length;
};



#endif
