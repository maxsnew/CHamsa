#define DEBUGoff

/**************************************************************************
 *             N o r t h w e s t e r n     U n i v e r s i t y             *
 *                                                                         *
 * Hamsa - Polymorphic Worm Signature Generation Project -- Zhichun Li     *
 *                                                       -- Mananan Sanghi *
 *                                                       -- Brian Chavez   *
 *                                                       -- Yan Chen       *
 *                                                       -- Ming-Kang Kao  *
 *                                                                         *
 * FILE:     token.c                                                       *
 * DATE:     October 31, 2005                                              *
 * AUTHOR:   Brian Chavez, cowboy@cs.northwestern.edu                      *
 * PROJECT:  Hamsa                                                         *
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
 *                                                                         *
 * CLASS NAME: N/A                                                         *
 * CLASS DEPENDENCIES:  ds_sort - For building suffix arrays.              *
 *                      bwt_aux                                            *
 *                      lcp_aux                                            *
 *                      TokenVector - A C implementation that represents a *
 *                                    token vector.                        *
 *                                                                         *
 *                                                                         *
 * DESCRIPTION:  This the main file used for token extraction.             *
 *                                                                         *
 * REVISION HISTORY:                                                       *
 *      Oct. 31, 2005 - Brian Chavez (cowboy@cs.northwestern.edu)          *
 *                    o Inital version                                     *
 *      Nov. 10, 2005 - Zhichun Li (lizc@cs.northwestern.edu)              *
 *                    o minor updates                                      *
 **************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h> 
#include "ds_ssort.h"
#include "bwt_aux.h"
#include "lcp_aux.h"
#include "TokenVector.h"
#include "interval.h"
#include "stack.h"

#define ALPHA_SIZE 256


unsigned char* dataArray;  //This is used to store the flows
int* suffixArray; //The suffix array of ints into dataArray.
int* lcpArray; //The LCP array generated by the suffix array.
int* sampleIndexArray; //Used to keep track of the samples in dataArray.
int numberOfSamples; //Length of sampleIndexArray.... ***Zhichun refers to this as S
long dataArrayLength = 0;

int kThreshold = 3; //This is the threshold that you must pass to qualify
                    //as a token.
//double rThreshold = 0.3f; //OCC value?

unsigned int covthSample=3;

//Mappings.
//int* originalSampleIDMap; //to initally store every byte of mapping to sample id.
//int* suffixSampleIDMap; //corrected mapping after suffix array has been built.

TokenVectorHndl TLhead = NULL;

void AppendTokenVector(TokenVectorHndl tv ){
	TokenVectorHndl temphead;


	temphead=TLhead;
	TLhead=tv;
	tv->next=temphead;

/*
 * Brian old code, inefficent
	if( head == NULL ){
		head = tv;
		return;
	}

	temp = head;

	while( temp->next != NULL ){
		temp = temp->next;
	}

	temp->next = tv;
	tv->next = NULL;
*/

}



/* Author: Zhichun
 * idx2off is the offsetArray
 * length is the total length of all samples
 */

unsigned int offset2index(unsigned int offset, unsigned int *idx2off, unsigned int numsample, unsigned int length)
{
	unsigned int end,current,thisone;
	unsigned int start=0;

	if (offset > length) {
		fprintf (stderr,"suffix's offset out of the bound of the orignal string!\n");
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
				fprintf(stderr, "BUG! TraceSary::offset2index: end==start+1\n");
				exit(1);
			}

		} else if (end == start) {
			if (offset >= idx2off[start] && offset< idx2off[start+1]) {
				return start;
			} else {
				//cerr << "BUG! TraceSary::offset2index: end==start" << endl;
				fprintf(stderr, "BUG! TraceSary::offset2index: end==start\n");

				exit(1);
			}
		}
	}
	return start;
}	

/* Author: Zhichun
 * 1 means the token is ok
 * 0 means the token is bad  (cross boundary)
 */
unsigned int checktoken (unsigned int Tokenstart, unsigned int Tokenend, unsigned int *idx2off, unsigned int numsample,	unsigned int length)
{
	unsigned int result=1;
	unsigned int i;
	unsigned int idx=offset2index(Tokenstart,idx2off,numsample,length);

	idx++;
	for (i=Tokenstart+1; i<=Tokenend; i++)
	{
		if (i==idx2off[idx])
		{
			result=0;
		}
	}
	return result;
}

