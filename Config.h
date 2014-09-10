/**
 *  Configration parameters abstraction
 *  $Id: Config.h 216 2011-08-18 10:54:36Z dantld $
 */

#ifndef __KISC__CXXHTC__Config__H__
#define __KISC__CXXHTC__Config__H__

#include <cxxhtc_defs.h>
#include <stdexcept>
#include <string>
#include <list>

/**
 * @defgroup Configurations Классы конфигурации приложения
 */

BEGIN_CXXHTC_NAMESPACE

using std::domain_error;
using std::list;

class AbstractFactory;

class ConfigError : public std::exception {
    std::string _message;
public:
    ConfigError( const char* msg );
    ~ConfigError() throw();
    const char* what() const throw();
};
/*
class ConfigChangeListener() {
protected:
    ConfigChangeListener() throw();
    virtual ~ConfigChangeListener() throw();
public:
    virtual const std::string& getParamsRE() const throw() = 0;
    virtual void onParamChanged( const std::string& paramName ) const throw() = 0;
};
*/

class Config;

class ConfigDestroyListenerError : public std::exception
{
public:
    ConfigDestroyListenerError() throw();
    ~ConfigDestroyListenerError() throw();
};

/**
 * @ingroup Configurations
 * ConfigDestroyListener - это листенер,
 * который ожидает события уничтожения в реализациях класса Config.
 * Этот листенер применяется в случаях, когда нужны специальные действия
 * при уничтожении ваших объектов.
 * @see Config::setDestroyListener
 */
class ConfigDestroyListener {
public:
    ConfigDestroyListener() throw();
    virtual ~ConfigDestroyListener() throw();
    /**
     * Запускается перед уничтожением объекта.
     * Здесь Вы можете позаботиться о действиях при уничтожении интересующего объекта.
     * Также Вы можете уничтожить его самостоятельно.
     * @param config - указатель на объект конфигурации приложения.
     * @param objectName - имя уничтожаемого объекта.
     * @param object - указатель на экземпляр объекта.
     * @param factory - фабрика, используемая для уничтожения объекта.
     * Может быть равно NULL, если объект будет уничтожаться
     * без конфигурируемой фабрики, т.е. для уничтожения объекта
     * пользователь сам должен назначить фабрику.
     * @return true если объект уничтожен реализацией листенера
     * @exception ConfigDestroyListenerError возникает,
     * если происходят ошибки при попытке уничтожить объект.
     * Пример использования листенера:
     * @include ConfigDestroyListener_example.cpp
     */
    virtual bool onDestroyObject(
	Config *config,
	const std::string& objectName,
	void* object,
	AbstractFactory *factory
    ) throw(ConfigDestroyListenerError) = 0;

};

/**
 * @ingroup Configurations
 * Базовый интерфейс, представляющий абстракцию конфигурации.
 *
 */
class Config {
protected:
    Config();
public:
    virtual ~Config();

    /**
     * Поиск фабрики в конфигурационном объекте по заданному имени.
     * @param name - имя фабрики как указатель на строку, завершённую нулём.
     * @return Возвращаемое значение - указатель на фабрику.
     * @exception ConfigError возникает, если фабрика с запрошенным именем не существует.
     * @see AbstractFactory DynamicLoader
     */
    virtual void* getFactoryByName( const char* name )  const throw(ConfigError) = 0;
    /**
     * Поиск фабрики в конфигурационном объекте по заданному имени.
     * @param name - имя фабрики как ссылка на объект std::string.
     * @return Возвращаемое значение - указатель на фабрику.
     * @exception ConfigError возникает, если фабрика с запрошенным именем не существует.
     * @see AbstractFactory DynamicLoader
     */
    virtual void* getFactoryByName( const std::string& name )  const throw(ConfigError) = 0;

    /**
     * Поиск объекта в конфигурационном объекте по заданному имени.
     * @param name - имя объекта как указатель на строку, завершённую нулём.
     * @return Возвращаемое значение - указатель на объект.
     * @exception ConfigError возникает, если объект с запрошенным именем не существует.
     */
    virtual void* getObjectByName( const char* name )   const throw(ConfigError) = 0;
    /**
     * Поиск объекта в конфигурационном объекте по заданному имени.
     * @param name - имя объекта как ссылка на объект std::string.
     * @return Возвращаемое значение - указатель на объект.
     * @exception ConfigError возникает, если объект с запрошенным именем не существует.
     */
    virtual void* getObjectByName( const std::string& name )   const throw(ConfigError) = 0;
    /**
     * Поиск задачи в конфигурационном объекте по заданному имени.
     * @param name - имя задачи как указатель на строку, завершённую нулём.
     * @return Возвращаемое значение - указатель на задачу.
     * @exception ConfigError возникает, если задача с запрошенным именем не существует.
     * @see ATask
     */
    virtual void* getTaskByName( const char* name )     const throw(ConfigError) = 0;
    /**
     * Поиск задачи в конфигурационном объекте по заданному имени.
     * @param name - имя задачи как ссылка на объект std::string.
     * @return Возвращаемое значение - указатель на задачу.
     * @exception ConfigError возникает, если задача с запрошенным именем не существует.
     * @see ATask
     */
    virtual void* getTaskByName( const std::string& name )     const throw(ConfigError) = 0;
    /**
     * Поиск свойства в конфигурационном объекте по заданному имени.
     * @param name - имя свойства как указатель на строку, завершённую нулём.
     * @return Возвращаемое значение - свойство как объект std::string.
     * @exception ConfigError возникает, если свойство с запрошенным именем не существует.
     */
    virtual std::string getProperty( const char* name ) const throw(ConfigError) = 0;
    /**
     * Поиск свойства в конфигурационном объекте по заданному имени.
     * @param name - имя свойства как ссылка на объект std::string.
     * @return  Возвращаемое значение - значение свойства в виде объекта std::string.
     * @exception ConfigError возникает, если свойство с запрошенным именем не существует.
     */
    virtual std::string getProperty( const std::string& name ) const throw(ConfigError) = 0;
    /**
     * Return the password based on connection string and user name
     * @param s connection string
     * @param u user name
     */
    virtual std::string getPassword( const std::string& s, const std::string &u ) const throw(ConfigError) = 0;
    /**
     * Установить листенер уничтожения для предпринятия специальных действий
     * при уничтожении конфигурируемых объектов.
     * @param listener - указатель на экземпляр реализации интерфейса ConfigDestroyListener.
     * @see ConfigDestroyListener
     */
    virtual void setDestroyListener( ConfigDestroyListener* listener ) throw() = 0;
};


