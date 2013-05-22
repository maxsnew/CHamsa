#!/bin/env python

"""
A python script used to show the signature stored in binary signature format
generated by CHamsa
"""

import sys
import os
import struct
import exceptions


def genfilename(prefix):
	sigdata= prefix + "_sigdata"
	sigoff= prefix + "_sigoffset"
	sigcnt= prefix + "_sigcount"

	return (sigdata,sigoff,sigcnt)

def testflist(flist):
	flag=True

	for fn in flist:
		flag = flag and os.path.exists(fn) and os.path.isfile(fn)
	
	return flag


def findallsigs(dir):
	no=1
	flag=True
	
	nprefix="SIG"

	while (flag):
		fname="%s/%s%d" % (dir,nprefix,no)
		fnamelist=genfilename(fname)
		if (not testflist(fnamelist)): break
		showsig(fnamelist,no)
		no+=1



def showsig(flist,no):

	print "-----------------Signature No %d ------------------------" % no
	dict=crtbin2dict(flist)
	print dict
	print


def crtbin2dict(flist):
	n= int(os.path.getsize(flist[1])/4)

	lenfile=open(flist[1])
	lenstream=lenfile.read()
	idx2len=[ (struct.unpack('I', lenstream[x*4:(x+1)*4])[0]) for x in range(n)]
	
	cntfile=open(flist[2])
	cntstream=cntfile.read()
	idx2cnt=[ (struct.unpack('I', cntstream[x*4:(x+1)*4])[0]) for x in range(n)]


	strfile=open(flist[0])
	strstream=strfile.read()
	pos=0
	dict={}
	for i in xrange(n):
		str=strstream[pos:pos+idx2len[i]]
		pos+=idx2len[i]
		dict[str]=int(idx2cnt[i])
		
	return dict	

if __name__ == "__main__":
	
	if (len(sys.argv)>=2):
		dir=sys.argv[1]
		
		if (not os.path.isdir(dir)):
			print "%s is not a vaid directory!" % dir
			sys.exit(1)
		
		findallsigs(dir)	

	else:
		print "Usage: %s <sigdir>: <sigdir> is the dir contains one or more signatures" % sys.argv[0]