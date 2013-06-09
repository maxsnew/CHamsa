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
 * FILE:     siggen_main.cpp 
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
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <string>
#include <list>

#define __DEFINE__GLOBALS
#include "globals.h"
#include "tracesary.h"
#include "conjpattern.h"

using namespace std;

extern void gen_sig (vector<ConjPattern*> & sigs, list<string*> & sample, unsigned int min_residue, 
	double coverage,list<TraceSary*> & normallist);


#define PATH_SEP '/'

static char* prog_name;
string suspool_name="";
string norpool_name="";
string sigpath="";
double U0=GUbound[0];
double Ur=0.5;


static void printcmdline(char *pname);
static void handleopt(int argc, char* argv[]);
static void gen_ubound(unsigned int K, double U0, double Ur);
void loadsample(list<string*> & sample, string suspool_name);

int main(int argc, char *argv[])
{
	if ((prog_name=strrchr(argv[0],PATH_SEP))!=NULL)
		prog_name++;
	else
		prog_name=argv[0];
		
	handleopt(argc,argv);

	gen_ubound(GKbound,U0,Ur);

	list<TraceSary*> normallist;
	TraceSary *norpool = new TraceSary(norpool_name);
	normallist.push_back(norpool);

//	unsigned int normallen=caln_totalsample(normallist);
//	bit_vec *testn=token_sigvector(normallist,normallen,"test",4,1);
//	cout <<"TEST occur" << testn->cntone() << endl;

	SusDataStream susdatapool(suspool_name);
	list<string*> sample;
	sample.clear();
	susdatapool.loadsample(sample);
        printf("susdatapool.length=%u\n",susdatapool.length);

/*DEBUG
	Gage++;
	Tokenobj *debugobj= new Tokenobj("sexy",2);
	Tokenobj *debugobj1= new Tokenobj("pp",1);
	Tokenobj *debugobj2= new Tokenobj("sex",2);

	unsigned char dsv[10]={0,0,1,1,2,2,1,3,9,1};

	debugobj->cal_covvec(dsv,10);	
	debugobj->testfp(normallist);

	debugobj1->cal_covvec(dsv,10);
	debugobj1->testfp(normallist);

	ConjPattern *cjn0= new ConjPattern();
	ConjPattern *cjn1= new ConjPattern(debugobj);
	ConjPattern *cjn2= new ConjPattern(cjn1, debugobj1);
	ConjPattern *cjn3= new ConjPattern(cjn2);

	cout << "Conj debugobj & debugobj1 cov=" << cjn3->cov <<endl;
	cout << "Conj debugobj & debugobj1 fpos=" << cjn3->fpos <<endl;

	cout << "testjoincov" << cjn2->testjoincov(debugobj1) <<endl;
	cout << "testjoinfp" << cjn3->testjoinfp(debugobj) <<endl;

	cjn3->printme();

	cjn3->crtsaveformat("debug1");

	exit(3);


DEBUG*/
	unsigned int covth = (unsigned int) (min_coverage*sample.size()+0.5);
        printf("covth=min_coverage*sample.size=%.3f*%u=%u\n",min_coverage,sample.size(),covth);
	vector<ConjPattern*> sigs;
	gen_sig (sigs, sample, covth, min_coverage, normallist);

	if (mkdir(sigpath.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP)==-1)
	{
		cerr << "Cannot create directory: " << sigpath << " : " <<strerror(errno) <<endl;
		exit(EXIT_ERROR);
	}

	vector<ConjPattern*>::iterator cppos;
	unsigned int i=0;
	char namebuf[30];
        
	for (cppos=sigs.begin(); cppos!=sigs.end(); ++cppos)
	{
		i++;
		bzero(namebuf,30);
		snprintf(namebuf,30,"/SIG%d",i);
		(*cppos)->crtsaveformat(sigpath + namebuf);
	}



	cerr << "FINISH!" <<endl;

	return 0;
}


static void gen_ubound(unsigned int K, double U0, double Ur)
{
	unsigned int i;
	double U=U0;

	GUbound[0]=U0;

	for (i=1; i<K; i++)
	{
		U=U*Ur;
		GUbound[i]=U;
	}
}

