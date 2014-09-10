## C++ High Tool Cat Kit Classes library
## $Id: Makefile 222 2011-11-21 04:39:23Z dantld $
##
.PHONY : clean

ifeq ($(ROOT_DIR),)
ROOT_DIR=../..
endif
include ../Makefile.conf
include ../Makefile.include
include ../Makefile.version

CXXFLAGS:=$(CXXFLAGS) $(CXXFLAGS_SHARED)

VERSION=$(CXXLIB_VERSION)
LIB_NAME=$(CXXLIB_NAME)
SHARED_LIB_NAME=$(CXXLIB_SHARED_LIB_NAME)
SHVER_NAME=$(CXXLIB_SHVER_NAME)

all : $(SHVER_NAME)

depends : $(DEPENDS)

$(SHVER_NAME) : Makefile $(HEADERS) $(OBJECTS)
	$(LNK) $(SHARED) $(LDFLAGS) $(SHVER)$(CXXLIB_SONAME) -o $@ $(OBJECTS)
	ln -sf $(SHVER_NAME) $(SHARED_LIB_NAME)


clean :
	-rm $(SHARED_LIB_NAME) $(SHVER_NAME) $(OBJECTS) *.d *.d.* core

