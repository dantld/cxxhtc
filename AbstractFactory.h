/**
 * $Id: AbstractFactory.h 107 2009-12-21 04:12:26Z olegm $
 * C++HTC
 * Abstract factory interface.
 */

#ifndef __CXXHTC__CLASS__AbstractFactory__IFACE__H__
#define __CXXHTC__CLASS__AbstractFactory__IFACE__H__

#include <cxxhtc_defs.h>
#include <exception>
#include <DynamicLoader.h>

BEGIN_CXXHTC_NAMESPACE

class Config;

class AbstractFactoryError : public std::exception {
public:
    AbstractFactoryError() throw();
    ~AbstractFactoryError() throw();
};

/**
 * @ingroup DynamicLoading
 * AbstractFactory - это общий интерфейс фабрики.
 * Во многих случаях, когда Вам необходимо создать динамически
 * загружаемую реализацию интерфейса из этой библиотеки
 * или Вашего собственного интерфейса, в любом случае 
 * требуется применить интерфейс AbstractFactory в 
 * Вашем модуле (Вашей реализации модуля).
 * @see DynamicLoader
 */
class AbstractFactory {
protected:
    AbstractFactory() throw(AbstractFactoryError);
public:
    virtual ~AbstractFactory() throw();
    /**
     * Создание экземпляра продукта.
     * @param name - имя создаваемого объекта. Это необязательный параметр,
     * но если Ваша реализация фабрики или продукта требует имени,
     * например, для конфигурационных целей, хорошая идея поместить его именно здесь.
     * @param config - глобальный объект конфигурации класса Config.
     * @return Возвращаемое значение - указатель на новый экземпляр продукта фабрики, 
     * Вам нужно назначить этот указатель на свой тип продукта.
     */
    virtual void* createInstance( const char* name, Config* config ) throw(AbstractFactoryError) = 0;
    /**
     * Уничтожение экземпляра продукта, созданного этой фабрикой.
     * @param object - указатель на экземпляр, полученный ранее через вызов createInstance.
     */
    virtual void  destroyInstance( void* object ) throw(AbstractFactoryError) = 0;
    /**
     * Общий шаблон для использования фабрики с DynamicLoader.
     */
    typedef DynamicLoader<AbstractFactory> Loader;
};

END_CXXHTC_NAMESPACE
#endif //__CXXHTC__CLASS__AbstarctFactory__IFACE__H__
