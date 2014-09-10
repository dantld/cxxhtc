/**
 * $Id: HelpMacro.h 215 2011-08-16 06:32:47Z dantld $
 * Helper error macros.
 */

#ifndef __KISC__CXX_HTC_LIBRARY___HELP_MACRO__H__
#define __KISC__CXX_HTC_LIBRARY___HELP_MACRO__H__
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdexcept>

/**
 * @defgroup helpers Вспомогательные классы, функции и макросы.
 */

/*@{*/

/**
 * Проверка возвращаемого значения библиотечных
 * функций pthread. Если возвращаемое значение
 * ненулевое, возбуждается ошибка std::runtime_error
 * с текстом описания ошибки и добавочным текстом,
 * заданным в макросе как параметр n.
 */
#define CHECK_PTHREAD_RETCODE(r,n) \
    if( r != 0 ) { \
	char errorMessageBuffer[512] = ""; \
	int n = snprintf( errorMessageBuffer, 512, \
        "%s:%d %s failed. Returns %d. %s", \
	__FILE__, __LINE__, \
        #n, \
        errno, \
        strerror(errno) ); \
	assert( n >= 0 ); \
	throw std::runtime_error( errorMessageBuffer ); \
   }

/**
 * Проверка возвращаемого значения стандартных
 * вызовов Unix. Если возвращаемое значение
 * ненулевое, возбуждается ошибка std::runtime_error
 * с текстом описания ошибки и добавочным текстом,
 * заданным в макросе как параметр n.
 */
#define CHECK_STDCALL_RETCODE(r,n) \
    if( r < 0 ) { \
	char errorMessageBuffer[512] = ""; \
	int n = snprintf( errorMessageBuffer, 512, \
        "%s:%d %s failed. Returns %d. %s", \
	__FILE__, __LINE__, \
        #n, \
        errno, \
        strerror(errno) ); \
	assert( n >= 0 ); \
	throw std::runtime_error( errorMessageBuffer ); \
   }


#define MYNEW(p,text) \
    p = new(std::nothrow) text; \
    if( p == NULL ) { \
	char errorMessageBuffer[512] = ""; \
	int n = snprintf( errorMessageBuffer, 512, \
    	    "%s:%d Failure to allocate storage", \
	    __FILE__, __LINE__ ); \
	assert( n >= 0 ); \
	throw std::runtime_error( errorMessageBuffer ); \
    }

/**
 * Help macro for generating the exception class.
 * B - the base class name
 * P - extension of the name of the inherited class
 */
#define CREATE_EXCEPTION_CLASS(B,P)\
    class B ## P : public B {\
	std::string _m;\
	B ## P() throw() {}\
    public:\
	B ## P(const char* m) throw() : _m(m) {} \
	~B ## P() throw() {} \
	const char* what() const throw() { return _m.c_str(); } \
    };


/**
 * Help macro for generating the default
 * task strategy implementation for simple
 * strategy tasks.
 */
#define CREATE_TASK_STRATEGY( P ) \
class P ## _Strategy : public CXXHTC::TaskStrategy {\
public: \
    P ## _Strategy( CXXHTC::ATask*, CXXHTC::Config* )  { } \
    virtual bool oneTime() throw() { \
	return true; \
    } \
    virtual bool isNeedToRun() throw() { \
	return true; \
    } \
    virtual bool isReadyToDestroy() throw() {\
	return true;\
    }\
};


/**
 * Help macro for generating factory for your tasks.
 * Constructor of your strategy task must have two parameters
 * pointers to task and config.
 * Macro also generate the create and destroy "C" calls for
 * using one in a dynamic loading.
 * @param NAMESPACE namespace of PRODUCT and STRATEGY class
 * @param PRODUCT the factory task product class name
 * @param STRATEGY the factory strategy product class name
 */
