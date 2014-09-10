/**
 * Task adapters and other task support classes definitions
 *  $Id: Task.cpp 213 2011-08-11 10:05:35Z dantld $
 */
   
#include "Task.h"
#include <algorithm>
#if defined(_REENTRANT)
#include <pthread.h>
#endif
#include <limits.h>
#include <time.h>
#include <assert.h>
#include <HelpMacro.h> 
#include <Protocol.h> 
#include <string> 
#include <stdio.h> 

#define LOGSRC "task.h" 

BEGIN_CXXHTC_NAMESPACE

ThreadContext::ThreadContext() throw() {}
ThreadContext::~ThreadContext() throw() {}

TaskListener::TaskListener() {}
TaskListener::~TaskListener() {}

TaskStrategy::TaskStrategy() {}
TaskStrategy::~TaskStrategy() {}

ATaskFactory::ATaskFactory() {}
ATaskFactory::~ATaskFactory() throw() {}
namespace {
class AbstractFactoryErrorImpl : public AbstractFactoryError {
    std::string _m;
public:
    AbstractFactoryErrorImpl( const char* m ) throw() : _m(m) {}
    ~AbstractFactoryErrorImpl() throw() {}
    const char* what() const throw() { return _m.c_str(); }
};
}
void* ATaskFactory::createInstance( const char*, Config* ) throw(AbstractFactoryError)
{
    throw AbstractFactoryErrorImpl("Not usabel methods");
    return 0;
}
void  ATaskFactory::destroyInstance( void* ) throw(AbstractFactoryError)
{
}

TaskFactoryException::TaskFactoryException() throw() :
    _m("Unnamed exception")
{
}
TaskFactoryException::TaskFactoryException( const char* m ) throw() :
    _m(m)
{
}
TaskFactoryException::~TaskFactoryException() throw()
{
}
const char* TaskFactoryException::what() const throw()
{
    return _m.c_str();
}


#if defined(_REENTRANT)
static pthread_mutex_t task_id_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
static unsigned long long task_id = 0;
ATask::ATask() {
    try {
#if defined(_REENTRANT)
	int r = pthread_mutex_lock( &task_id_mutex );
	CHECK_PTHREAD_RETCODE(r,pthread_mutex_lock);
#endif
        task_id++;
#if defined(__HP_aCC) && defined(__ia64)
        if( task_id == ULONG_LONG_MAX ) task_id = 0;
#elif defined(__DECCXX)
        if( task_id == ULONG_MAX ) task_id = 0;
#endif
#if defined(_REENTRANT)
	r = pthread_mutex_unlock( &task_id_mutex );
	CHECK_PTHREAD_RETCODE(r,pthread_mutex_unlock);
#endif
    } catch( const std::exception &e ) {
	protocol->logError( LOGSRC, "%s", e.what() );
    }
}

ATask::~ATask() {
}

bool CompareTask::operator()( const ATask* a, const ATask* b ) const {
    return strcmp( a->getTaskId(), b->getTaskId() ) > 0;
}

TaskAdapter::TaskAdapter() {
}
    
TaskAdapter::TaskAdapter(
    const std::string& taskName,
    ATaskFactory *factory
    ) :
    _taskName(taskName),
    _factory(factory),
    _monSrv(0) 
{
    char *ptr = _taskId;
    struct timespec abstime;
    clock_gettime( CLOCK_REALTIME, &abstime );
    assert( snprintf( _taskId, 128, "%016llx.%lx.%lx", task_id, abstime.tv_sec, abstime.tv_nsec ) >= 0 );
}

TaskAdapter::~TaskAdapter() {
    _listeners.clear();
}

void TaskAdapter::addListener( TaskListener * l ) throw() {
    _listeners.push_back( l );
}

void TaskAdapter::removeListener( TaskListener *l ) throw() {
    ATask::TaskListeners::iterator it = std::find( _listeners.begin(), _listeners.end(), l );
    if( it != _listeners.end() ) {
	_listeners.erase(it);
    }
}

const ATask::TaskListeners& TaskAdapter::getListeners() const throw() {
    return _listeners;
}

void TaskAdapter::setMonitorServer( MonitorServer *m ) throw() {
    _monSrv = m;
}

MonitorServer* TaskAdapter::getMonitorServer() throw() {
    return _monSrv;
}
	
ATaskFactory* TaskAdapter::getFactory() const throw() {
    return _factory;
}

const char* TaskAdapter::getTaskId() const throw() {
    return _taskId;
}

void TaskAdapter::setTaskName( const char* taskName ) throw() {
    _taskName = taskName;
}

const char* TaskAdapter::getTaskName() const throw() {
    return _taskName.c_str();
}

TaskContext::TaskContext() {
}
TaskContext::~TaskContext() {
}

TaskContextImpl::TaskContextImpl() throw() {
}

TaskContextImpl::~TaskContextImpl() throw() {
    _ptrsMap.clear();
    _intsMap.clear();
    _strsMap.clear();
    _namesSet.clear();
}

template <class T, class M> const T getProperty( const T& type, const M& map, const std::string& name )
{
    typename M::const_iterator it = map.find( name );
    if( it == map.end() ) {
	std::stringstream stream;
	stream
	    << "Find task context property "
	    << "name: \"" << name << "\" "
//	    << "type: \"" << type_info(info).get_name() << "\" "
	    << "failed: cannot find value"
	    ;
	throw std::domain_error( stream.str().c_str() );
    }
    return it->second;
}


template <class T,class M> void setProperty( M& map, const std::string& name, const T& value  )
{
    std::pair< typename M::iterator, bool > r =
	map.insert( typename M::value_type( name, value ) );
    if( r.second ) {
	return;
    }
    // Property exists we need to modify it.
    typename M::iterator it = map.find( name );
    if( it == map.end() ) {
	std::stringstream stream;
	stream << "Modify task context property \"" << name << "\" failed: cannot find modified value";
	throw std::domain_error( stream.str().c_str() );
    }
    it->second = value;
}

const void* TaskContextImpl::getProp(const std::string& propName ) 
    const throw(domain_error)
{
    return getProperty( (const void*)0, _ptrsMap, propName );
}

const void* TaskContextImpl::getPointer(const std::string& propName ) 
    const throw(domain_error)
{
    return getProperty(  (const void*)0, _ptrsMap, propName );
}

int TaskContextImpl::getInt(const std::string& propName )
    const throw(domain_error)
{
    return getProperty( int(0), _intsMap, propName );
}

std::string TaskContextImpl::getString(const std::string& propName )
    const throw(domain_error)
{
    std::string dummy_type;
    std::string value = getProperty( dummy_type, _strsMap, propName );
    return value;
}

void TaskContextImpl::setProp(const std::string& propName, const void* propVal) 
    throw(domain_error)
{
    setProperty( _ptrsMap, propName, propVal );
}

void TaskContextImpl::setProp(const std::string& propName, int   propVal)
    throw(domain_error)
{
    setProperty( _intsMap, propName, propVal );
}

void TaskContextImpl::setProp(const std::string& propName, const std::string& propVal)
    throw(domain_error)
{
    setProperty( _strsMap, propName, propVal );
}

END_CXXHTC_NAMESPACE
