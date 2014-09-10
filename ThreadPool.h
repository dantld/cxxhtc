/**
 * Light thread pool
 * $Id: ThreadPool.h 119 2010-01-10 12:02:18Z olegm $
 */

#ifndef __LIGHT__THREAD__POOL__CLASS__H__
#define __LIGHT__THREAD__POOL__CLASS__H__
#include <cxxhtc_defs.h>
#include <DynamicLoader.h>
#include <stdexcept>
#include <AbstractFactory.h>

BEGIN_CXXHTC_NAMESPACE

class ATask;
class TaskStrategy;
class Config;

/**
 * Общие ошибки для реализаций пула потоков.
 */
class ThreadPoolError : public std::exception
{
public:
    ThreadPoolError() throw();
    ~ThreadPoolError() throw();
};

/**
 * Интерфейс для пула потоков.
 * Пул потоков - это некоторая специальная версия шаблона проектирования
 * известного как пул ресурсов. Пул потоков - это основная концепция для
 * создания многопоточных приложений, ориентированных на работу с задачами.
 * @see ATask
 */
class ThreadPool {
protected:
    virtual ~ThreadPool();
public:
    ThreadPool();
    /**
     * Метод processTask помещает задачу в очередь. Задачи из этой очереди
     * в последствии будут обработаны во временно выделенном потоке.
     * @param task - экземпляр реализации класса ATask.
     * @param strategy - экземпляр класса TaskStrategy. Если стратегия не 
     * требуется, параметр равен NULL. Такие задачи всегда являются одноразовыми.
     */
    virtual void processTask( ATask *task, TaskStrategy *strategy ) throw(ThreadPoolError) = 0;
    virtual void suspend() = 0;
    virtual void resume() = 0;
    /**
     * Получение информации.
     * @return Конфигурационный максимум предварительно загружаемых потоков.
     */
    virtual int getMaxPreloadThreads() const throw() = 0;
    virtual void reload() throw(ThreadPoolError) = 0;
    static ThreadPool* createInstance();
    
    static const char* const DBGSRC;
};

const char* const THREADPOOL_FACTORY_NAME = "factory.object.threadpool";
class ThreadPoolFactory : public AbstractFactory {
protected:
    ThreadPoolFactory() throw();
public:
    typedef DynamicLoader<ThreadPoolFactory> Loader;
    virtual ~ThreadPoolFactory() throw();
};

END_CXXHTC_NAMESPACE

#endif //__LIGHT__THREAD__POOL__CLASS__H__
