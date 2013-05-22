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
 * FILE:     conjpattern.cpp 
 * DATE:     October 31, 2005                                              
 * AUTHOR:   Zhichun Li, lizc@cs.northwestern.edu                      
 * PROJECT:  Hamsa                                                         
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
#include <map>

#include <ctype.h>


#define 	__IMPL_CONJPATTERN
#include "bitvec.h"
#include "conjpattern.h"

using namespace std;

map<string,cacheitem_t*> cache;

static string gen_cachekey(string token, unsigned int cnt)
{
	char buf[20];
	bzero(buf,20);
	snprintf(buf,20,"%u",cnt);
	string together = token + buf;

	return together;
}

void flushcache()
{
	map<string,cacheitem_t*>::iterator miter;
	cacheitem_t *item;

	if (not Gtestcache) return;

	//cout << "CACHE size=" << cache.size() <<endl;
	//cout << "Flushing CACHE" <<endl;
	for (miter=cache.begin(); miter!=cache.end();)
	{
		item=miter->second;
		if (item!=NULL)
		{
			if (item->iage<Gage)
			{
				delete item->fpbvec;
				delete item;
				item=NULL;
				cache.erase(miter++);
			} else {
				++miter;
			}
		} else {
			cache.erase(miter++);
		}
		
	}
	//cout << "CACHE size=" << cache.size() <<endl;
}

static void print_str(string & str)
{
	unsigned int len=str.size();
	unsigned int i;
	unsigned char bc;

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
			printf("\\%c",str[i]);

		} else {
			bc=str[i];
			printf("\\x%02x",bc);
		}
	}
	
}

static unsigned int overlap_count(string & data, string & pattern)
{
	unsigned int count=0;
	string::size_type start=0;
	string::size_type pos;

	pos=data.find(pattern,start);

	while (pos!=string::npos)
	{
		count++;
		start=pos+1;
		pos=data.find(pattern,start);
	}

	return count;
}




Tokenobj::Tokenobj (string tokenstr, unsigned int tokencnt)
{
	token=tokenstr;
	cnt=tokencnt;
	covvec=NULL;
	fpvec=NULL;
	cov=0;
	fpos=0.0;
	score=0;

	
}

Tokenobj::~Tokenobj()
{
	if (covvec!=NULL) delete covvec;
	if (fpvec!=NULL) delete fpvec;
	
}

bool Tokenobj::isequal(Tokenobj* player)
{
	return ((player->token==token) && (player->cnt==cnt));
}

bool Tokenobj::issubset(Tokenobj* player)
{
	return ((player->token.find(token)!=string::npos) && (player->token.length()>token.length()) && (player->cnt>=cnt));
}
	
bool Tokenobj::issuperset(Tokenobj* player)
{
	return ((token.find(player->token)!=string::npos) && (token.length()>player->token.length()) && (cnt>=player->cnt));
}


unsigned int Tokenobj::length()
{
	return token.length();
}

/*
 *Implement cache layer and test whether total_sample has been calculated
 */
