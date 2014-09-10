/**
 * $Id: ObjectsPool.h 200 2011-07-08 11:19:42Z dantld $
 * Library: C++HTC
 * Interface: Object Pool
 */
#ifndef __CPPHTC__OBJECTS_POOL__IFACE__H__
#define __CPPHTC__OBJECTS_POOL__IFACE__H__

#include <cxxhtc_defs.h>
#include <stdexcept>
#include <Config.h>
#include <AbstractFactory.h>
#include <CommonErrors.h>

BEGIN_CXXHTC_NAMESPACE

/**
 * Poolable - простой интефейс, используемый реализациями
 * интерфейса ObjectPool для принятия решения о повторном
 * создании экземпляра.
 * Poolable является продуктом фабрики PoolableFactory.
 * @see PoolableFactory ObjectsPool
 */
class Poolable {
protected:
    Poolable() throw();
    virtual ~Poolable() throw();
public:
    /**
     * @memberof Метод даёт возможность пулу объектов запрашивать
     * информацию о состоянии объекта. 
     * @return Возвращаемое значение true, если объект в хорошем
     * состоянии, или false, если объект имеет ошибки и 
     * нуждается в повторном создании.
     * @see ObjectsPool::release
     */
    virtual bool isOk() throw() = 0;
    /**
     * Метод предназначен для хранения и идентификации экземпляров в реализациях
     * ObjectsPool.
     * @return Уникальный идентификатор экземпляра.
     */
    virtual std::string getId() const throw() = 0;
    /**
     * Метод, предназначенный для контроля состояния объекта, реализациями интерфейса
     * ObjectsPool.
     */
    virtual void poll() throw() = 0;
    /**
     * Return the reason of false return value from isOk method
     */
    virtual std::string reason() const throw() = 0;
};

/**
 * Исключение для интерфейса PoolableFactory.
 */
class PoolableFactoryError : public std::exception {
    const std::string _m;
public:
    PoolableFactoryError( const std::string &m ) throw();
    ~PoolableFactoryError() throw();
    const char* what() const throw();
};

/**
 * Фабрика PoolableFactory - интерфейс для создания экземпляров
 * конкретных реализаций класса Poolable.
 * Фабрика PoolableFactory применяется для сокрытия деталей
 * создаваемых объектов в реализациях класса ObjectsPool.
 * @see Poolable ObjectsPool
 */
class PoolableFactory : public AbstractFactory {
protected:
    PoolableFactory() throw();
    virtual ~PoolableFactory() throw();
public:
    /**
     * Создание экземпляра конкректной реализации интерфейса Poolable.
     * @param poolName - имя пула объектов.
     * @param config - указатель на конфигурационный объект приложения класса Config.
     * @return Возвращаемое значение - указатель на новый экземпляр реализации класса Poolable.
     * @exception PoolableFactoryError возникает, если создание объекта оказалось неудачным.
     * @exception std::runtime_error configuration or object state cannot be used in application. Clients have to terminate process if this exception has caught.
     *
     */
    virtual Poolable* createPoolable( const char* poolName, Config* config ) 
	throw(PoolableFactoryError,std::runtime_error) = 0;
    /**
     * Уничтожение экземпляра, созданного методом createInstance.
     * @exception PoolableFactoryError возникает, если уничтожение объекта оказалось неудачным.
     */
    virtual void destroyPoolable( Poolable* ) throw(PoolableFactoryError) = 0;
    void* createInstance( const char* name, Config* config ) throw(AbstractFactoryError);
    void  destroyInstance( void* object ) throw(AbstractFactoryError);
};

/**
 * Общий класс исключений для всех типов ошибок, которые
 * могут быть возбуждены в реализациях интерфейса ObjectsPool.
 */
class ObjectsPoolError : public std::exception {
public:
    ObjectsPoolError();
    ~ObjectsPoolError() throw();
};

/**
 * Интерфейс ObjectsPool является общим шаблоном программирования (известным как
 * resources pool) для хранения и повторного использования дорогостоящих
 * в плане создания объектов.
 * Интерфейс использует фабрику PoolableFactory для создания объектов класса Poolable.
 * @see PoolableFactory Poolable
 */
class ObjectsPool {
protected:
    ObjectsPool() throw();
    virtual ~ObjectsPool() throw();
public:
    /**
     * Получить свободный объект из пула.
     * Если доступен свободный объект из пула, метод немедленно возвращает
     * указатель на экземпляр. Если свободных объектов в пуле нет,
     * метод будет ждать появления свободных объектов.
     * Если допустимый период ожидания будет превышен, возникнет исключение.
     * @exception ResourceBusy get no available objects in a pool 
     * @exception ObjectsPoolError internal error has happened in a pool
     * @return Указатель на экземпляр объекта класса Poolable.
     */
    virtual Poolable* get() throw(ObjectsPoolError,ResourceBusy) = 0;
    /**
     * Возвращает занятый объект в пул.
     * Состояние объекта изменяется с BUSY на FREE.
     * Если объект находится в состоянии ошибки, он будет создан повторно.
     * @param object - указатель, ранее вызванный с помощью метода @call get.
     * @see Poolable::isOk.
     */
    virtual void release( Poolable* object ) throw(ObjectsPoolError) = 0;
    /**
     * Информация о занятых объектах.
     * @return Возвращаемое значение - количество объектов, занятых в данный момент.
     */
    virtual int getBusyCount() const throw() = 0;
    /**
     * Информация о конфигурации пула.
     * @return Максимальное количество объектов, доступное согласно конфигурации.
     */
    virtual int getMaxSize() const throw() = 0;
    /**
     * Информация о параметрах реального времени.
     * @return Количество объектов, созданных пулом на настоящий момент времени.
     */
    virtual int getSize() const throw() = 0;
    /**
     * Информация о конфигурации пула.
     * @return Допустимое время ожидания в секундах.
     */
    virtual int getTimeout() const throw() = 0;
    /**
     * Get the wait for busy timeout.
     * @return timeout in a seconds
     */
    virtual int getWaitbusyTimeout() const throw() = 0;
    /**
     * Get the objects polling interval.
     * Be shure that your implementation has support
     * objects polling facility before using this method.
     * If returned value zero then polling facility 
     * @return poll interval in a seconds.
     */
    virtual int getPollInterval() const throw() = 0;
 
};

END_CXXHTC_NAMESPACE
#endif // __CPPHTC__OBJECTS_POOL__IFACE__H__