/*****************************************************************************
 * report The report() function implementation for Algorithm 4.4 in:
 *            http://www.zbh.uni-hamburg.de/staff/kurtz/papers/AboKurOhl2004.pdf
 * @param interval inter - An interval structure as described in the paper above. 
 *        Typically these interval values are poped from the stack in 
 *        Algorithm 4.4.
 * @return void
 */
void report(interval inter){
	int i = 0;
	int p = 0;
	int interLength = 0;
	int covth = 0;
	TokenVectorHndl sampleVector;
	int tokenStart = 0;
	//int tokenEnd = 0;
	unsigned int result = 1;
	unsigned int idx = 0;
	
	#ifdef DEBUG
		char buf[100]; //debug buf;
	#endif /* ifdef DEBUG */
	//tokenStart = suffixArray[inter.leftb];
	//tokenEnd = suffixArray[inter.leftb]+inter.lcpValue;
	
	interLength = inter.rightb - inter.leftb + 1;
	
	//covth = (int)(rThreshold*numberOfSamples);
	covth = covthSample;
	
	if( interLength >= covth && inter.lcpValue >= kThreshold ){
		//map the interval based on suffix sample ID Map.

		sampleVector = NewTokenVector( numberOfSamples );
		/*for( i = inter.leftb; i <= inter.rightb; i++ ){
			if( sampleVector->occuranceVector[suffixSampleIDMap[i]] < 255 ){
				sampleVector->occuranceVector[suffixSampleIDMap[i]]++;
			}
		}*/
		
		for (i=inter.leftb; i<=inter.rightb; i++) {
			result = 1;
			tokenStart=suffixArray[i];
			idx=offset2index(tokenStart, sampleIndexArray, numberOfSamples, dataArrayLength);
			if ((tokenStart+inter.lcpValue-1)>=sampleIndexArray[idx+1]){
				result=0; 
				#ifdef DEBUG
					bzero(buf,100);
					memcpy(buf,&dataArray[suffixArray[inter.leftb]], inter.lcpValue);
					fprintf(stderr,"DEBUG:TK(%d,%d) idx=%d, pos(idx)=%d, pos(idx+1)=%d, %s\n",
						tokenStart,tokenStart+inter.lcpValue-1, idx,
						sampleIndexArray[idx],sampleIndexArray[idx+1],buf);
				#endif /*ifdef DEBUG*/

			}
			if (sampleVector->occuranceVector[idx]<250){
				sampleVector->occuranceVector[idx]+=result;
			}
		}


		p = 0;

		for( i = 0; i < numberOfSamples; i++ ){
			if( sampleVector->occuranceVector[i] != 0 ){
				p++;
			}
		}

		if( p >= covth ){
			//alloc memory for string
			sampleVector->string = (char*)calloc( inter.lcpValue, sizeof(char) );

			memcpy( (char*)sampleVector->string, &dataArray[suffixArray[inter.leftb]], inter.lcpValue );

			sampleVector->strLen = inter.lcpValue;

			AppendTokenVector( sampleVector );
		}
	}
}



/*****************************************************************************
 * runTokenExtraction() --
 *            This is the main implementation for token extraction. 
 *            This is the last place where I left off.  Zhichun has pointed 
 *            out a good algorithm for keeping track of tokens by
 *            mainting a sorted list of tokens.
 *
 * @param N/A
 * @return N/A
 */
void runTokenExtraction(){
/*
 String Suffix
 Pos    Array
 012345
 ababac 0 [1]
   abac 2 [2]
     ac 4 [3]
  babac 1 [4]
    bac 3 [5]
      c 5 [6]

        LCP
 012345 Array
 ababac - [1]
 abac   3 [2]
 ac     1 [3]
 babac  0 [4]
 bac    2 [5]
 c      0 [6]

sampleIndexArray[0] = 0
suffixArray[1] = 0
suffixArray[2] = 2
suffixArray[3] = 4
suffixArray[4] = 1
suffixArray[5] = 3
suffixArray[6] = 5
lcpArray[1] = -
lcpArray[2] = 3
lcpArray[3] = 1
lcpArray[4] = 0
lcpArray[5] = 2
lcpArray[6] = 0
*/
	
	int i = 0;
	int leftb = 0;

	interval temp;
	temp.lcpValue = 0;
	temp.leftb = 0;
	temp.rightb = PIPENULL;

#ifdef DEBUG
	fprintf(stderr,"TEA DEBUG %s: Running Token Extraction Algorithm...\n", __FUNCTION__);
#endif

	stack_init();
	stack_push( temp );

	for( i = 2; i <= dataArrayLength; i++ )
	{
		leftb = i - 1;

		while( lcpArray[i] < stack_top().lcpValue ){

			temp = stack_pop();
			temp.rightb = i - 1;
			
			report( temp );

			leftb = temp.leftb;
		}
		if( lcpArray[i] > stack_top().lcpValue ){
			temp.lcpValue = lcpArray[i];
			temp.leftb = leftb;
			temp.rightb = PIPENULL;
			stack_push( temp );
		}
	}
}


