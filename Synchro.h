/**
 * Read-Write synchronization interface
 * $Id: Synchro.h 207 2011-07-22 11:23:37Z andrey.volkov $
 */

#ifndef __KISC__CXX_HTC_LIBRARY__SYNCHRO__IFACES___H__
#define __KISC__CXX_HTC_LIBRARY__SYNCHRO__IFACES___H__
#include <cxxhtc_defs.h>
#include <DynamicLoader.h>
#include <CommonErrors.h>
#include <stdexcept>

/**
 * @defgroup Synchro Синхронизация Вашего кода.
 */

BEGIN_CXXHTC_NAMESPACE

class Config;
class ReadWriteSyncFactory;
BEGIN_CXXHTC_UTIL_NAMESPACE

/**
 * @ingroup Synchro 
 * Простой класс для обеспечения синхронизации
 * операций чтения в Вашем коде. Базируется
 * на шаблоне и области видимости языка C++.
 * Параметризованный класс T должен иметь методы:
 * readLock() and unLock().
 */
template <class T> class AutoReadLock
{
    T* _lock;
    AutoReadLock() {}
public:
    AutoReadLock( T* lock ) : _lock(lock) { _lock->readLock(); }
    ~AutoReadLock() { _lock->unLock(); }
};

/**
 * @ingroup Synchro
 * Простой класс для обеспечения синхронизации
 * операций записи в Вашем коде. Базируется
 * на шаблоне и области видимости языка C++.
 * Параметризованный класс T должен иметь методы:
 * writeLock() and unLock().
 */
template <class T> class AutoWriteLock
{
    T* _lock;
    AutoWriteLock() {}
public:
    AutoWriteLock( T* lock ) : _lock(lock) { _lock->writeLock(); }
    ~AutoWriteLock() { _lock->unLock(); }
};
END_CXXHTC_UTIL_NAMESPACE

/**
 * @ingroup Synchro 
 * Интерфейс для абстракции блокировки чтения-записи.
 * Экземпляры реализаций используются с двумя
 * шаблонными классами: AutoReadLock and AutoWriteLock.
 */
class ReadWriteSync {
protected:
    virtual ~ReadWriteSync();
    ReadWriteSync();
public:
    /**
     * Запрос блокировки операции чтения.
     */
    virtual void readLock() throw(SyncError) = 0;
    /**
     * Запрос блокировки операции записи.
     */
    virtual void writeLock() throw(SyncError) = 0;
    /**
     * Освобождение ранее полученной блокировки операции чтения или записи.
     */
    virtual void unLock() throw(SyncError) = 0;
    static ReadWriteSyncFactory* getDefaultFactory() throw();
};
/*TODO: wrong std::exception ancestor for factory error*/
 
class ReadWriteSyncFactoryError : public std::exception {
public:
    ReadWriteSyncFactoryError() throw();
    ~ReadWriteSyncFactoryError() throw();
    const char* what() const throw();
};

class ReadWriteSyncFactory {
protected:
    ReadWriteSyncFactory() throw();
public:
    static const char* const READ_WRITE_SYNC_FACTORY_NAME;
    typedef DynamicLoader<ReadWriteSyncFactory> Loader;
    virtual ~ReadWriteSyncFactory() throw();
    virtual ReadWriteSync* create( Config * ) throw(ReadWriteSyncFactoryError) = 0;
    virtual void destroy( ReadWriteSync* ) throw(ReadWriteSyncFactoryError) = 0;
};

END_CXXHTC_NAMESPACE

#endif //__KISC__CXX_HTC_LIBRARY__SYNCHRO__IFACES___H__