static void printcmdline(char *pname)
{
	printf("Usage : %s -S <SusPool> -N <NorPool> -L <min_tlen> -r <min_coverage> -U <U0> -v <Ur> -K <Kstar>\n"
	"\t-S\tthe directory of suspicious pool including data and offset\n"
	"\t-N\tthe directory of normal pool including data and offset\n"
	"\t-G\tthe directory for saving the set of signatures\n"
	"\t-L\tthe minimum token length, default=2\n"
	"\t-r\tthe minimum coverage of a worm also the rThreshold for token extraction, default=15%%\n"
	"\t-U\tthe U0 parameter\n"
	"\t-v\tthe Ur parameter\n"
	"\t-K\tthe Kstar parameter (the maximum length of initial signature), should be less than 15\n"
	,pname);
}

static void handleopt(int argc, char* argv[])
{
	int lmin=min_token_len;
	double covmin=min_coverage;
	double u0=U0;
	double ur=Ur;
	int Kstar=GKbound;

	int c;
	while (1) {
		c=getopt(argc,argv,"S:N:G:L:r:U:v:K:h");
		if (c==-1) break;
		switch (c) {
			case 'S':
				if(strcmp(optarg,"")==0) {
					fprintf(stderr,"Missing The directory of Suspicious pool including data, offset!\n");
					printcmdline(prog_name);
					exit(EXIT_ERROR);
				}
				suspool_name=string(optarg);
				break;

			case 'N':
				if(strcmp(optarg,"")==0) {
					fprintf(stderr,"Missing The directory of Normal pool including data, offset!\n");
					printcmdline(prog_name);
					exit(EXIT_ERROR);
				}
				norpool_name=string(optarg);
				break;

			case 'G':
				if(strcmp(optarg,"")==0) {
					fprintf(stderr,"Missing The directory for saving the signatures!\n");
					printcmdline(prog_name);
					exit(EXIT_ERROR);
				}
				sigpath=string(optarg);
				break;

			case 'L':
				lmin=atoi(optarg);
				if(lmin<=0) {
					fprintf(stderr,"Missing min_token_len -L should be followed by the length!\n");
					printcmdline(prog_name);
					exit(EXIT_ERROR);
				}
				if (lmin>min_token_len) min_token_len=lmin;
				
				break;

			case 'r':
				covmin=atof(optarg);
				if(covmin<=0.01 || covmin>=1) {
					fprintf(stderr,"Missing min_coverage -r should be followed by the coverage!\n");
					printcmdline(prog_name);
					exit(EXIT_ERROR);
				}

				min_coverage=covmin;
				
				break;

			case 'U':
				u0=atof(optarg);
				if(u0<=0 || u0>=1) {
					fprintf(stderr,"Missing U0 -U should be followed by the U0!\n");
					printcmdline(prog_name);
					exit(EXIT_ERROR);
				}

				U0=u0;
				
				break;

			case 'v':
				ur=atof(optarg);
				if(ur<=0 || ur>=1) {
					fprintf(stderr,"Missing Ur -v should be followed by the Ur!\n");
					printcmdline(prog_name);
					exit(EXIT_ERROR);
				}

				Ur=ur;
				
				break;

			case 'K':
				Kstar=atoi(optarg);
				if(Kstar<=0 || Kstar > GKbound) {
					fprintf(stderr,"Missing Kstar -K should be followed by the Kstar!\n");
					printcmdline(prog_name);
					exit(EXIT_ERROR);
				}

				GKbound=Kstar;
				
				break;

			case 'h':
				
				printcmdline(prog_name);
				exit(EXIT_SUCCESS);
				break;

			default:
				fprintf(stderr,"Parameter error!\n");
				printcmdline(prog_name);
				exit(EXIT_ERROR);
				break;
		}
	}

	if (optind!=argc) {
		fprintf(stderr,"Parameter error!\n");
		printcmdline(prog_name);
		exit(EXIT_ERROR);
	}

	if (suspool_name=="")
	{
		fprintf(stderr,"Suspicious pool directory name cannot be empty!\n");
		printcmdline(prog_name);
		exit(EXIT_ERROR);
	}

	if (norpool_name=="")
	{
		fprintf(stderr,"Normal pool directory name cannot be empty!\n");
		printcmdline(prog_name);
		exit(EXIT_ERROR);
	}

	if (sigpath=="")
	{
		fprintf(stderr,"the directory name for saving signatures cannot be empty!\n");
		printcmdline(prog_name);
		exit(EXIT_ERROR);
	}

}



