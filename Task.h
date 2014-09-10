/**
 *  Abstract task declaration
 *  $Id: Task.h 205 2011-07-18 10:23:25Z dantld $
 */

#ifndef __KISC__MSPD2__ATASK__H__
#define __KISC__MSPD2__ATASK__H__
#include <cxxhtc_defs.h>
#include <stdexcept>
#include <string>
#include <list>
#include <set>
#include <map>
#include <functional>
#include "DynamicLoader.h"
#include "Config.h"
#include "AbstractFactory.h"
#include "SmartPointers.h"

/**
 * @def SET_TASK_INFO( tctx, text )
 * Фиксация с целью мониторинга информации о работе, которой 
 * занята задача в настоящий момент.
 */
#define SET_TASK_INFO( tctx, text ) \
    if( getMonitorServer() ) {\
	getMonitorServer()->setThreadsInfo( tctx->getThreadIndex(), getTaskName(), text );\
    }

/**
 * @def SET_TASK_STATE( state, infoText )
 * Фиксация с целью мониторинга информации об ошибках, возникших при выполнении задачи.
 */
#define SET_TASK_STATE( state, infoText ) \
    if( getMonitorServer() ) getMonitorServer()->setTaskState( getTaskName(), state, infoText );
    
BEGIN_CXXHTC_NAMESPACE

class MonitorServer;
using std::domain_error;
using std::list;

/**
 * Класс исключений, который может быть возбуждён методом ATask::task.
 */
class TaskException : public std::runtime_error {
public:
    TaskException( const std::string &what ) : std::runtime_error(what) {}
};


/**
 * Класс исключений, который может быть возбуждён методами фабрики TaskFactory.
 */
class TaskFactoryException : public AbstractFactoryError {
    std::string _m;
public:
    TaskFactoryException() throw();
    TaskFactoryException( const char* ) throw();
    ~TaskFactoryException() throw();
    const char* what() const throw();
};

/**
 * TaskContext - посреднический интерфейс для обеспечения связи
 * между Вашей реализацией задачи и основным модулем приложения 
 * или другими реализациями задачи. TaskContext даёт возможность
 * обмена информацией (данные, объекты, текст) между задачами.
 * @see ATask
 */
class TaskContext : public Obj {
protected:
    TaskContext();
public:
    typedef SmartPtr<TaskContext> Ptr;
    virtual ~TaskContext();
    /**
     * Получить свойство по константной версии имени.
     * @param name - имя запрашиваемого параметра.
     * @return Возвращается указатель на свойство. Это может быть указатель
     * на структуру, экземпляр класса, интегральный тип или текстовую строку.
     * @exception domain_error, если параметров с таким именем нет.
     * @deprecate nto available in a next version
     */
    virtual const void* getProp(const std::string& name) const throw(domain_error) = 0;
    
    virtual const void* getPointer(const std::string& name) const throw(domain_error) = 0;
    virtual int getInt(const std::string& name) const throw(domain_error) = 0;
    virtual std::string getString(const std::string& name) const throw(domain_error) = 0;
};

/**
 * Simple implementation class for interface TaskContext.
 * TaskContextImpl introduce the simple properties map.
 * Also implementation may be used as adapter class.
 */
class TaskContextImpl : public TaskContext {
    typedef std::set< std::string >             PropNamesSet;
    typedef std::map< std::string, const void*>       PtrPropsMap;
    typedef std::map< std::string, int>         IntPropsMap;
    typedef std::map< std::string, std::string> StrPropsMap;
    PropNamesSet _namesSet;
    PtrPropsMap _ptrsMap;
    IntPropsMap _intsMap;
    StrPropsMap _strsMap;
public:
    typedef SmartPtr<TaskContextImpl> Ptr;
    TaskContextImpl() throw();
    ~TaskContextImpl() throw();

    virtual const void* getProp(const std::string& name) const throw(domain_error);

    virtual const void* getPointer(const std::string& name) const throw(domain_error);
    virtual int getInt(const std::string& name) const throw(domain_error);
    virtual std::string getString(const std::string& name) const throw(domain_error);
    
    virtual void setProp(const std::string& propName, const void* propVal) throw(domain_error);
    virtual void setProp(const std::string& propName, int   propVal) throw(domain_error);
    virtual void setProp(const std::string& propName, const std::string& propVal) throw(domain_error);
};

/**
 * ThreadContext - это посреднический интерфейс между интерфейсом
 * ATask и скрытой реализацией потока.
 * ThreadContext позволяет получать информацию из Ваших реализаций
 * интерфейса ATask или класса TaskAdapter.
 * Фактически реализация посреднического интерфейса ThreadContext 
 * появляется в реализациях интерфейса ThreadPool.
 */
