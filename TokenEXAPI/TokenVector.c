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
#include "TokenVector.h"




TokenVectorHndl NewTokenVector(int numberOfSamples){
	TokenVectorHndl hndl;
	int i = 0;
	hndl = (TokenVector*)malloc(sizeof(TokenVector));
	
	hndl->occuranceVector = (unsigned char*)calloc( numberOfSamples, sizeof(unsigned char));

	for( i = 0; i < numberOfSamples; i++ ){
		hndl->occuranceVector[i] = 0;
	}

	hndl->next = NULL;
	hndl->strLen = 0;
	hndl->string = NULL;

	return hndl;
}

void DeleteTokenVectorList(TokenVectorHndl tvhead ) {
	TokenVectorHndl tvtemp;

	while (tvhead !=NULL) {
		tvtemp=tvhead;
		tvhead=tvhead->next;
		free(tvtemp->string);
		free(tvtemp->occuranceVector);
		free(tvtemp);
	}

}	
