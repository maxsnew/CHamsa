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
 * FILE:     conjpattern.h 
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

#ifndef _INCL_CONJPATTERN_H
#define _INCL_CONJPATTERN_H

#include <string>
#include <vector>
#include <list>

#include "bitvec.h"
#include "tracesary.h"

using namespace std;

class Tokenobj {

  public:
	//Constructor
	Tokenobj (string tokenstr, unsigned int tokencnt);

	//Destructor
	~Tokenobj();

	//Member Functions
	bool isequal(Tokenobj* player);
	bool issubset(Tokenobj* player);
	bool issuperset(Tokenobj* player);
	unsigned int length();
	void cal_covvec(unsigned char *sv,unsigned int numsample);
	void testfp(list <TraceSary*> & normallist, bool flush=false);


  //private:
	string token;
	unsigned int cnt;
	unsigned int cov;
	double fpos;
	bit_vec *covvec, *fpvec;
	unsigned int score;
	

	
};

class ConjPattern {

  public:
	//Constructor
	ConjPattern ();
	ConjPattern (ConjPattern *cjptn);
	ConjPattern (ConjPattern *cjptn, Tokenobj *tk);
	ConjPattern (Tokenobj *tk);
	//Destructor
	~ConjPattern();

	//Member Functions
	bool istkinclude(Tokenobj *newtk);
	unsigned int testjoincov(Tokenobj *tk);
	double testjoinfp(Tokenobj *tk);

	bool match(string & data);
//	double testfp_match();

	void printme();
	void crtsaveformat(string fname); //unsure yet!!!
	unsigned int length();


	

  //private:
	vector<Tokenobj> tokens;
	unsigned int cov;
	double fpos;
	bit_vec *covvec, *fpvec;
	double score;

};

struct cacheitem {
	int iage;
	bit_vec *fpbvec;
};

typedef struct cacheitem cacheitem_t;

#ifdef __IMPL_CONJPATTERN
	void flushcache();
#else
	extern void flushcache();
#endif /* __IMPL_CONJPATTERN */

#endif
