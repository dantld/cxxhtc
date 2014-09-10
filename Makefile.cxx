## C++ High Tool Cat Kit Classes library
## HP Tru64 include
## $Id: Makefile.cxx 31 2009-09-17 04:40:00Z dantld $
##

CXX=cxx
LNK=cxx
CHTC_INC_PATH=..
CXXFLAGS=-pthread -I$(CHTC_INC_PATH) -g -D__USE_STD_IOSTREAM
SHVER=-set_version 
SHARED=-shared
LDFLAGS=-pthread -L/usr/shlib -lrt

OBJECTS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
HEADERS=$(wildcard *.h)
DEPENDS=$(patsubst %.cpp,%.d,$(wildcard *.cpp))

-include $(DEPENDS)

%.d : %.cpp
	@echo '>>>' Make depends for source $<
	@set -e; rm -f $@; \
	$(CXX) $(CXXFLAGS) -M $(INCLUDES) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
