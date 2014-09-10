/**
 * $Id: Queue.h 246 2014-08-08 10:42:24Z dantld $
 * Interface for queue
 */

#ifndef __CXXHTC__LIBRARY__IFACE__QUEUE__H__
#define __CXXHTC__LIBRARY__IFACE__QUEUE__H__
#include <cxxhtc_defs.h>
#include <exception>
#include <time.h>
#include <list>
BEGIN_CXXHTC_NAMESPACE

    class Config;

    class QueueError : public std::exception {
    public:
	QueueError() throw();
	~QueueError() throw();
    };
    
    class QueueFactoryError : public std::exception {
    public:
	QueueFactoryError() throw();
	~QueueFactoryError() throw();
    };
    
    /**
     * Интерфейс Queue.
     * Интерфейс Queue может быть использован в многопоточных приложениях
     * для организации связи между двумя и более задачами.
     * Реализация интерфейса Queue должна иметь синхронизированные версии
     * всех вызовов.
     */
    class Queue {
    protected:
	Queue() throw();
	virtual ~Queue() throw();
    public:
	/**
	 * Поместить указатель в очередь.
	 */
        virtual void  push( void* ) throw( QueueError ) = 0;
	/**
	 * Извлечь указатель из очереди.
	 */
        virtual void* pop() throw( QueueError ) = 0;
	/**
	 * Извлечь указатель из очереди с указанием времени ожидания.
	 */
        virtual void* timed_pop( struct timespec * ) throw( QueueError ) = 0;
	/**
	 * Проверить, является ли очередь пустой.
	 */
	virtual bool  empty() const throw( QueueError ) = 0;
	/**
	 * Остановить очередь.
	 */
	virtual void  stop() throw( QueueError ) = 0;
    };

    /**
     * Queue2 interface introduce new method
     * pop() that can be poped multiple elements
     * from queue per single call.
     */
    class Queue2 : public Queue {
    protected:
	Queue2() throw();
	virtual ~Queue2() throw();
    public:
	/**
	 * Get multiple items from queue
	 * @list list that filled by poped elements from queue
	 * @batchSize limit maximum elments that will be poped from queue
	 */
        virtual void popItems(std::list<void*>& list, int batchSize = 0 ) throw( QueueError ) = 0;
	/**
	 * Clean queue using the clean function.
	 * @clean_fnc pointer on clean function which must destroy object
	 * @return count of cleaned items
	 */
	virtual int clean( void (*clean_fnc)( void* ) ) throw(QueueError) = 0;
	/**
	 * Push item with pointer on clean function
	 * Implementation can "cut" the qeueue length
	 * by deleting front element
	 */
	virtual void pushAndCut( void* item, void (*clean_fnc)( void* ) ) throw(QueueError) = 0;
    };

    /** 
     * 
     */    
    class QueueFactory {
    protected:
	QueueFactory() throw();
    public:
	virtual ~QueueFactory() throw();
        virtual Queue* createInstance( const Config* ) throw(QueueFactoryError) = 0;
        virtual void destroyInstance( Queue* ) throw(QueueFactoryError) = 0;
    };
    
END_CXXHTC_NAMESPACE
#endif // __CXXHTC__LIBRARY__IFACE__QUEUE__H__