class ThreadContext {
protected:
    ThreadContext() throw();
    virtual ~ThreadContext() throw();
public:
    /**
     * Информационный метод, возвращающий индекс потока.
     * Индекс потока - это абстрактный уникальный номер, назначаемый
     * интерфейсом ThreadPool. Он не является системным идентификатором
     * потока.
     * @return Индекс текущего потока, привязанный к задаче.
     * @see ThreadPool
     */
    virtual int getThreadIndex() const throw() = 0;
};

class ATask;       
/**
 * Интерфейс TaskListener - это обобщённый листенер для контроля завершения
 * задач. Если Вам нужна информация о том, как и когда завершилась
 * Ваша задача, то необходимо сделать реализацию интерфейса
 * TaskListener и добавить её в экземпляр задачи с помощью
 * вызова ATask::addListener. Касательно вызова методов имейте ввиду, что
 * они располагаются в другом потоке. Соответственно необходимо
 * обеспечивать синхронизацию.
 * @see ATask::addListener 
 *
 * Вам нужно убедиться, что код Ваших реализаций методов onComplete 
 * и onTaskError синхронизирован, потому что эти методы выполняются
 * в потоке, обслуживающем задачу. И если Вы назначаете листенер
 * для многих задач, возможно возникновение конкуренции вызовов 
 * методов листенера.
 */
class TaskListener {
public:
    TaskListener();
    ~TaskListener();
    /**
     * Срабатывает при завершении задачи.
     * @param task - указатель на экземпляр реализации задачи ATask, чей метод возвращается.
     */
    virtual void onComplete( const ATask* task ) const throw() = 0;
    /**
     * Срабатывает, когда задача поднимает исключение.
     * @param task - указатель на экземпляр реализации задачи ATask, чей метод возвращается.
     * @error Возникает экземпляр исключения, который был поднят методом задачи.
     */
    virtual void onTaskError( const ATask* task, const std::exception& error ) const throw() = 0;
};

class ATaskFactory;

/**
 * Абстрактный интерфейс задач.
 * Интерфейс вводит методы, необходимые для сложной инфрастуктуры
 * приложений с пулом потоков, вызовами мониторинга и информации.
 * Большинство методов применимы в адаптивном классе TaskAdapter.
 * @see ThreadPool ThreadContext Monitor TaskListener TaskAdapter ATaskFactory
 */
class ATask {
protected:
    ATask();
    virtual ~ATask();
public:
    typedef list<TaskListener*> TaskListeners;
    typedef list<TaskListener*>::const_iterator TaskListenersIter;
    /**
     * Наиболее важный метод в этом интефейсе.
     * Вы всегда имеете реализацию этого метода в Вашей пользовательской задаче.
     * Этот метод будет работать во временно выделенном потоке из пула потоков
     * и будет выполнять полезную работу для Вашего приложения.
     * @param tctx - экземпляр класса ThreadContext для целей мониторинга.
     * @exception TaskException возникает, когда задача сталкивается с серьёзной ошибкой
     * и не может продолжать работу.
     */
    virtual void task( const ThreadContext* tctx ) throw (TaskException) = 0;
    /**
     * Добавить листенер к этой задаче для обнаружения изменений в состоянии задачи.
     * @param listener - указатель на листенер, который нужно добавить.
     */
    virtual void addListener( TaskListener *listener ) throw() = 0;
    /**
     * Удалить листенер, ранее добавленный вызовом ATask::addListener.
     * @param listener - листенер для удаления.
     */
    virtual void removeListener( TaskListener *listener ) throw() = 0;
    /**
     * Получить список листенеров.
     */
    virtual const TaskListeners& getListeners() const throw() = 0;
    /**
     * Установить сервер монитора, позволяющий Вашей задаче выполнять мониторинг.
     * Вы не можете вызывать этот метод из Вашего приложения. Он вызывается 
     * реализациями класса ThreadPool.
     */
    virtual void setMonitorServer( MonitorServer * ) throw() = 0;
    /**
     * Получить фабрику задач. Этот метод будет использоваться реализациями
     * класса ThreadPool для уничтожения экземпляра задачи.
     * @return фабрика, которая использовалась для создания задачи.
     */    
    virtual ATaskFactory* getFactory() const throw() = 0;
    /**
     * Получить имя задачи. Метод использует макросы мониторинга SET_TASK_INFO
     * и SET_TASK_STATE.
     * @return Возвращается имя задачи.
     */
    virtual const char* getTaskName() const throw() = 0;
    /**
     * Получить уникальный идентификатор задачи. Этот вспомогательный метод
     * используется для организации набора задач.
     * @return Уникальный идентификатор задачи.
     */
    virtual const char* getTaskId() const throw() = 0;
};