void Tokenobj::testfp(list <TraceSary*> & normallist, bool flush)
{
	unsigned int total=0;
	string cachekey;
	cacheitem_t * item;
	map<string,cacheitem_t*>::iterator pos;

	if (flush)
	{
		if (fpvec!=NULL) delete fpvec;
		total=caln_totalsample(normallist);
		fpvec=token_sigvector(normallist, total, token.c_str(), token.length(), cnt);
		fpos=(fpvec->cntone()*1.0)/fpvec->bitlen;
	} else if (fpvec==NULL)
	{

		//todo implement cache layer!!!!!!
		//todo use find instead of associative array
		if (Gtestcache && Gage!=0) {
			cachekey=gen_cachekey(token,cnt);
			pos=cache.find(cachekey);
			if (pos!=cache.end())
			{
				//cout << "CACHE HIT (" << token <<"," <<cnt <<")" <<endl;
				item=pos->second;
				if (item!=NULL)
				{
					//cout << "TRUE HIT" << endl;
					fpvec = new bit_vec(item->fpbvec);
					fpos = (fpvec->cntone()*1.0)/fpvec->bitlen;
					item->iage=Gage;
					return;
				}
			}

		}

		if (totalnormal!=0) {
			total=totalnormal;
		} else {
			total=caln_totalsample(normallist);
			totalnormal=total;
		}
				
		fpvec=token_sigvector(normallist, total, token.c_str(), token.length(), cnt);
		fpos=(fpvec->cntone()*1.0)/fpvec->bitlen;

		if (Gtestcache)
		{
			item=NULL;
			cachekey=gen_cachekey(token,cnt);
			pos=cache.find(cachekey);
			if (pos!=cache.end()) item=pos->second;
			
			if (item==NULL)
			{
				//cout << "UPDATE CACHE (" << token <<"," <<cnt <<")" <<endl;
				item= new cacheitem_t;
				item->iage=Gage;
				item->fpbvec = new bit_vec(fpvec);
				cache[cachekey]=item;
			}
			
		}
	}
	
}

void Tokenobj::cal_covvec(unsigned char *sv,unsigned int numsample)
{
	unsigned int i;
	covvec= new bit_vec(numsample);

	for (i=0;i<numsample;i++) {
		if (sv[i] >= cnt) {
			covvec->setbit(i);
		} else {
			covvec->clearbit(i);
		}
	}

	cov=covvec->cntone();

}


ConjPattern::ConjPattern ()
{
	tokens.clear();

	covvec= NULL;
	cov=0;
	fpvec= NULL;
	fpos=0;
	score=0.0;
}

ConjPattern::ConjPattern (ConjPattern *cjptn)
{
	Tokenobj tmtk("",0);
	vector<Tokenobj>::const_iterator vtiter;

	tokens.clear();
	for (vtiter=cjptn->tokens.begin(); vtiter!=cjptn->tokens.end(); ++vtiter)
	{
		tmtk.token=vtiter->token;
		tmtk.cnt=vtiter->cnt;
		tmtk.cov=vtiter->cov;
		tmtk.fpos=vtiter->fpos;
		tmtk.covvec=NULL;
		tmtk.fpvec=NULL;
		tokens.push_back(tmtk);
	}
	
	covvec= new bit_vec(cjptn->covvec);
	cov=cjptn->cov;
	fpvec= new bit_vec(cjptn->fpvec);
	fpos=cjptn->fpos;
	score=0.0;

}

ConjPattern::ConjPattern (ConjPattern *cjptn, Tokenobj *tk)
{
	Tokenobj tmtk("",0);
	vector<Tokenobj>::const_iterator vtiter;

	tokens.clear();
	for (vtiter=cjptn->tokens.begin(); vtiter!=cjptn->tokens.end(); ++vtiter)
	{
		tmtk.token=vtiter->token;
		tmtk.cnt=vtiter->cnt;
		tmtk.cov=vtiter->cov;
		tmtk.fpos=vtiter->fpos;
		tmtk.covvec=NULL;
		tmtk.fpvec=NULL;
		tokens.push_back(tmtk);
	}
	tmtk.token=tk->token;
	tmtk.cnt=tk->cnt;
	tmtk.cov=tk->cov;
	tmtk.fpos=tk->fpos;
	tmtk.covvec=NULL;
	tmtk.fpvec=NULL;
	tokens.push_back(tmtk);

	
	covvec= new bit_vec(cjptn->covvec);
	vec_and(covvec,tk->covvec);
	cov=covvec->cntone();
	fpvec= new bit_vec(cjptn->fpvec);
	vec_and(fpvec,tk->fpvec);
	fpos=(fpvec->cntone()*1.0)/fpvec->bitlen;
	score=0.0;

}

