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
 * FILE:     tokenExtract.cpp 
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


#include "TokenEXAPI/TokenVector.h"
#include "TokenEXAPI/ds_ssort.h"

#define __IMPL_tokenExtract
#include "tokenExtract.h"
#include "utils.h"

using namespace std;

static void print_str(char* str, unsigned int len)
{
	unsigned int i;

	for (i=0; i<len; i++)
	{
		if (isalnum(str[i])) 
		{
			putchar(str[i]);

		} else if (str[i]==' ')
		{
			putchar(' ');

		} else if (str[i]=='\t')
		{
			printf("\\t");

		} else if (str[i]=='\n')
		{
			printf("\\n");

		} else if (str[i]=='\r')
		{
			printf("\\r");

		} else if (ispunct(str[i]))
		{
			printf("%c",str[i]);

		} else {
			printf("\\x%02x",str[i]);
		}
	}
}



void SusTrace::extract_ptntoken (vector<Tokenobj*> & tkobjlist, unsigned int min_token_len, unsigned int min_token_occ) 
{
	unsigned int histogram[MAX_ALLOWREP+1];
	unsigned int i,maxcount=0;
	int ptncount=0;
	int tokencount=0;

	TokenVectorHndl tlist,tvtemp;
	tlist=TokenExtraction(data, length, idx2off, numsample, min_token_len, min_token_occ);

//	printf("enter SusTrace::extract_ptntoken\n");
//	printf("min_token_len=%u, min_token_occ=%u\n",min_token_len,min_token_occ);

/* DEBUG TOKEN EXTRACTION
	TokenVectorHndl tvTemp=tlist;
	while( tvTemp != NULL ){
		printf("TOKEN:'");
		print_str( tvTemp->string, tvTemp->strLen );

		printf("'\n");
		printf("TOKENVECTOR ");
		for( i = 0; i < numsample; i++ )
		{
			if( tvTemp->occuranceVector[i] != 0 ){
				printf("%d:%d  ", i, tvTemp->occuranceVector[i] );
			}
		}
		printf("\n");

		tvTemp = tvTemp->next;
	}
	cout << "SUCCESS EXTRACT TOKEN!" << endl;
*/



	tvtemp=tlist;
	while (tvtemp!=NULL)
	{

		for (i=0;i<MAX_ALLOWREP+1;i++) histogram[i]=0;

		maxcount=0;
		for (i=0; i<numsample ; i++)
		{
			if(tvtemp->occuranceVector[i]>maxcount) maxcount=tvtemp->occuranceVector[i];
		}

//		cout << string(tvtemp->string,tvtemp->strLen) <<" MAXCOUNT=" << maxcount <<endl;

		for (i=0; i<numsample ; i++)
		{
			histogram[tvtemp->occuranceVector[i]]++;
//			printf("i=%d Vector[%d]=%d histogram[Vecter[%d]]=%d\n",i,i,
//			tvtemp->occuranceVector[i],i,histogram[tvtemp->occuranceVector[i]]);
		}

		for (i=maxcount-1; i>=1; --i) //bug!!!
		{
			histogram[i]+=histogram[i+1];
//			cout <<"i= " << i << "histogram[i]= " << histogram[i] << endl;
		}

/*DEBUG HISTOGRAM
		cout << "VECTOR(1-maxcount): ";
		for (i=1; i<=maxcount; i++)
		{
			cout << histogram[i] <<"  ";
		}
		cout <<endl;
**/
		for (i=1; i<=maxcount ; i++)
		{
			if ((i==maxcount) || ((i<maxcount) && (histogram[i]>histogram[i+1]))) 
			{
				if (histogram[i]>=min_token_occ)
				{
					string tkstr=string(tvtemp->string,tvtemp->strLen);
					Tokenobj *tmptkobj= new Tokenobj(tkstr,i);
//					cout << "DEBUG:::ADD TOKEN: " << tkstr << "  cnt=" << i <<endl;
					tmptkobj->cal_covvec(tvtemp->occuranceVector,numsample);
					tkobjlist.push_back(tmptkobj);
					ptncount++;

				} else {
					break;
				}
			}
		}

		tvtemp=tvtemp->next;
		tokencount++;
	}

	DeleteTokenVectorList(tlist);
	cout << "Total # of tokens = " << tokencount <<endl <<endl;
	//cout << "size of raw patterns = " << tkobjlist.size() <<endl;
	//cout << "true size of raw patterns = " << ptncount <<endl;

}

void SusTrace::extract_uniptn (vector<Tokenobj> & tkobjlist, unsigned int min_token_len) 
{
	unsigned int histogram[MAX_ALLOWREP+1];
	unsigned int i,maxcount=0;

	TokenVectorHndl tlist,tvtemp;
	tlist=TokenExtraction(data, length, idx2off, numsample, min_token_len, numsample);

	tvtemp=tlist;
	while (tvtemp!=NULL)
	{

		for (i=0;i<MAX_ALLOWREP+1;i++) histogram[i]=0;

		maxcount=0;
		for (i=0; i<numsample ; i++)
		{
			if(tvtemp->occuranceVector[i]>maxcount) maxcount=tvtemp->occuranceVector[i];
		}

		for (i=0; i<numsample ; i++)
		{
			histogram[tvtemp->occuranceVector[i]]++;
		}

		for (i=maxcount-1; i>=1; --i)
		{
			histogram[i]+=histogram[i+1];
		}

		for (i=maxcount; i>0 ; i--)
		{
			if (histogram[i]>=numsample)
			{
				string tkstr=string(tvtemp->string,tvtemp->strLen);
				Tokenobj tmptkobj(tkstr,i);
				tkobjlist.push_back(tmptkobj);
				break;
			}

		}

		tvtemp=tvtemp->next;
	}

	DeleteTokenVectorList(tlist);
	//cout << "size of raw patterns = " << tkobjlist.size() <<endl;


}
