/**
 *  Configration parameters abstraction
 *  $Id: ConfigBuilder.h 202 2011-07-14 05:38:24Z dantld $
 */

#ifndef __KISC__CXXHTC__ConfigBuilder__H__
#define __KISC__CXXHTC__ConfigBuilder__H__

#include <cxxhtc_defs.h>
#include <stdexcept>
#include <string>
#include <list>
#include <Config.h>

BEGIN_CXXHTC_NAMESPACE

using std::domain_error;
using std::list;

class AbstractFactory;


class ConfigBuilderError : public std::exception
{
};

class ConfigBuilderListenerError : public std::exception {
};
/**
 * @ingroup Configurations
 * Листенер на изменение свойств на этапе ConfigBuilder::buildProperties.
 */
class ConfigBuilderPropertyChangeListener
{
public:
    ConfigBuilderPropertyChangeListener() throw();
    virtual ~ConfigBuilderPropertyChangeListener() throw();
    /**
     * Срабатывает при чтении любого свойства из источника конфигурации.
     * Пример использования ConfigBuilderPropertyChangeListener:
     * @include ConfigBuilderPropertyChangeListener_example.cpp
     */
    virtual void onChangeProperty( 
	const std::string&,
	std::string&
    ) throw(ConfigBuilderListenerError) = 0;
};

/**
 * @ingroup Configurations
 * Листенер для поддержки процесса
 * построения в реализации ConfigBuilder.
 * В примере ниже показан код ConfigBuilderListener
 * для перехвата процессов изменения свойств и построения 
 * пользовательского объекта.
 * @include ConfigBuilderListener_example.cpp
 */
class ConfigBuilderListener
{
public:
    ConfigBuilderListener() throw();
    virtual ~ConfigBuilderListener() throw();
    /**
     * Срабатывает на любом свойстве.
     * Вы можете захватить некоторые свойства в 
     * глобальных или локальных переменных Вашего 
     * приложения на этапе ConfigBuilder::buildProperties.
     * 
     */
    virtual void onProperty( 
	const std::string&,
	const std::string&
    ) throw(ConfigBuilderListenerError) = 0;
    /**
     * Срабатывает на этапе ConfigBuilder::buildObjects
     * перед созданием объекта.
     * @return Возвращаемое значение - указатель
     * на объект, если листенер самостоятельно создаёт
     * объект, или NULL.
     */
    virtual void* beforeBuildObject(
	Config *c,
	const std::string objectName,
	AbstractFactory *factory
    ) throw(ConfigBuilderListenerError) = 0;
    /**
     * Срабатывает на этапе ConfigBuilder::buildObjects
     * после того, как объект был создан.
     * @return Возвращаемое значение - указатель 
     * на объект, если листенер самостоятельно создаёт
     * объект, или NULL.
     */
    virtual void afterBuildObject(
	Config *c,
	const std::string objectName,
	void* obj
    ) throw(ConfigBuilderListenerError) = 0;

    /**
     * Call before build task.
     * Can be used for creating custom-factory tasks
     * @param taskStrategy is output
     */
    virtual ATask* beforeBuildTask(
	Config *c,
	const std::string taskName,
	ATaskFactory *factory,
	TaskStrategy **taskStrategy
    ) throw(ConfigBuilderListenerError) = 0;
    /**
     * Call after task was builded
     */
    virtual void afterBuildTask(
	Config *c,
	const std::string taskName,
	ATask* task
    ) throw(ConfigBuilderListenerError) = 0;
};

/**
 * @ingroup Configurations
 * Интерфейс для использования в Вашем приложении.
 * Продуктом ConfigBuilder является Config.
 * Реализация интерфейса ConfigBuilder должна комплексно строить
 * экземпляр класса Config, необходимый Вашему приложению.
 * Вы можете установить в различных местах листенеры для отслеживания
 * изменений процесса построения.
 * Использование листенеров также позволит Вам изменять некоторые
 * свойства во время фазы построения.
 * @see Config
 * Пример типового использования класса ConfigBuilder:
 * @include ConfigBuilder_example.cpp
 */
class ConfigBuilder 
{
public:
    virtual ~ConfigBuilder() throw() {}
    /**
     * Получение доступа к построенному экземпляру класса Config.
     */
    virtual Config *getConfig() throw(ConfigBuilderError) = 0;
    /**
     * Этап 2: построение протокола.
     */
    virtual void buildProtocol() throw(ConfigBuilderError) = 0;
    /**
     * Этап 1: построение свойств.
     */
    virtual void buildProperties() throw(ConfigBuilderError) = 0;
    /**
     * Этап 3: построение фабрик.
     */
    virtual void buildFactories() throw(ConfigBuilderError) = 0;
    /**
     * Этап 4: построение объектов.
     */
    virtual void buildObjects() throw(ConfigBuilderError) = 0;
    /**
     * Этап 5: построение задач.
     */
    virtual void buildTasks() throw(ConfigBuilderError) = 0;
    /**
     * Установка листенеров для отслеживания изменений процесса построения.
     * @see ConfigBuilderListener
     */
    virtual void setListener( ConfigBuilderListener* ) throw(ConfigBuilderError) = 0;
    /**
     * Установка листенера для отслеживания изменений и проверок значений свойств.
     * @see ConfigBuilderPropertyChangeListener
     */
    virtual void setPropertyChangeListener( ConfigBuilderPropertyChangeListener* ) throw(ConfigBuilderError) = 0;
};

END_CXXHTC_NAMESPACE

#endif // __KISC__CXXHTC__ConfigBuilder__H__

