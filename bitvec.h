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
 * FILE:     bitvec.h 
 * PURPOSE:  General bit vector lib
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




#ifndef _INCL_BITVEC_H
#define _INCL_BITVEC_H

#include <string.h>

using namespace std;

const unsigned int char_size = 0x08;	// 8 bits in 1 char(unsigned)
const unsigned int char_bits = 3;
const unsigned int char_mask = 0x07;
const unsigned int bytemask = 0xff;

const unsigned char bit_mask[8] = {
	0x01,			//00000001
	0x02,			//00000010
	0x04,			//00000100
	0x08,			//00001000
	0x10,			//00010000
	0x20,			//00100000
	0x40,			//01000000
	0x80			//10000000
};

class bit_vec {

      public:

	bit_vec(unsigned int bit_size) {
		bitlen=bit_size;
		bytelen = bitlen/char_size + 1;
		bvec = new unsigned char[bytelen];
		for (unsigned int i = 0; i < bytelen; i++) {
			bvec[i] = 0;
		}
	} 

	bit_vec(bit_vec *oldbvec) {
		bitlen=oldbvec->bitlen;
		bytelen = oldbvec->bytelen;;
		onecnt=oldbvec->onecnt;
		bvec = new unsigned char[bytelen];
		memcpy(bvec,oldbvec->bvec,bytelen);
	} 
	
	~bit_vec() {
		delete[]bvec;
	}

	void setbit(const unsigned int bitidx) {
		bvec[bitidx >> char_bits] |=
		    bit_mask[bitidx & char_mask];
	}

	void clearbit(const unsigned int bitidx) {
		//unsigned char umask;

//		bvec[bitidx >> char_bits] &= (bytemask - bit_mask[bitidx & char_mask]);
		bvec[bitidx >> char_bits] &= (~bit_mask[bitidx & char_mask]);

	}

	bool testbit(const unsigned int bitidx) {
		return ((bvec[bitidx >> char_bits] & bit_mask[bitidx & char_mask]) != 0);
		
	}



	unsigned int cntone () {
		unsigned int cnt=0;
		unsigned int i;
		for (i=0; i<bitlen; i++) {
			unsigned int bit = i & char_mask;
			if ((bvec[i >> char_bits] & bit_mask[bit]) == bit_mask[bit]) ++cnt;
		}
		onecnt=cnt;
		return cnt;
	}


//      private:
	unsigned char *bvec;
	unsigned int bytelen;
	unsigned int bitlen;
	unsigned int onecnt;


};

static void vec_and (bit_vec *dst, bit_vec *src) {
	assert (dst->bitlen == src->bitlen);

	for (unsigned int i=0; i<dst->bytelen; i++) {
		dst->bvec[i] &= src->bvec[i];
	}

}

#endif
