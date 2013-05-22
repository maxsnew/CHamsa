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
 * FILE:     global.cpp 
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

#ifndef _INCL_GLOBALS_H
#define _INCL_GLOBALS_H

#define CACHELEVEL		1
#define EXIT_SUCCESS	0
#define EXIT_ERROR		1
#define MAX_ALLOWREP	255

#ifdef __DEFINE__GLOBALS
unsigned int totalnumsample=0;
unsigned int totalnormal=0;
unsigned int min_token_len=2;
unsigned int min_cov_token=3;
double min_coverage=0.15;
double Gfinalfp=0.01;
unsigned int GKbound=15;
double GUbound[15]={
	0.2,0.1,0.04,0.025,0.015,
	0.0055, 0.0025, 0.0015, 0.001, 0.0008,
	0.0006, 0.0005, 0.0004, 0.0003, 0.0002
	};

bool Gtestcache=true;
int Gage=0;

#else
extern unsigned int totalnumsample;
extern unsigned int totalnormal;
extern unsigned int min_token_len;
extern unsigned int min_cov_token;
extern double min_coverage;
extern double Gfinalfp;
extern unsigned int GKbound;
extern double GUbound[15];
extern bool Gtestcache;
extern int Gage;

#endif


#endif
