/**
 * $Id: Monitor.cpp 109 2009-12-22 03:30:42Z dantld $
 */

#include "../Monitor.h"

BEGIN_CXXHTC_NAMESPACE

const char* Monitor::getStateName( Monitor::States state ) throw()
{
    switch( state ) {
	case MONST_NORMAL:	return "NORMAL";
	case MONST_WARNING:	return "WARNING";
	case MONST_ERROR:	return "ERROR";
        case MONST_UNKNOWN:	return "UNKNOWN";
	default:		return "UNKNOWN";
    }
}

MonitorError::MonitorError() throw() {
}

MonitorError::~MonitorError() throw() {
}

MonitorClient::MonitorClient() throw() {
}

MonitorClient::~MonitorClient() throw() {
}

MonitorServer::MonitorServer() throw() {
}

MonitorServer::~MonitorServer() throw() {
}

MonitorStateListener::MonitorStateListener() throw() {
}

MonitorStateListener::~MonitorStateListener() throw() {
}

END_CXXHTC_NAMESPACE
