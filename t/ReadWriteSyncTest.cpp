/**
 * $Id: ReadWriteSyncTest.cpp 14 2009-09-02 09:31:31Z dantld $
 *
 */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../Synchro.h"
#include <pthread.h>
#include <signal.h>

using namespace kisc;
static const char* const TEST_CHAR_DATA = "HELLO WORLD!!! THIS IS A Test Data.";
static std::string my_read_write_object = TEST_CHAR_DATA;
static const int MAX_TESTS = 10000;
static const int MAX_READERS = 10;
static std::string writer_error_message;

struct ReaderContext {
    ReadWriteSync *rwSync;
    std::string    reader_error_message;
    ReaderContext() : rwSync( NULL ) {}
};

ReaderContext rcs[MAX_READERS];

static void *thread_proc_reader( void* arg ) {
    ReaderContext *rc = (ReaderContext*)arg;
    ReadWriteSync *rwSync = rc->rwSync;
    sigset_t sigSet;
    sigemptyset( &sigSet );
    sigaddset( &sigSet, SIGHUP );
    pthread_sigmask( SIG_SETMASK, &sigSet, NULL );
    int signal = 0;
    sigwait( &sigSet, &signal );
    try {
	for( int i = 0; i < MAX_TESTS; i++ ) {
	    {
	    util::AutoReadLock<ReadWriteSync> lock( rwSync );
	    if( my_read_write_object.compare( TEST_CHAR_DATA ) != 0 ) {
		rc->reader_error_message = "Data comparing failed.";
		break;
	    }
	    }
	}
    } catch( const SyncError& e ) {
	rc->reader_error_message = e.what();
    }
    return NULL;
}

static void *thread_proc_writer( void* arg ) {
    ReadWriteSync *rwSync = (ReadWriteSync*)arg;
    sigset_t sigSet;
    sigemptyset( &sigSet );
    sigaddset( &sigSet, SIGHUP );
    pthread_sigmask( SIG_SETMASK, &sigSet, NULL );
    int signal = 0, retVal = 0;
    sigwait( &sigSet, &signal );
    try {
	for( int i = 0; i < MAX_TESTS; i++ ) {
	    {
	    util::AutoWriteLock<ReadWriteSync> lock( rwSync );
	    my_read_write_object.assign( "BAD DATA FOR SOME TIMES..." );
	    my_read_write_object.assign( TEST_CHAR_DATA );
	    }
	}
    } catch( const SyncError& e ) {
	writer_error_message = e.what();
    }
    return NULL;
}

class ReadWriteSyncTest : public CppUnit::TestFixture {
    std::string _name;
    pthread_t _readers[MAX_READERS];
    pthread_t _writer;
    ReadWriteSync *_rwSync;
public:
    ReadWriteSyncTest() { }
    void setUp();
    void test01();
    void tearDown();

    CPPUNIT_TEST_SUITE( ReadWriteSyncTest );
	CPPUNIT_TEST(test01);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( ReadWriteSyncTest );

void ReadWriteSyncTest::setUp() {
    ReadWriteSyncFactory *factory = ReadWriteSync::getDefaultFactory();
    _rwSync = factory->create( NULL );
    delete factory;
    pthread_create( &_writer, NULL, thread_proc_writer, _rwSync );
    for( int i = 0; i < MAX_READERS; i++ ) {
	rcs[i].rwSync = _rwSync;
	pthread_create( _readers + i, NULL, thread_proc_reader, rcs + i );
    }
}
void ReadWriteSyncTest::test01() {
    sleep(1);
    pthread_kill( _writer, SIGHUP );
    for( int i = 0; i < MAX_READERS; i++ ) {
	pthread_kill( _readers[i], SIGHUP );
    }
    pthread_join( _writer, NULL );
    for( int i = 0; i < MAX_READERS; i++ ) {
	pthread_join( _readers[i], NULL );
    }
    for( int i = 0; i < MAX_READERS; i++ ) {
	if( rcs[i].reader_error_message.length() != 0 ) {
	    std::cerr << "Reader error: " << rcs[i].reader_error_message << std::endl;
	}
    }
    if( writer_error_message.length() != 0 ) {
	std::cerr << "Writer error: " << writer_error_message << std::endl;
    }
    for( int i = 0; i < MAX_READERS; i++ ) {
	CPPUNIT_ASSERT( rcs[i].reader_error_message.length() == 0 );
    }
    CPPUNIT_ASSERT( writer_error_message.length() == 0 );
}
void ReadWriteSyncTest::tearDown() {
    try {
	ReadWriteSyncFactory *factory = ReadWriteSync::getDefaultFactory();
	factory->destroy( _rwSync );
	delete factory;
    } catch( const std::exception e ) {
	std::cerr << e.what() << std::endl;
    }
}


