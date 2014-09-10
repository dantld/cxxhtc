/**
 * $Id: SynchroImpl.cpp 109 2009-12-22 03:30:42Z dantld $
 */
#include "Synchro.h"
#include <pthread.h>
#include <string.h>

BEGIN_CXXHTC_NAMESPACE

const char* const ReadWriteSyncFactory::READ_WRITE_SYNC_FACTORY_NAME = "factory.object.readwritesync";

ReadWriteSync::~ReadWriteSync() {
}
ReadWriteSync::ReadWriteSync() {
}

class ReadWriteSyncImpl : public ReadWriteSync {
    pthread_rwlock_t _lock;
public:
    ReadWriteSyncImpl();
    ~ReadWriteSyncImpl();
    virtual void readLock() throw(SyncError);
    virtual void writeLock() throw(SyncError);
    virtual void unLock() throw(SyncError);
};

ReadWriteSyncImpl::ReadWriteSyncImpl() {
    int r = pthread_rwlock_init( &_lock, NULL );
    if( r != 0 ) {
	throw SyncError( r, strerror(r) );
    }
}
ReadWriteSyncImpl::~ReadWriteSyncImpl() {
    int r = pthread_rwlock_destroy( &_lock );
    if( r != 0 ) {
	throw SyncError( r, strerror(r) );
    }
}
void ReadWriteSyncImpl::readLock() throw(SyncError) {
    int r = pthread_rwlock_rdlock( &_lock );
    if( r != 0 ) {
	throw SyncError( r, strerror(r) );
    }
}
void ReadWriteSyncImpl::writeLock() throw(SyncError) {
    int r = pthread_rwlock_wrlock( &_lock );
    if( r != 0 ) {
	throw SyncError( r, strerror(r) );
    }
}
void ReadWriteSyncImpl::unLock() throw(SyncError) {
    int r = pthread_rwlock_unlock( &_lock );
    if( r != 0 ) {
	throw SyncError( r, strerror(r) );
    }
}


class ReadWriteSyncFactoryErrorImpl : public ReadWriteSyncFactoryError {
    std::string _message;
public:
    ReadWriteSyncFactoryErrorImpl( const char* message) throw();
    ~ReadWriteSyncFactoryErrorImpl() throw();
    const char* what() const throw();
};

ReadWriteSyncFactoryErrorImpl::ReadWriteSyncFactoryErrorImpl( const char* message ) throw() :
    _message(message)
{
}
ReadWriteSyncFactoryErrorImpl::~ReadWriteSyncFactoryErrorImpl() throw() {
}
const char* ReadWriteSyncFactoryErrorImpl::what() const throw() {
    return _message.c_str();
}


class ReadWriteSyncFactoryImpl : public ReadWriteSyncFactory {
public:
    ReadWriteSyncFactoryImpl() throw();
    virtual ~ReadWriteSyncFactoryImpl() throw();
    virtual ReadWriteSync* create( Config * ) throw(ReadWriteSyncFactoryError);
    virtual void destroy( ReadWriteSync* ) throw(ReadWriteSyncFactoryError);
};

ReadWriteSyncFactoryImpl::ReadWriteSyncFactoryImpl() throw() {
}
ReadWriteSyncFactoryImpl::~ReadWriteSyncFactoryImpl() throw() {
}
ReadWriteSync* ReadWriteSyncFactoryImpl::create( Config * ) throw(ReadWriteSyncFactoryError) {
    try {
	return new ReadWriteSyncImpl;
    } catch( const SyncError& e ) {
	throw ReadWriteSyncFactoryErrorImpl( e.what() );
    }
}
void ReadWriteSyncFactoryImpl::destroy( ReadWriteSync* instance ) throw(ReadWriteSyncFactoryError) {
    if( !dynamic_cast<ReadWriteSyncImpl*>(instance) ) {
	throw ReadWriteSyncFactoryErrorImpl("Bad implementation for this factory");
    }
    try {
	delete (ReadWriteSyncImpl*)instance;
    } catch( const SyncError& e ) {
	throw ReadWriteSyncFactoryErrorImpl( e.what() );
    }
}


ReadWriteSyncFactory* ReadWriteSync::getDefaultFactory() throw() {
    return new ReadWriteSyncFactoryImpl();
}

END_CXXHTC_NAMESPACE
