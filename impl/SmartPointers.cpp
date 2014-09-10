/**
 * $Id: SmartPointers.cpp 109 2009-12-22 03:30:42Z dantld $
 *
 * SmartPointers implementation part
 */
 
#include "../SmartPointers.h"
#include <pthread.h>
#include <iostream>

BEGIN_CXXHTC_NAMESPACE

#ifdef _REENTRANT
static pthread_mutex_t object_reference_counter_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

Obj::Obj()
    : refcnt(0) 
{
}
Obj::~Obj()
{
}
int Obj::incRefCnt() throw()
{ 
#ifdef _REENTRANT
    pthread_mutex_lock( &object_reference_counter_mutex );
#endif
    refcnt++; 
#ifdef _REENTRANT
    pthread_mutex_unlock( &object_reference_counter_mutex );
#endif
    return refcnt;
}
int Obj::decRefCnt() throw()
{ 
#ifdef _REENTRANT
    pthread_mutex_lock( &object_reference_counter_mutex );
#endif
    refcnt--;
#ifdef _REENTRANT
    pthread_mutex_unlock( &object_reference_counter_mutex );
#endif
    return refcnt;
}
int  Obj::getRefCnt() throw()
{ 
    return refcnt; 
}

END_CXXHTC_NAMESPACE
