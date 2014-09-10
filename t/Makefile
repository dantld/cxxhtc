## C++ High Tool Cat Kit Classes library
## $Id: Makefile 183 2011-03-18 09:17:24Z dantld $
##
.PHONY : clean


include ../Makefile.include

valgrind :
	LD_LIBRARY_PATH=../impl:/usr/local/lib valgrind -v --leak-check=full --show-reachable=yes --log-file=report.log ./t0000

CXXFLAGS := $(CXXFLAGS) -I/usr/local/include

LIBS=../impl/libc++htc.so -L/usr/local/lib -lcppunit -lm -lpthread -lrt
LDFLAGS:=$(LDFLAGS) $(CHECK_ALLOC)

all : t0000

depends : $(DEPENDS)

t0000 : Makefile $(HEADERS) $(OBJECTS)
	$(LNK) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

test : t0000
	LD_LIBRARY_PATH=../impl:/usr/local/lib ./t0000

test-leaks : t0000
	BATCH_RTC=on GDBRTC_CONFIG=./rtcconfig RTC_NO_ABORT=1 LD_PRELOAD=/opt/langtools/lib/hpux64/librtc.sl LD_LIBRARY_PATH=../impl ./t0000

clean :
	-rm t0000 $(OBJECTS) *.d *.d.* core