#define CREATE_TASK_FACTORY(NAMESPACE,PRODUCT,STRATEGY)\
namespace {\
    class PRODUCT ## _Factory : public CXXHTC_NAMESPACE::ATaskFactory {\
	CXXHTC_NAMESPACE::Config *_config;\
	public:\
	    PRODUCT ## _Factory() throw() {}\
	    ~PRODUCT ## _Factory() throw() {}\
	    CXXHTC_NAMESPACE::ATask* createTask(\
		const std::string &name,\
		CXXHTC_NAMESPACE::Config *config,\
		CXXHTC_NAMESPACE::TaskContext *taskContext )\
		    throw(CXXHTC_NAMESPACE::TaskFactoryException)\
    		try {\
		    _config = config;\
            	    return new NAMESPACE::PRODUCT( name, this, config, taskContext );\
	        } catch( const std::exception &e ) {\
		    throw CXXHTC_NAMESPACE::TaskFactoryException( e.what() );\
		}\
	    CXXHTC_NAMESPACE::TaskStrategy* createStrategy( CXXHTC_NAMESPACE::ATask *t ) \
		throw (CXXHTC_NAMESPACE::TaskFactoryException) {\
		NAMESPACE::STRATEGY *strategy = new NAMESPACE::STRATEGY( (NAMESPACE::PRODUCT*)t, _config );\
		return strategy;\
	    }\
	    void destroyTask( CXXHTC_NAMESPACE::ATask* o ) \
		throw(CXXHTC_NAMESPACE::TaskFactoryException)\
	    try {\
    		NAMESPACE::PRODUCT *t = dynamic_cast<NAMESPACE::PRODUCT*>(o);\
    		if( t == NULL ) throw std::bad_cast();\
    		delete t;\
	    } catch(const std::exception &e ) {\
		throw CXXHTC_NAMESPACE::TaskFactoryException( e.what() );\
	    }\
    };\
}\
static PRODUCT ## _Factory* _factory_ = NULL;\
extern "C" void * create() {\
    if( _factory_ == NULL ) {\
	_factory_ = new PRODUCT ## _Factory();\
    }\
    return _factory_;\
}\
extern "C" void destroy( void* o ) {\
    if( _factory_ == NULL ) {\
        throw std::runtime_error( "#PRODUCT ## _Factory not allocated yet" );\
    }\
    if( _factory_ != o ) {\
        throw std::bad_cast();\
    }\
    delete _factory_;\
    _factory_ = NULL;\
}

/**
 * Help macro for generating factory for your singleton tasks.
 * Constructor of your strategy task must have two parameters
 * pointers to task and config.
 * Macro also generate the create and destroy "C" calls for
 * using one in a dynamic loading.
 * We suppose that this factory used in a synchronized environment.
 * @param NAMESPACE namespace of PRODUCT and STRATEGY class
 * @param PRODUCT the factory task product class name
 * @param STRATEGY the factory strategy product class name
 */
#define CREATE_SINGLETON_TASK_FACTORY(NAMESPACE,PRODUCT,STRATEGY)\
namespace { \
    class PRODUCT ## _Factory : public CXXHTC_NAMESPACE::ATaskFactory {\
	CXXHTC_NAMESPACE::Config *_config;\
	static NAMESPACE::PRODUCT *_product;\
	public:\
	    PRODUCT ## _Factory() throw() {}\
	    ~PRODUCT ## _Factory() throw() {}\
	    CXXHTC_NAMESPACE::ATask* createTask(\
		const std::string &name,\
		CXXHTC_NAMESPACE::Config *config,\
		CXXHTC_NAMESPACE::TaskContext *taskContext )\
		    throw(CXXHTC_NAMESPACE::TaskFactoryException)\
    		try {\
		    if( _product == NULL ) {\
			_config = config;\
            		_product = new NAMESPACE::PRODUCT( name, this, config, taskContext );\
		    }\
		    return _product;\
	        } catch( const std::exception &e ) {\
		    throw CXXHTC_NAMESPACE::TaskFactoryException( e.what() );\
		}\
	    CXXHTC_NAMESPACE::TaskStrategy* createStrategy( CXXHTC_NAMESPACE::ATask *t ) \
		throw (CXXHTC_NAMESPACE::TaskFactoryException) {\
		NAMESPACE::STRATEGY *strategy = new NAMESPACE::STRATEGY( (NAMESPACE::PRODUCT*)t, _config );\
		return strategy;\
	    }\
	    void destroyTask( CXXHTC_NAMESPACE::ATask* o ) \
		throw(CXXHTC_NAMESPACE::TaskFactoryException)\
	    try {\
    		NAMESPACE::PRODUCT *t = dynamic_cast<NAMESPACE::PRODUCT*>(o);\
    		if( _product == NULL ) throw std::runtime_error("Task instance not allocated yet");\
    		if( _product != t ) throw std::runtime_error("Task instance must be the same");\
		_product = NULL;\
    		delete t;\
	    } catch(const std::exception &e ) {\
		throw CXXHTC_NAMESPACE::TaskFactoryException( e.what() );\
	    }\
    };\
}\
static PRODUCT ## _Factory* _factory_ = NULL;\
NAMESPACE::PRODUCT *PRODUCT ## _Factory :: _product = NULL;\
extern "C" void * create() {\
    if( _factory_ == NULL ) {\
	_factory_ = new PRODUCT ## _Factory();\
    }\
    return _factory_;\
}\
extern "C" void destroy( void* o ) {\
    if( _factory_ == NULL ) {\
        throw std::runtime_error( "#PRODUCT ## _Factory not allocated yet" );\
    }\
    if( _factory_ != o ) {\
        throw std::bad_cast();\
    }\
    delete _factory_;\
    _factory_ = NULL;\
}

/*@}*/

#endif // __KISC__CXX_HTC_LIBRARY___HELP_MACRO__H__
