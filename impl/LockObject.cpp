/**
 * Howto operate with locking resources.
 */


#include "LockObject.h"
#include <pthread.h>
#include <sstream>
#include <string.h>

BEGIN_CXXHTC_NAMESPACE

class LockObjectErrorImpl : public LockObjectError {
    std::string _message;
public:
    LockObjectErrorImpl( const char *message) throw() : _message(message) {}
    LockObjectErrorImpl( const char *message, int) throw() : _message(message) {}
    ~LockObjectErrorImpl() throw() {}
    const char* what() const throw() { return _message.c_str(); }
};

LockObject::LockObject() {
}

LockObject::~LockObject() {
}

class LockObjectImpl : public LockObject {
    pthread_mutex_t _mutex;
    pthread_cond_t _event;
    Predicate *_predicate;
    bool _exit;
public:
    LockObjectImpl( Predicate * );
    virtual ~LockObjectImpl();
    virtual void requireLock() throw(LockObjectError);
    virtual void acquireLock() throw(LockObjectError);
    virtual void releaseLock() throw(LockObjectError);
    virtual void wakeupAll() throw(LockObjectError);
};

LockObjectImpl::LockObjectImpl( Predicate *predicate ) :
    _predicate(predicate), _exit(false)

{
    pthread_mutex_init( &_mutex, NULL );
    pthread_cond_init( &_event, NULL );
}

LockObjectImpl::~LockObjectImpl() {
    pthread_cond_destroy( &_event );
    pthread_mutex_destroy( &_mutex );
}

void LockObjectImpl::requireLock() throw(LockObjectError) {
    int r = pthread_mutex_lock( &_mutex );
	if( r != 0 ) {
		std::ostringstream os;
		os << "requireLock:pthread_mutex_lock return code" << r << ", Error:" << strerror(r);
		throw LockObjectErrorImpl( os.str().c_str() );
	}
}

void LockObjectImpl::acquireLock() throw(LockObjectError) {
    int r = pthread_mutex_lock( &_mutex );
	if( r != 0 ) {
		std::ostringstream os;
		os << "acquireLock:pthread_mutex_lock return code" << r << ", Error:" << strerror(r);
		throw LockObjectErrorImpl( os.str().c_str() );
	}
    if( _predicate->getValue() ) {
	return;
    }
    do {
		r = pthread_cond_wait( &_event, &_mutex );
		if( r != 0 ) {
			std::ostringstream os;
			os << "acquireLock:pthread_cond_wait return code" << r << ", Error:" << strerror(r);
			throw LockObjectErrorImpl( os.str().c_str() );
		}
    } while( !_predicate->getValue() && !_exit );
    if( _exit ) {
		throw LockObjectErrorImpl("acquireLock: process encountered with exit event");
    }
}


void LockObjectImpl::releaseLock() throw(LockObjectError) {
	int r = pthread_mutex_unlock( &_mutex );
	if( r != 0 ) {
		std::ostringstream os;
		os << "releaseLock:pthread_mutex_unlock return code" << r << ", Error:" << strerror(r);
		throw LockObjectErrorImpl( os.str().c_str() );
	}
    r = pthread_cond_signal( &_event );
	if( r != 0 ) {
		std::ostringstream os;
		os << "releaseLock:pthread_cond_signal return code" << r << ", Error:" << strerror(r);
		throw LockObjectErrorImpl( os.str().c_str() );
	}
}

void LockObjectImpl::wakeupAll() throw(LockObjectError) {
    int r = pthread_mutex_lock( &_mutex );
	if( r != 0 ) {
		std::ostringstream os;
		os << "wakeupAll:pthread_mutex_lock return code" << r << ", Error:" << strerror(r);
		throw LockObjectErrorImpl( os.str().c_str() );
	}
    _exit = true;
    r = pthread_mutex_unlock( &_mutex );
	if( r != 0 ) {
		std::ostringstream os;
		os << "wakeupAll:pthread_mutex_unlock return code" << r << ", Error:" << strerror(r);
		throw LockObjectErrorImpl( os.str().c_str() );
	}
    r = pthread_cond_broadcast( &_event );
	if( r != 0 ) {
		std::ostringstream os;
		os << "wakeupAll:pthread_cond_broadcast return code" << r << ", Error:" << strerror(r);
		throw LockObjectErrorImpl( os.str().c_str() );
	}
}

LockObject *LockObject::create( Predicate* predicate ) throw() {
    return new LockObjectImpl( predicate );
}

void LockObject::destroy( LockObject *lockobj) throw() {
    delete lockobj;
}


END_CXXHTC_NAMESPACE
