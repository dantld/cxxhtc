/**
 * $Id: Monitor.h 111 2009-12-28 12:24:40Z olegm $
 * C++HTC
 * Monitor interface
 */

#ifndef __CXXHTC__CLASS__MonitorInterface__H__
#define __CXXHTC__CLASS__MonitorInterface__H__

#include <cxxhtc_defs.h>
#include <AbstractFactory.h>
#include <time.h>
#include <sys/timeb.h>

BEGIN_CXXHTC_NAMESPACE

class Config;

/**
 * Сервисный класс для определения констант, имён и т.д.
 */
class Monitor {
public:
    /**
     * States enum определяет состояния задач для мониторинга.
     */
    enum States {
	MONST_NORMAL /*! Задача в нормальном состоянии. Это состояние по умолчанию. */,
	MONST_WARNING/*! Задача имеет предупреждение. */,
	MONST_ERROR  /*! Задача имеет ошибки. */,
        MONST_UNKNOWN /*! Состояние задачи не определено. */
    };

    /**
     * Получить текстовое представление состояния, описанного в enum Monitor::States.
     * @param state - одно из состояний из enum Monitor::States, для которого
     * требуется получить текстовое представление.
     * @return Возвращаемое значение - указатель на строку, завершённую нулём.
     */
    static const char* getStateName( Monitor::States state ) throw();
};

/**
 * MonitorStateListener - это листенер для Вашей задачи вне
 * кода, где Вы не могли (или не хотели) получить экземпляр 
 * сервера монитора и экземпляр посредника контекста потока.
 * Обычно MonitorStateListener реализуется постановщиками
 * задачи, которые делают вызов интерфейсного объекта с целью 
 * сохранить мониторинг задачи.
 * Предполагается, что объект имеет такие методы для установки
 * MonitorStateListener.
 */
class MonitorStateListener {
public:
    MonitorStateListener() throw();
    virtual ~MonitorStateListener() throw();
    /**
     * Срабатывает, когда информационная строка становится 
     * доступной для чтения.
     */
    virtual void onInfo( const char* infoTxt ) throw() = 0;
    /**
     * Срабатывает, когда строка состояния задачи становится доступной.
     */
    virtual void onStateInfo( Monitor::States, const char* infoTxt ) throw() = 0;
};

/**
 * Листенер клиента монитора для получения информации о потоке.
 */
class MonitorThreadInfoListener {
public:
    /**
     * Срабатывает, когда информация о потоке становится доступной.
     * @param threadId - уникальный идентификатор потока.
     * @param bufTaskName - указатель на буфер для получения имени задачи.
     * @param bufTaskNameLength - размер буфера bufTaskName.
     * @param bufThreadInfo - указатель на буфер для получения текста информации потока.
     * @param bufThreadInfoLength - размер буфера bufThreadInfo.
     * @param recordTime - время события.
     * @return Возвращаемое значение равно true, если листенеру нужно 
     * прервать процесс получения данных.
     */
    virtual bool onLine( 
	int    threadId,
	char  *bufTaskName,
	int    bufTaskNameLength,
	char  *bufThreadInfo,
	int    bufThreadInfoLength,
	struct timeb *recordTime
    ) throw() = 0;
};

/**
 * Листенер клиента монитора для получения информации о состоянии задачи.
 */
class MonitorTaskStateListener {
public:
    /**
     * Срабатывает, когда становится доступна информационная строка
     * состояния задачи.
     * @param state - текущее состояние задачи.
     * @param taskName - имя задачи.
     * @param bufNameLen - размер буфера taskName
     * @param taskInfo - детальная информация о текущем состоянии задачи.
     * @param bufInfoLen - длина буфера taskInfo.
     * @param recordTime - время события.
     * @return Возвращаемое значение равно true, если листенеру нужно 
     * прервать процесс получения данных.
     */
    virtual bool onLine( 
	Monitor::States state,
	const char* taskName,
	int bufNameLen,
	const char* taskInfo,
	int bufInfoLen,
	struct timeb *recordTime
    ) throw() = 0;
};

/**
 * Класс общих исключений для методов MonitorServer и MonitorClient.
 */
class MonitorError : public std::exception {
protected:
    MonitorError() throw();
public:
    ~MonitorError() throw();
};

/**
 * MonitorClient - это база для создания пользовательских реализаций 
 * клиента монитора.
 */
class MonitorClient {
protected:
    MonitorClient() throw();
    virtual ~MonitorClient() throw();
public:
    /**
     * Получить информацию о потоках.
     */
    virtual void getThreadsInfo( MonitorThreadInfoListener* ) throw(MonitorError) = 0;
    /**
     * Получить информацию о состоянии задач.
     */
    virtual void getTasksInfo( MonitorTaskStateListener* ) throw(MonitorError) = 0;
    /**
     * Сбросить информацию о состоянии задачи.
     */
    virtual void resetTaskInfo( const char* ) throw(MonitorError) = 0;
};

/**
 * Интерфейс MonitorServer является основой для создания
 * Вашей пользовательской реализации серверной части монитора.
 * В действительности Вам не нужно использовать напрямую
 * экземпляр реализации интерфейса MonitorServer.
 * MonitorServer для Ваших задач предоставляется
 * объектом класса ThreadPool, используя
 * метод ATask::setMonitorServer. Вызов методов класса
 * MonitorServer выполняется при помощи двух макросов,
 * определённых в заголовочном файле ATask.h. Также
 * эти методы могут быть вызваны реализациями класса ThreadPool.
 * @see ThreadPool ATask
 */
class MonitorServer {
protected:
    virtual ~MonitorServer() throw();
    MonitorServer() throw();
public:
    /**
     * Установка информации о потоке.
     */
    virtual void setThreadsInfo( 
	int thrIdx,
	const char* taskName,
	const char* taskInfo
	) throw() = 0;
    /**
     * Метод setTaskState позволяет Вам сообщить о состоянии задачи.
     */
    virtual void setTaskState( 
	const char* taskName,
	Monitor::States,
	const char* infoText
	) throw() = 0;
    /**
     * Сброс информации обо всех задачах.
     */
    virtual void resetAllTasksInfo() throw() = 0;
    /**
     * Метод checkpoint используется для
     * демонстрации признаков жизни Вашего приложения.
     * Вы вызываете этот метод каждую секунду.
     * Пример использования метода checkpoint в Вашем приложении.
     * @include Monitor_examples.cpp
     */
    virtual void checkpoint() throw() = 0;
};

END_CXXHTC_NAMESPACE
#endif // __CXXHTC__CLASS__MonitorInterface__H__
