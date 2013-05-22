#include <fstream>
#include <iostream>

using namespace std;

#include "bitvec.h"

int main() {

	unsigned int bitsize=21;

	bit_vec *testv= new bit_vec(bitsize);

	testv->setbit(0); 
	testv->setbit(1); 
	testv->setbit(2); 
	testv->setbit(3); 
	testv->clearbit(3); 
	testv->clearbit(0); 
	testv->setbit(8); 
	testv->setbit(11); 
	testv->setbit(20); 

	for (unsigned int i=0; i<testv->bytelen; i++) 
		printf("%02X",testv->bvec[i]);
	printf("\n");

	cout << "# of 1: " << testv->cntone() <<endl;

	bit_vec *test2= new bit_vec(testv);
	test2->setbit(4);
	for (unsigned int i=0; i<test2->bytelen; i++) 
		printf("%02X",test2->bvec[i]);
	printf("\n");

	cout << "# of 1: " << test2->cntone() <<endl;

	bit_vec *test3= new bit_vec(bitsize);
	test3->setbit(1);
	test3->setbit(3);
	test3->setbit(5);
	test3->setbit(7);
	test3->setbit(9);
	test3->setbit(11);

	vec_and(test3,testv);	
	cout << "test 3 # of 1: " << test3->cntone() <<endl;

	

	return 0;
}

