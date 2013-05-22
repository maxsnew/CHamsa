/**************************************************************************
 *             N o r t h w e s t e r n     U n i v e r s i t y             *
 *                                                                         *
 * Hamsa - Polymorphic Worm Signature Generation Project -- Zhichun Li     *
 *                                                       -- Mananan Sanghi *
 *                                                       -- Brian Chavez   *
 *                                                       -- Yan Chen       *
 *                                                       -- Ming-Kang Kao  *
 *                                                                         *
 * FILE:     TokenVector.h                                                 *
 * DATE:     October 31, 2005                                              *
 * AUTHOR:   Brian Chavez, cowboy@cs.northwestern.edu                      *
 * PROJECT:  Hamsa                                                         *
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
 *                                                                         *
 * CLASS NAME: N/A                                                         *
 * CLASS DEPENDENCIES:  None                                               *
 *                                                                         *
 *                                                                         *
 * DESCRIPTION:  This is the header file for the TokenVector               *
 *               implementation.                                           *
 *                                                                         *
 * REVISION HISTORY:                                                       *
 *      Oct. 31, 2005 - Brian Chavez (cowboy@cs.northwestern.edu)          *
 *                    o Inital version                                     *
 *                                                                         *
 **************************************************************************/

#ifndef _TOKENVECTOR_H
#define _TOKENVECTOR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//TokenVector
typedef struct TokenVector{
	unsigned char* occuranceVector;
	char* string;
	unsigned int strLen;
	struct TokenVector* next;
}TokenVector;

typedef struct TokenVector* TokenVectorHndl;

TokenVectorHndl NewTokenVector();
void DeleteTokenVectorList(TokenVectorHndl tvhead );

TokenVectorHndl TokenExtraction(char* data, unsigned int datasize, unsigned int* offset, unsigned int numsample, unsigned int min_token_len, unsigned int min_token_occ );
#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _TOKENVECTOR_H */


