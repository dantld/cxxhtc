/**
 * $Id: SmartPointersTest.cpp 213 2011-08-11 10:05:35Z dantld $
 *
 */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../SmartPointers.h"
#include <queue>
#include <errno.h>
#include <string.h>

#ifdef _REENTRANT
#include <pthread.h>
#endif

#define STD_CALL(stmt) {\
    errno = 0;\
    if( (stmt) == -1 ) {\
	std::ostringstream os;\
	os << "Call: \"" << #stmt "\" failed: (" << errno << "), "<< strerror(errno) << std::ends;\
	throw std::runtime_error( os.str().c_str() );\
    }\
}

const int MAX_ITER = 1000;

class DestructorException : public std::exception
{
public:
    const char* what() const throw() { return "dtor processed"; }
};

class TestAutoPtrClass 
{
public:
    TestAutoPtrClass() {
    }
    ~TestAutoPtrClass() throw(DestructorException) {
	throw DestructorException();
    }
};

class SimpleTestObject : public kisc::Obj
{
    char _buffer[0x4000];
public:
    typedef kisc::SmartPtr<SimpleTestObject> Ptr;
    SimpleTestObject() {
    }
    ~SimpleTestObject() throw(DestructorException) {
	throw DestructorException();
    }
};


class SmartPtr_TestObject_1 : public kisc::Obj
{
    int _id;
    char _buffer[0x4000];
    static int _instCnt;
    static pthread_mutex_t _instCntLck;
    SmartPtr_TestObject_1() {}
public:
    typedef kisc::SmartPtr<SmartPtr_TestObject_1> Ptr;
    SmartPtr_TestObject_1(int id) : _id(id) {
	#ifdef _REENTRANT
	STD_CALL(pthread_mutex_lock( &_instCntLck ));
	#endif
	_instCnt++;
	#ifdef _REENTRANT
	STD_CALL(pthread_mutex_unlock( &_instCntLck ));
	#endif
    }
    ~SmartPtr_TestObject_1() {
	#ifdef _REENTRANT
	STD_CALL(pthread_mutex_lock( &_instCntLck ));
	#endif
	_instCnt--;
	#ifdef _REENTRANT
	STD_CALL(pthread_mutex_unlock( &_instCntLck ));
	#endif
    }
    inline int getId() const throw() { return _id; }
    static int getInstCnt() throw() { return _instCnt; }
};

int SmartPtr_TestObject_1::_instCnt = 0;
pthread_mutex_t SmartPtr_TestObject_1::_instCntLck = PTHREAD_MUTEX_INITIALIZER;

class TestQueue {
    std::queue<SmartPtr_TestObject_1::Ptr> _queue;
    bool _stop;
    #ifdef _REENTRANT
    pthread_mutex_t _mutex;
    pthread_cond_t  _cond;
    #endif
public:
    TestQueue() : _stop(false)
    {
	#ifdef _REENTRANT
	pthread_mutex_init( &_mutex, NULL );
	pthread_cond_init(  &_cond, NULL );
	#endif
    }
    ~TestQueue() 
    {
	#ifdef _REENTRANT
	pthread_cond_destroy( &_cond );
	pthread_mutex_destroy( &_mutex );
	#endif
    }
    void push( SmartPtr_TestObject_1::Ptr o ) {
	#ifdef _REENTRANT
	pthread_mutex_lock( &_mutex );
	#endif
	if( _stop ) {
	    #ifdef _REENTRANT
	    pthread_mutex_unlock( &_mutex );
	    #endif
	    throw std::runtime_error("queue is stopped");
	}
	_queue.push( o );
	#ifdef _REENTRANT
	pthread_mutex_unlock( &_mutex );
	pthread_cond_signal( &_cond );
	#endif
    }
    SmartPtr_TestObject_1::Ptr pop() throw(std::range_error,std::runtime_error) {
	#ifdef _REENTRANT
	pthread_mutex_lock( &_mutex );
	#endif
	while( _queue.empty() ) {
            struct timespec tmd;
	    clock_gettime( CLOCK_REALTIME, &tmd );
	    tmd.tv_nsec += long(100e6);
	    #ifdef _REENTRANT
	    int r = pthread_cond_timedwait( &_cond, &_mutex, &tmd );
	    #endif
	    if( r == ETIMEDOUT ) {
		if( _stop ) {
		    #ifdef _REENTRANT
		    pthread_mutex_unlock( &_mutex );
		    #endif
		    throw std::runtime_error("queue is stopped");
		}
	    }
	}
	SmartPtr_TestObject_1::Ptr o = _queue.front();
	_queue.pop();
	#ifdef _REENTRANT
	pthread_mutex_unlock( &_mutex );
	#endif
	return o;
    }
    void stopQueue() {
	#ifdef _REENTRANT
	pthread_mutex_lock( &_mutex );
	#endif
	_stop = true;
	#ifdef _REENTRANT
	pthread_mutex_unlock( &_mutex );
	pthread_cond_broadcast( &_cond );
	#endif
    }
};

