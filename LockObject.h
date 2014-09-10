/**
 * Howto operate with locking resources.
 */

#ifndef __KISC__LOCK_OBJECT__CLASS__H__
#define __KISC__LOCK_OBJECT__CLASS__H__

#include <cxxhtc_defs.h>
#include <stdexcept>

BEGIN_CXXHTC_NAMESPACE

class LockObjectError : public std::exception {
};

class Predicate {
public:
    virtual bool getValue() const throw() = 0;
};

class LockObject {
protected:
    LockObject();
    virtual ~LockObject();
public:
    virtual void requireLock() throw(LockObjectError) = 0;
    virtual void acquireLock() throw(LockObjectError) = 0;
    virtual void releaseLock() throw(LockObjectError) = 0;
    virtual void wakeupAll() throw(LockObjectError) = 0;
    static LockObject* create( Predicate* ) throw();
    static void destroy( LockObject* ) throw();
};

END_CXXHTC_NAMESPACE
#endif

