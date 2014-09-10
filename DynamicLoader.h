/**
 * Dynamic loader
 * $Id: DynamicLoader.h 161 2010-03-11 04:57:57Z Andrey.Volkov $
 */

#ifndef __DYNAMIC__LOADER__CLASS__H__
#define __DYNAMIC__LOADER__CLASS__H__

#include <cxxhtc_defs.h>
#include <string>
#include <stdexcept>
#include <typeinfo>
#include <dlfcn.h>
#include <sstream>
/**
 * @defgroup DynamicLoading Динамическая загрузка выполняемых модулей.
 */

BEGIN_CXXHTC_NAMESPACE

using std::string;
using std::runtime_error;
using std::bad_cast;
using std::exception;

/**
 * @ingroup DynamicLoading
 * Определение указателя для создания функции.
 * Вы должны определить такую функцию в вашем 
 * динамически загружаемом объекте.
 * Этот запрос должен создавать
 * экземпляр класса AbstractFactory. 
 * Этот символ должен быть экспортирован на этапе
 * компоновки вашего динамического объекта.
 */
typedef void* (*_CREATE_CALL)();
/**
 * @ingroup DynamicLoading
 * Определение указателя для уничтожения
 * функции. Вы должны определить эту функцию
 * в вашем динамически загружаемом объекте.
 * Вы должны передать указатель на экземпляр
 * класса AbstractFactory.
 * Этот символ должен быть экспортирован на этапе
 * компоновки вашего динамического объекта.
 */
typedef void (*_DESTROY_CALL)(void*);

/**
 * @ingroup DynamicLoading
 * DynamicLoader - это шаблонный класс
 * для динамической загрузки динамических 
 * библиотек с применением некоторого 
 * интерфейса.
 * Динамический объект должен экспортировать
 * два символа: "create" и "destroy". 
 * Эти вызовы должны создавать и уничтожать
 * экземпляры вашего применения интерфейса
 * AbstractFactory.
 * Пример использования DynamicLoader см. ниже:
 * @include DynamicLoader_example.cpp
 * О написании динамических модулей см. пример:
 * @include DynamicModule_example.cpp
 */
template<class T> class DynamicLoader {
    string _path;
    string _create_name;
    string _destroy_name;
    void  *_handle;
    _CREATE_CALL _create_call;
    _DESTROY_CALL _destroy_call;
public:
    /**
     * Наиболее важный конструктор класса DynamicLoader.
     * Конструктор загружает динамический объект
     * и пытается импортировать два необходимых
     * символа для создания и уничтожения.
     * По умолчанию значения имён символов объявлены 
     * как create и destroy.
     * Пожалуйста, прочтите описание для dlopen() по 
     * поводу путей поиска динамической библиотеки,
     * переменных окружения и т.д.
     * @param path - путь к динамическому объекту.
     * @param create_name - имя символа, используемого для создания фабрики.
     * @param destroy_name - имя символа, используемого для уничтожения фабрики.
     * @exception std::runtime_error возникает, если имеется ошибка 
     * при вызове dlopen() или dlsym().
     */
    DynamicLoader( const string& path, 
	const string &create_name = "create",
	const string &destroy_name = "destroy" ) : 
	_path(path), 
	_create_name(create_name),
	_destroy_name(destroy_name),
	_handle(NULL)
    {
	_handle = dlopen( _path.c_str(), RTLD_NOW|RTLD_LOCAL );
	if( _handle == NULL ) {
	    std::ostringstream os;
	    os << "Filed to open library: "
	       << _path
#if defined(__hpux)	       
	       << " error: " << dlerrno()
#endif	     
	       << " text: " << dlerror();
	    throw runtime_error( os.str().c_str() );
	}
	_create_call = (_CREATE_CALL)dlsym( _handle, _create_name.c_str() );
	if( _create_call == NULL ) {
	    std::ostringstream os;
	    os << "Failed to find symbol: "
	       << _create_name 
	       << " path: " << _path
#if defined(__hpux)	       
	       << " error: " << dlerrno()
#endif	     
	       << " text: " << dlerror();
	    throw runtime_error( os.str().c_str() );
	}
	_destroy_call = (_DESTROY_CALL)dlsym( _handle, _destroy_name.c_str() );
	if( _destroy_call == NULL ) {
	    std::ostringstream os;
	    os << "Failed to find symbol: "
	       << _destroy_name 
	       << " path: " << _path
#if defined(__hpux)	       
	       << " error: " << dlerrno()
#endif	     
	       << " text: " << dlerror();
	    throw runtime_error( os.str().c_str() );
	}
    }
    /**
     * Деструктор будет пытаться выгрузить динамический объект,
     * загруженный конструктором. Перед уничтожением DynamicLoader
     * проверяет, что фабрика или её продукт не используется
     * вашим приложением.
     * Для более полного понимания, пожалуйста, прочтите описание вызова dlclose().
     * @exception std::runtime_error возникает, если вызов dlclose() оказывается неудачным.
     */
    ~DynamicLoader() {
	if( _handle != NULL ) {
	    if( dlclose( _handle ) != 0 ) {
		std::ostringstream os;
		os << "Failed to unload library: "
	    	<< " path: " << _path
#if defined(__hpux)	       
	        << " error: " << dlerrno()
#endif	     
	        << " text: " << dlerror();
		throw runtime_error( os.str().c_str() );
	    }
	}
    }

    /**
     * Создание экземпляра объекта типа T.
     * @return Возвращаемое значение - указатель на экземпляр объекта типа T.
     * @exception std::exception если создание не удалось.
     */    
    T* create() throw(exception) {
	return dynamic_cast<T*>( (T*)(*_create_call)() );
    }
    /**
     * Уничтожение экземпляра объекта типа T.
     * @param object - указатель на экземпляр объекта типа T.
     * @exception std::exception если уничтожение не удалось.
     */
    void destroy( T* object ) throw(exception) {
	 (*_destroy_call)( object );
    }
};

END_CXXHTC_NAMESPACE

#endif //__DYNAMIC__LOADER__CLASS__H__

