 /**
 * $Id: BuffersPool.h 107 2009-12-21 04:12:26Z olegm $
 * C++HTC library.  Buffers pool.
 */

#ifndef __CXXHTC_LIBRARY__BuffersPool_Class__H__
#define __CXXHTC_LIBRARY__BuffersPool_Class__H__

#include <cxxhtc_defs.h>
#include <stdexcept>
#include "Config.h"
#include <AbstractFactory.h>

BEGIN_CXXHTC_NAMESPACE

class Config;

class BuffersPoolError : public std::exception {
};

/**
 * Класс BufferPool:
 * создание буферов char и управление ими в многопоточном окружении.
 *
**/
class BuffersPool  
{
protected:
    BuffersPool() throw(BuffersPoolError);
public:
    typedef unsigned int BufferSize;
    typedef void* BufferPtr;

    virtual ~BuffersPool() throw();
    virtual BufferPtr getBuffer(BufferSize) throw (BuffersPoolError) = 0;
    virtual BufferSize getBufferSize( BufferPtr ) throw(BuffersPoolError) = 0; 
    virtual void free(BufferPtr ) throw(BuffersPoolError) = 0;
    virtual int getFreeBuffers() throw() = 0;
    virtual void prepareToExit() const throw() = 0;
};


class BuffersPoolFactory : public AbstractFactory 
{
protected:
	BuffersPoolFactory() throw();
	virtual ~BuffersPoolFactory() throw();
};

END_CXXHTC_NAMESPACE

#endif //__CXXHTC_LIBRARY__BuffersPool_Class__H__