ConjPattern::ConjPattern (Tokenobj *tk)
{
	Tokenobj tmtk("",0);

	tmtk.token=tk->token;
	tmtk.cnt=tk->cnt;
	tmtk.cov=tk->cov;
	tmtk.fpos=tk->fpos;
	tmtk.covvec=NULL;
	tmtk.fpvec=NULL;
	tokens.push_back(tmtk);

	
	covvec= new bit_vec(tk->covvec);
	cov=tk->cov;
	fpvec= new bit_vec(tk->fpvec);
	fpos=tk->fpos;
	score=0.0;

}

ConjPattern::~ConjPattern()
{
	if (covvec!=NULL) delete covvec;
	if (fpvec!=NULL) delete fpvec; 
}




bool ConjPattern::istkinclude(Tokenobj *newtk)
{
	bool included=false;
	vector<Tokenobj>::iterator vtiter;

	for (vtiter=tokens.begin(); vtiter!=tokens.end(); ++vtiter)
	{
		if (vtiter->issuperset(newtk)) included=true;
		
	}

	return included;
}

unsigned int ConjPattern::testjoincov(Tokenobj *tk)
{
	bit_vec *tmpvec;
	

	tmpvec= new bit_vec(covvec);
	vec_and(tmpvec,tk->covvec);

	return tmpvec->cntone();
	
}

double ConjPattern::testjoinfp(Tokenobj *tk)
{
	bit_vec *tmpvec;
	double tmpfpos=0.0;

	tmpvec = new bit_vec(fpvec);
	vec_and(tmpvec,tk->fpvec);
	tmpfpos=(tmpvec->cntone()*1.0)/tmpvec->bitlen;

	return tmpfpos;

}

bool ConjPattern::match(string & data)
{
	bool matched=true;

	vector<Tokenobj>::iterator vtiter;

	for (vtiter=tokens.begin(); vtiter!=tokens.end(); ++vtiter)
	{
		if (overlap_count(data,vtiter->token)< vtiter->cnt) matched=false;
	}

	return matched;
}


void ConjPattern::printme()
{
	vector<Tokenobj>::iterator vtiter;
	unsigned int len,i;
	len=tokens.size();
	i=0;

	for (vtiter=tokens.begin(); vtiter!=tokens.end(); ++vtiter)
	{
		printf("'");
		print_str(vtiter->token);
		i++;
		if (i==len)
		{
			printf("': %d" ,vtiter->cnt);

		}else{
			printf("': %d, ",vtiter->cnt);
		}
		
	}
	printf("\n");

}

void ConjPattern::crtsaveformat(string fname)
{
	ofstream dataof,offsetof,countof;
	
	string datafname=fname + "_sigdata";
	string offsetfname= fname + "_sigoffset";
	string countfname= fname + "_sigcount";

	dataof.open(datafname.c_str(),ios::out | ios::binary);
	if (!dataof)
	{
		cerr << "Error open file" << datafname << "for save the signature!" <<endl;
		exit(1);
	}

	offsetof.open(offsetfname.c_str(),ios::out | ios::binary);
	if (!offsetof)
	{
		cerr << "Error open file" << offsetfname << "for save the signature!" <<endl;
		exit(1);
	}

	countof.open(countfname.c_str(),ios::out | ios::binary);
	if (!countof)
	{
		cerr << "Error open file" << countfname << "for save the signature!" <<endl;
		exit(1);
	}

	vector<Tokenobj>::const_iterator vtiter;
	unsigned int tmplen,tmpcnt;
	
	for (vtiter=tokens.begin(); vtiter!=tokens.end(); ++vtiter)
	{
		tmplen=vtiter->token.length();
		tmpcnt=vtiter->cnt;
		dataof.write((char*)vtiter->token.c_str(), tmplen);
		offsetof.write((char*)&tmplen,sizeof(tmplen));
		countof.write((char*)&tmpcnt,sizeof(tmpcnt));
	}

	dataof.close();
	offsetof.close();
	countof.close();

}



unsigned int ConjPattern::length()
{
	unsigned int length=0;

	vector<Tokenobj>::iterator vtiter;

	for (vtiter=tokens.begin(); vtiter!=tokens.end(); ++vtiter)
	{
		length+=vtiter->length();
	}

	return length;
}