#ifdef _REENTRANT
void* thread_routine_1( void *arg ) {
    TestQueue *queue = (TestQueue*)arg;
    int i;
    for( i = 0; i < MAX_ITER; i++ ) {
	SmartPtr_TestObject_1::Ptr o( new SmartPtr_TestObject_1(i) );
	queue->push( o );
    }
    queue->stopQueue();
    return (void*)i;
}
void* thread_routine_2( void *arg ) {
    TestQueue *queue = (TestQueue*)arg;
    int i = 0;
    try {
	while( 1 ) {
	    SmartPtr_TestObject_1::Ptr o = queue->pop();
	    CPPUNIT_ASSERT( o->getId() == i );
	    i++;
	}
    } catch( const std::exception& e ) {
    }
    return (void*)i;
}
#endif

class SmartPointersTest : public CppUnit::TestFixture {
#ifdef _REENTRANT
    pthread_t _t1;
    pthread_t _t2;
#endif
    TestQueue _queue;
public:
    SmartPointersTest() {}
    void setUp();
    void test_AutoPtr();
    void test_SmartPtr();
#ifdef _REENTRANT
    void test_SmartPtr_Multithread();
#endif
    void tearDown();

    CPPUNIT_TEST_SUITE( SmartPointersTest );
	CPPUNIT_TEST_EXCEPTION(test_AutoPtr,DestructorException);
	CPPUNIT_TEST_EXCEPTION(test_SmartPtr,DestructorException);
#ifdef _REENTRANT
	CPPUNIT_TEST(test_SmartPtr_Multithread);
#endif
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( SmartPointersTest );

void SmartPointersTest::setUp() {
}
void SmartPointersTest::test_AutoPtr() {
    kisc::AutoPtr<TestAutoPtrClass> ptr( new TestAutoPtrClass );
}
void SmartPointersTest::test_SmartPtr() {
    SimpleTestObject::Ptr ptr1( new SimpleTestObject );
    try {
	SimpleTestObject::Ptr ptr2 = ptr1;
    } catch( const DestructorException& ) {
	std::cerr << "Destruction exception has caught" << std::endl;
    }
    SimpleTestObject::Ptr ptr3 = ptr1;
}

#ifdef _REENTRANT
void SmartPointersTest::test_SmartPtr_Multithread() {
    int r = pthread_create( &_t1, NULL, thread_routine_1, &_queue );
    if( r < 0 ) throw std::runtime_error("pthread_create failed");
    r = pthread_create( &_t2, NULL, thread_routine_2, &_queue );
    if( r < 0 ) throw std::runtime_error("pthread_create failed");
    void *retVal;
    r = pthread_join( _t1, &retVal );
    if( r < 0 ) throw std::runtime_error("pthread_join 1 failed");
    CPPUNIT_ASSERT( (int*)MAX_ITER == (int*)retVal );
    r = pthread_join( _t2, &retVal );
    if( r < 0 ) throw std::runtime_error("pthread_join 2 failed");
    CPPUNIT_ASSERT( (int*)MAX_ITER == (int*)retVal );
    CPPUNIT_ASSERT( SmartPtr_TestObject_1::getInstCnt() == 0 );
}
#endif

void SmartPointersTest::tearDown() {
}
