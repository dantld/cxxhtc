/**
 * $Id: ObjectsPool.cpp 196 2011-07-05 10:49:15Z dantld $
 * Library: C++HTC
 * Interface: Object Pool
 */

#include "../ObjectsPool.h"
#include <string>

BEGIN_CXXHTC_NAMESPACE
namespace {

class AbstractFactoryErrorImpl : public AbstractFactoryError
{
    std::string _m;
public:
    AbstractFactoryErrorImpl(const char* m) throw() : _m(m) {}
    ~AbstractFactoryErrorImpl() throw() {}
    const char* what() const throw() { return _m.c_str(); }
};

}

Poolable::Poolable() throw()
{
}
Poolable::~Poolable() throw()
{
}
PoolableFactoryError::PoolableFactoryError( const std::string &m )
    throw() 
    :
    _m(m)
{
}
PoolableFactoryError::~PoolableFactoryError() throw()
{
}
const char* PoolableFactoryError::what() const throw()
{
    return _m.c_str();
}
PoolableFactory::PoolableFactory() throw()
{
}
PoolableFactory::~PoolableFactory() throw()
{
}
void* PoolableFactory::createInstance( const char* name, Config* config ) throw(AbstractFactoryError)
{
    throw AbstractFactoryErrorImpl("PoolableFactory can not uses createInstance method.");
}
void PoolableFactory::destroyInstance( void* object ) throw(AbstractFactoryError)
{
    throw AbstractFactoryErrorImpl("PoolableFactory can not uses destroyInstance method.");
}
ObjectsPoolError::ObjectsPoolError() 
{
}

ObjectsPoolError::~ObjectsPoolError() throw()
{
}

ObjectsPool::ObjectsPool() throw()
{
}
ObjectsPool::~ObjectsPool() throw()
{
}

END_CXXHTC_NAMESPACE
