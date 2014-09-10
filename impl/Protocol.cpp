/**
 * Message logger
 * $Id: Protocol.cpp 216 2011-08-18 10:54:36Z dantld $
 */

#include "Protocol.h" 

#include <stdexcept>
#include "DynamicLoader.h"
#include <stdarg.h>
#include <stdio.h>
#include "Config.h"

using namespace std;
BEGIN_CXXHTC_NAMESPACE

namespace internal {
class ProtocolImpl : public Protocol {
    int dummy_data;
    void logva( const char *fmt, va_list args ) throw() {
	vprintf( fmt, args );
	printf( " [WARNING: used default protocol implementation.]\n" );
    }
public:
    ProtocolImpl() {}
    virtual ~ProtocolImpl() throw() {}
    virtual void log(const char* fmt, ...) throw() {
	va_list args;
	va_start( args, fmt );
	logva( fmt, args );
	va_end( args );
    }
    virtual void logError(const char* source, const char *fmt, ...) throw() {
	va_list args;
	va_start( args, fmt );
	logva( fmt, args );
	va_end( args );
    }
    virtual void logWarning(const char* source, const char *fmt, ...) throw() {
	va_list args;
	va_start( args, fmt );
	logva( fmt, args );
	va_end( args );
    }
    virtual void logInfo(const char* source, const char *fmt, ...) throw() {
	va_list args;
	va_start( args, fmt );
	logva( fmt, args );
	va_end( args );
    }
    virtual void logDebug(const char* source, int level, const char *fmt, ...) throw() {
	va_list args;
	va_start( args, fmt );
	logva( fmt, args );
	va_end( args );
    }
};
}

static AbstractFactory::Loader *protocolLoader = 0;
static AbstractFactory         *protocolFactory = 0;
static internal::ProtocolImpl statProtocol;
Protocol *protocol = &statProtocol;

void Protocol::create( const Config* config ) {
    if( protocolLoader == 0 )
	protocolLoader = new AbstractFactory::Loader( config->getProperty( "factory.object.protocol" ).c_str() );
    if( protocolFactory == 0 )
	protocolFactory = protocolLoader->create();
    if( protocol != 0 && dynamic_cast<internal::ProtocolImpl*>(protocol) != 0 ) {
//	delete (internal::ProtocolImpl*)protocol;
	protocol = 0;
    }
    if( protocol == 0 ) {
	try {
	    protocol = (Protocol*)
		protocolFactory->createInstance( 
		    "protocol", 
		    const_cast<Config*>(config) 
		    );
	} catch( const exception &e ) {
	    protocol = new internal::ProtocolImpl;
	    throw e;
	}
    }
}
void Protocol::destroy() {
    if( protocol != 0 ) {
	protocolFactory->destroyInstance( protocol );
	protocol = new internal::ProtocolImpl;
    } 
    if( protocolLoader != 0 ) {
	delete protocolLoader;
	protocolLoader = 0;
    }
}

Protocol::Protocol() {}
Protocol::~Protocol() throw() {}


END_CXXHTC_NAMESPACE

