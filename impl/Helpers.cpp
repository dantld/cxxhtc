/**
 * $Id: Helpers.cpp 109 2009-12-22 03:30:42Z dantld $
 * C++HTC helper functions collection.
 */
#include <time.h>
#include <stdlib.h>
#include "../cxxhtc_defs.h"

BEGIN_CXXHTC_NAMESPACE
BEGIN_CXXHTC_UTIL_NAMESPACE

struct timespec *calcTimeout( int microSec, struct timespec *abstime )
{
    if( microSec > 0 ) {
	int s(0), n(0);
	if( microSec >= 1e6 ) {
	    div_t qr = div( microSec, 1e6 );
	    s = qr.quot;
	    n = qr.rem;
	} else {
	    n = microSec * 1e6;
	}
	clock_gettime( CLOCK_REALTIME, abstime );
	if( abstime->tv_nsec + n >= 1e9 ) {
	    ldiv_t qr = ldiv( abstime->tv_nsec + n, 1e9 );
	    abstime->tv_sec  += qr.quot;
	    abstime->tv_nsec  = qr.rem;
	} else {
	    abstime->tv_nsec += n;
	}
    }
    return abstime;
}

END_CXXHTC_UTIL_NAMESPACE
END_CXXHTC_NAMESPACE
