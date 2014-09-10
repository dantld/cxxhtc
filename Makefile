## C++ High Tool Cat Kit Classes library
## $Id: Makefile 167 2010-04-19 09:08:03Z dantld $
##

.PHONY : clean all test

ifeq ($(ROOT_DIR),)
ROOT_DIR=../..
endif

all :
	gmake -C impl ROOT_DIR=$(ROOT_DIR) all
	gmake -C t ROOT_DIR=$(ROOT_DIR) all

depends :
	gmake -C impl ROOT_DIR=$(ROOT_DIR) depends
	gmake -C t ROOT_DIR=$(ROOT_DIR) depends

test :
	gmake -C t ROOT_DIR=$(ROOT_DIR) test

clean :
	gmake -C impl clean
	gmake -C t clean