class ModifiableConfig {
protected:
    ModifiableConfig();
public:
    virtual ~ModifiableConfig();
    virtual void setObject(int, const void*) throw(ConfigError) = 0;
    virtual void setProp( const std::string&, const std::string& ) throw(ConfigError) = 0;
};

BEGIN_CXXHTC_CONFIG_NAMESPACE

/**
 * @ingroup Configurations
 * SimpleConfig - это расширение интерфейса Config, которое
 * вводит новые методы модификации.
 * SimpleConfig по умолчанию имеет реализацию в библиотеке C++HTC.
 * Реализация по умолчанию хорошо применима для тестирования и
 * простых приложений, когда конфигурация генерируется "на лету" и
 * не требуется комплексное построение процесса.
 * @see Config
 */
class SimpleConfig : public Config {
public:
    /**
     * Установка фабрики с заданным именем.
     * @param name - имя фабрики как указатель на строку, завершённую нулём.
     * @param factory - указатель на фабрику.
     * @exception ConfigError возникает, если фабрика с таким именем уже существует.
     */
    virtual void setFactory( const char* name, void* factory ) throw(ConfigError) = 0;
    /**
     * Установка фабрики с заданным именем.
     * @param name - имя фабрики как ссылка на объект std::string.
     * @param factory - указатель на фабрику.
     * @exception ConfigError возникает, если фабрика с таким именем уже существует.
     */
    virtual void setFactory( const std::string& name, void* factory ) throw(ConfigError) = 0;

    /**
     * Установка объекта с заданным именем.
     * @param name - имя объекта как указатель на строку, завершённую нулём.
     * @param object - указатель на объект.
     * @exception ConfigError возникает, если фабрика с таким именем уже существует.
     */
    virtual void setObject( const char* name, void* object ) throw(ConfigError) = 0;
    /**
     * Установка объекта с заданным именем.
     * @param name - имя объекта как ссылка на объект std::string.
     * @param object - указатель на объект.
     * @exception ConfigError возникает, если фабрика с таким именем уже существует.
     */
    virtual void setObject( const std::string& name, void* object ) throw(ConfigError) = 0;

    /**
     * Установка задачи с заданным именем.
     * @param name - имя задачи как указатель на строку, завершённую нулём.
     * @param task - указатель на задачу.
     * @exception ConfigError возникает, если задача с таким именем уже существует.
     * @see ATask
     */
    virtual void setTask( const char* name, void* task ) throw(ConfigError) = 0;
    /**
     * Установка задачи с заданным именем.
     * @param name - имя задачи как ссылка на объект std::string.
     * @param task - указатель на задачу.
     * @exception ConfigError возникает, если задача с таким именем уже существует.
     * @see ATask
     */
    virtual void setTask( const std::string& name, void* task ) throw(ConfigError) = 0;

    /**
     * Установка значения свойства с заданным именем.
     * @param name - имя свойства как указатель на строку, завершённую нулём.
     * @param value - значение свойства как строка, завершённая нулём.
     * @exception ConfigError возникает, если свойство с таким именем уже существует.
     */
    virtual void setProperty( const char* name, const char* value ) throw(ConfigError) = 0;
    /**
     * Установка значения свойства с заданным именем.
     * @param name - имя свойства как ссылка на объект std::string.
     * @param value - значение свойства как ссылка на объект std::string.
     * @exception ConfigError возникает, если свойство с таким именем уже существует.
     */
    virtual void setProperty( const std::string& name, const std::string& value ) throw(ConfigError) = 0;
    /**
     * Установка значения свойства с заданным именем.
     * @param name - имя свойства как указатель на строку, завершённую нулём.
     * @param value - значение свойства как целое число.
     * @exception ConfigError возникает, если свойство с таким именем уже существует.
     */
    virtual void setProperty( const char* name, int value ) throw(ConfigError) = 0;
    /**
     * Установка значения свойства с заданным именем.
     * @param name - имя свойства как ссылка на объект std::string.
     * @param value - значение свойства как целое число.
     * @exception ConfigError возникает, если свойство с таким именем уже существует.
     */
    virtual void setProperty( const std::string& name, int value) throw(ConfigError) = 0;
    
    /**
     * Создание реализации по умолчанию интерфейса SimpleConfig.
     * @return Возвращаемое значение - указатель на экземпляр объекта SimpleConfig.
     */
    static SimpleConfig* createInstance() throw();
};

END_CXXHTC_CONFIG_NAMESPACE

END_CXXHTC_NAMESPACE

#endif // __KISC__CXXHTC__Config__H__