/**
 * Адаптивный класс.
 * Класс TaskAdapter помогает Вам реализовать Ваши пользовательские задачи.
 * Адаптер реализует имеющийся список листенеров, сервер монитора,
 * идентификатор задачи, фабрику задачи, имя задачи.
 * @see ATask
 */
class TaskAdapter : public ATask {
    ATask::TaskListeners _listeners;
    MonitorServer *_monSrv;
    char _taskId[129];
    ATaskFactory* _factory;
    std::string _taskName;
    TaskAdapter();
protected:
    TaskAdapter( const std::string &, ATaskFactory* );
    virtual ~TaskAdapter();
    void setTaskName( const char* taskName ) throw();
public:
    virtual const char* getTaskName() const throw();
    virtual void addListener( TaskListener * ) throw();
    virtual void removeListener( TaskListener * ) throw();
    virtual const TaskListeners& getListeners() const throw();
    virtual void setMonitorServer( MonitorServer * ) throw();
    MonitorServer* getMonitorServer() throw();
    virtual const char* getTaskId() const throw();
    virtual ATaskFactory* getFactory() const throw();
};

/**
 * Класс TaskStrategy нужен для организации цикла жизни Вашей
 * пользовательской реализации задачи. Реализация класса TaskStrategy
 * определяет, когда задачу требуется выполнить или уничтожить.
 * @see ATask ThreadPool
 */
class TaskStrategy {
protected:
    ATask* _task;
public:
    TaskStrategy();
    virtual ~TaskStrategy();

    virtual void   setTask( ATask* task ) { _task = task; }
    virtual ATask* getTask()  { return _task; }
    
    /**
     * Одноразовая задача.
     * @return По умолчанию задача является одноразовой (возвращаемое значение true).
     */
    virtual bool oneTime() throw() { return true; }
    /**
     * Если задача завершила свой жизненный цикл, она может быть удалена.
     * Соответственно, реализация класса ThreadPool принимает решение
     * на уничтожение задачи, если возвращаемое значение метода isReadyToDestroy()
     * равно true.
     * @return По умолчанию возвращаемое значение true (задача готова к уничтожению).
     */
    virtual bool isReadyToDestroy() throw() { return true; }
    /**
     * Задача требует исполнения.
     * Реализация класса ThreadPool в зависимости от значения, возвращаемого методом isNeedToRun(), 
     * принимает решение о повторном исполнении задачи. Если метод isNeedToRun() возвращает false,
     * то задача продолжает оставаться в состоянии ожидания, пока один из методов isNeedToRun() 
     * или isReadyToDestroy() не вернёт значение true. Соответственно, задача будет перезапущена 
     * или уничтожена.
     * @return По умолчанию возвращаемое значение равно false - задача не требует перезапуска.
     */
     virtual bool isNeedToRun() throw() { return false; }
};


/**
 * Фабрика задач.
 * Класс ATaskFactory является расширением класса AbstractFactory и
 * вводит новые методы, специфичные для процедуры создания задач.
 * Процедура создания задачи часто требует передачи объекта 
 * класса TaskContext. Фабрика также требуется при создании
 * экземпляра класса TaskStrategy.
 * @see AbstractFactory ATask TaskStrategy
 */
class ATaskFactory : public AbstractFactory {
    virtual void  destroyInstance( void* ) throw(AbstractFactoryError);
public:
    typedef DynamicLoader<ATaskFactory> Loader;
    ATaskFactory();
    virtual ~ATaskFactory() throw();
    /**
     * Создать экземпляр класса ATask.
     * @param config - конфигурация приложения как указатель на экземпляр класса Config.
     * @param taskContext - специфический контекст задачи как указатель на экземпляр класса TaskContext.
     * @return Указатель на новый экземпляр задачи (класс ATask).
     */
    virtual ATask* createTask(
	const std::string &name,
	Config *config,
	TaskContext *taskContext
	) throw (TaskFactoryException) = 0;
    /**
     * Создать экземпляр класса TaskStrategy.
     * @param task - экземпляр класса ATask, для которого требуется стратегия.
     * @return Указатель на новый экземпляр класса TaskStrategy.
     */
    virtual TaskStrategy* createStrategy( ATask* task ) throw (TaskFactoryException) = 0;

    virtual void  destroyTask( ATask* ) throw(AbstractFactoryError) = 0;
    virtual void* createInstance( const char*, Config* ) throw(AbstractFactoryError);

};

struct CompareTask 
{
    bool operator()( const ATask* a, const ATask* b ) const;
}; 

typedef std::set< const ATask*, CompareTask > SetOfTasks;
typedef SetOfTasks::iterator SetOfTasksIter;
typedef SetOfTasks::const_iterator SetOfTasksCIter;


END_CXXHTC_NAMESPACE

#endif // __KISC__MSPD2__ATASK__H__

