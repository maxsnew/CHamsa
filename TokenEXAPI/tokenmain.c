 /**************************************************************************
 *             N o r t h w e s t e r n     U n i v e r s i t y             *
 *                                                                         *
 * Hamsa - Polymorphic Worm Signature Generation Project -- Zhichun Li     *
 *                                                       -- Mananan Sanghi *
 *                                                       -- Brian Chavez   *
 *                                                       -- Yan Chen       *
 *                                                       -- Ming-Kang Kao  *
 *                                                                         *
 * FILE:     TokenVector.c                                                 *
 * DATE:     October 31, 2005                                              *
 * AUTHOR:   Brian Chavez, cowboy@cs.northwestern.edu                      *
 * PROJECT:  Hamsa                                                         *
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
 *                                                                         *
 * CLASS NAME: N/A                                                         *
 * CLASS DEPENDENCIES:  TokenVector - A C implementation that represents a *
 *                                    token vector.                        *
 *                                                                         *
 *                                                                         *
 * DESCRIPTION:  This is the implementation file that represents a token   *
 *               vector.                                                   *
 *                                                                         *
 * REVISION HISTORY:                                                       *
 *      Oct. 31, 2005 - Brian Chavez (cowboy@cs.northwestern.edu)          *
 *                    o Inital version                                     *
 *                                                                         *
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

#define FNAMEBUFSIZE	3000

/*****************************************************************************
 * get_filesize --
 *            Returns the file size of a given file.  This is generally
 *            used to figure out the size of the data that needs to be 
 *            sorted in a suffix array.  Data from the file that needs to be 
 *            sorted must be loaded in memory, therefore, we need this 
 *            function to determine the number of bytes we need in memory.
 *
 * @param char* fileName -- Pointer to a char[] that contains the file name.
 * @return long -- The number of bytes required to load this data file in
 *                 memory.
 */
long get_filesize(char *fileName){

	struct stat file;
	if( !stat(fileName, &file) ){
		return file.st_size;
	}
	return 0;
}

/*****************************************************************************
 * getNumberOfSamples() --
 *            This function is used to get the number of
 *            samples of suspicous flows.  This function reads a file called 
 *            'samples' in the current directory to determine the number of 
 *            flows.  This is defined on the first line of the file.
 * @param None
 * @return int -- The number of samples that are required to load in memory.
 */
int getNumberOfSamples(){
	FILE* sampleListFile;
	int numberOfSamples;

	sampleListFile = fopen( "samples", "r" );
	fscanf( sampleListFile, "%d\n", &numberOfSamples );
	fclose( sampleListFile );

	fprintf(stderr, "%s(): samples = %d\n", __FUNCTION__, numberOfSamples);

	return numberOfSamples;
}




/*****************************************************************************
 * loadSamplesDataIntoMemory() --
 *            Helpler function to simplify life and
 *            load data into memory in one shot.
 *
 * @param int numberOfSamples -- The number of samples you would like to
 *                               load.
 *          
 *        unsigned char* dataArray -- The traffic data array where 
 *                                    suspicious flows are stored in memory.
 *        
 *        int* sampleIndexArray -- 
 *                     Used to keep record of the flows in dataArray.
 *                     The index into this array is the sample #,
 *                     the value inside sampleIndexArray[i] is the
 *                     index into dataArray where this suspicous flow 
 *                     begins.
 *        char* dataArrayInputFileName --
 *                     Input file name of the dataArray (raw data).
 *        char* offsetArrayInputFilename --
 *                     Input file name of the offsetArray - A file that
 *                     contains binary integer data of offsets.
 * @return int -- 1 if successful load, 0 otherwise.
 */
int loadSamplesDataIntoMemory(/*IN*/int numberOfSamples, /*OUT*/unsigned char* dataArray, /*OUT*/int* sampleIndexArray, /*IN*/char* dataArrayInputFileName, /*IN*/char* offsetArrayInputFileName)
{
	FILE* dataInputFile;
	FILE* offsetInputFile;
	
	dataInputFile = fopen( dataArrayInputFileName, "r" );
	fread( dataArray, 1, get_filesize( dataArrayInputFileName ), dataInputFile );
	fclose( dataInputFile );
	offsetInputFile = fopen( offsetArrayInputFileName, "r" );
	fread( sampleIndexArray, sizeof(int), numberOfSamples, offsetInputFile );

	return 1;
}


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


int main(int argc, char* argv[])
{
	char datafname[FNAMEBUFSIZE];
	char offsetfname[FNAMEBUFSIZE];
	int i = 0;
	int dataLen;
	int numSamples;

	int overshoot;

	unsigned char* data;
	int* offsets;
	TokenVectorHndl tvHead, tvTemp;


	if( argc < 4 ){
		fprintf( stderr, "Error: Usage 'token <datadir> <min_token_len> <min_occ_ratio>'\n" );
		exit(-1);
	}
	
	snprintf(datafname,FNAMEBUFSIZE,"%s/data",argv[1]);
	snprintf(offsetfname,FNAMEBUFSIZE,"%s/offsets",argv[1]);

	unsigned int min_token_len=atoi(argv[2]);
	double min_occ_ratio=atof(argv[3]);	

	dataLen = get_filesize(datafname);
	numSamples = (int)(get_filesize(offsetfname) / sizeof(int)) ;

	overshoot = init_ds_ssort( 500, 2000 );

	data = (unsigned char*)calloc( dataLen+overshoot, sizeof(unsigned char) );
	offsets = (int*)calloc( numSamples + 1, sizeof(int) );

	if( !loadSamplesDataIntoMemory( numSamples, data, offsets, datafname, offsetfname ) ){
		fprintf(stderr, "%s: Error loading samples into memory.\n", __FUNCTION__);
		return -1;
	}

	offsets[numSamples] = dataLen;

//	fprintf(stderr, "DONE!\n");
	tvHead = TokenExtraction( data, dataLen, offsets, numSamples, min_token_len, (unsigned int)(numSamples*min_occ_ratio));

	
	tvTemp=tvHead;
	while( tvTemp != NULL ){
		//printf("TOKEN:'");
		printf("'");
		print_str( tvTemp->string, tvTemp->strLen );

		printf("'\t");
		//printf("TOKENVECTOR ");
		//for( i = 0; i < numSamples; i++ )
		//{
		//	if( tvTemp->occuranceVector[i] != 0 ){
		//		printf("%d:%d  ", i, tvTemp->occuranceVector[i] );
		//	}
		//}
		//printf("\n");

		tvTemp = tvTemp->next;
	}

	DeleteTokenVectorList(tvHead);

	return 0;
}


