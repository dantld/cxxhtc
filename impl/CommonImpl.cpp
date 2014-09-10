/**
 * $Id: CommonImpl.cpp 238 2013-06-14 04:29:24Z dantld $
 * C++ HTC library: static members
 */

#include "../ThreadPool.h"
#include "../BuffersPool.h"
#include "../Synchro.h"
#include "../Queue.h"
#include "../AbstractFactory.h"
#include "../ThreadPool.h"
#include "../Task.h"
#include "../ConfigBuilder.h"

BEGIN_CXXHTC_NAMESPACE
const char* const ThreadPool::DBGSRC = "tp";

BuffersPool::BuffersPool() throw(BuffersPoolError) {
}

BuffersPool::~BuffersPool() throw() {
}

ConfigBuilderPropertyChangeListener::ConfigBuilderPropertyChangeListener() throw()
{
}

ConfigBuilderPropertyChangeListener::~ConfigBuilderPropertyChangeListener() throw()
{
}

ConfigBuilderListener::ConfigBuilderListener() throw()
{
}

ConfigBuilderListener::~ConfigBuilderListener() throw()
{
}

ConfigDestroyListenerError::ConfigDestroyListenerError() throw()
{
}
ConfigDestroyListenerError::~ConfigDestroyListenerError() throw()
{
}

ConfigDestroyListener::ConfigDestroyListener() throw()
{
}
ConfigDestroyListener::~ConfigDestroyListener() throw()
{
}

ThreadPoolError::ThreadPoolError() throw() {}
ThreadPoolError::~ThreadPoolError() throw() {}

ThreadPool::~ThreadPool() {
}

ThreadPool::ThreadPool() {
}

ThreadPoolFactory::~ThreadPoolFactory() throw() {}

ThreadPoolFactory::ThreadPoolFactory() throw() {}

ReadWriteSyncFactoryError::ReadWriteSyncFactoryError() throw() {
}
ReadWriteSyncFactoryError::~ReadWriteSyncFactoryError() throw() {
}
const char* ReadWriteSyncFactoryError::what() const throw() {
    return "ReadWriteSyncFactoryError";
}

ReadWriteSyncFactory::ReadWriteSyncFactory() throw() {
}

ReadWriteSyncFactory::~ReadWriteSyncFactory() throw() {
}

QueueError::QueueError() throw() {}
QueueError::~QueueError() throw() {}
QueueFactoryError::QueueFactoryError() throw() {}
QueueFactoryError::~QueueFactoryError() throw() {}
Queue::Queue() throw() {}
Queue::~Queue() throw() {}
Queue2::Queue2() throw() {}
Queue2::~Queue2() throw() {}
QueueFactory::QueueFactory() throw() {}
QueueFactory::~QueueFactory() throw() {}

AbstractFactoryError::AbstractFactoryError() throw() { }
AbstractFactoryError::~AbstractFactoryError() throw() { }
AbstractFactory::AbstractFactory() throw(AbstractFactoryError) { }
AbstractFactory::~AbstractFactory() throw() { }

END_CXXHTC_NAMESPACE