int * lcp_sa2lcp_9n(unsigned char *text, int n, int *sa)
{
	int i;
	int occ[ALPHA_SIZE];
  
	for(i=0;i<ALPHA_SIZE;i++) occ[i]=0;
  	for(i=0;i<n;i++) occ[text[i]]++;

	return _lcp_sa2lcp_9n(text,n,sa,occ);
}

TokenVectorHndl TokenExtraction(char* data, unsigned int datasize, unsigned int* offset, unsigned int numsample, unsigned int min_token_len, unsigned int min_token_occ ){
	dataArray = data;
	dataArrayLength = datasize;
	sampleIndexArray = offset;
	numberOfSamples = numsample;
	TLhead=NULL;

	int overshoot = 0;
	//int i = 0;
	//int j = 0;

	//DEBUG
	printf("%s: ",__FUNCTION__);
	printf("datasize=%d,numsample=%d,min_token_len=%d,min_token_occ_ratio=%u\n",
		datasize,numsample,min_token_len,min_token_occ);
	
	
	kThreshold = min_token_len;
//	rThreshold = min_token_occ_ratio;
	covthSample = min_token_occ;

	//THE FOLLOWING CODE IS BASICALLY A COPY FROM MAIN()

	//overshoot is some extra space that ds_sort needs
	//to uses as a scratch pad.
	overshoot = init_ds_ssort( 500, 2000 );


//Added by cowboy:  Build the byte-byte sample mapping.
//	originalSampleIDMap = (int*)calloc( dataArrayLength, sizeof( int ) );
//	j = 0;
//	for( i = 0; i < dataArrayLength; i++ ){
//		originalSampleIDMap[i] = j;
//		if( j < numberOfSamples && i == sampleIndexArray[j+1] ){
//			j++;
//			originalSampleIDMap[i] = j;
//		}
//	}

	//Lets allocate some memory for our suffix array.
	//Becareful!  The correct size of the suffix array needs to be allocated,
	//  otherwise, ds_sort will not work.  The suffix array is defined as
	//  dataArrayLength+1
	suffixArray = (int*)calloc( dataArrayLength+1, sizeof(int) );
	//tempSuffixArray = (int*)calloc( dataArrayLength+1, sizeof(int) );

	//This builds the suffix array.
	//Becareful!  ds_ssort() is very specific on these input parameters
	//  1. Pass in the dataArray that needs to be sorted.
	//  2. A pointer to suffixArray[1], not suffixArray[0]!
	//  3. And the length of the data.
	//  Failure to provide suffixArray+1 will result in
	//  incorrectly built suffixArrays.
	ds_ssort( dataArray, suffixArray+1, dataArrayLength );

//	suffixSampleIDMap = (int*)calloc( dataArrayLength, sizeof( int ) );
//	for( i = 0; i < dataArrayLength; i++ ){
//		suffixSampleIDMap[i] = originalSampleIDMap[suffixArray[i]];
//	}
//	free( originalSampleIDMap );


	//This call builds the LCP array.
//	memcpy( tempSuffixArray, suffixArray, dataArrayLength+1 );
	
	lcpArray = lcp_sa2lcp_9n( dataArray, dataArrayLength, suffixArray );
//	memcpy( suffixArray, tempSuffixArray, dataArrayLength+1 );
//	free( tempSuffixArray );

	runTokenExtraction();

	free( suffixArray );
	free( lcpArray );
//	free( suffixSampleIDMap );
	
	return TLhead;
}





