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
 * FILE:     calscore.h 
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

#include <math.h>

#define ALPHA  	20.0
#define BETA	0.01
#define LOGBASE 10
#define MIN_FP	1e-6


static double calscore (unsigned int covth, double fpos, unsigned int length, unsigned int totalsample)
{
	double score;

	score= -log10(MIN_FP+fpos) + ALPHA*1.0*covth/totalsample + BETA*1.0*length;

	return score;
}
