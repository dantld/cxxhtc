/**
 * $Id: ConfigHelper.h 226 2012-04-06 08:43:35Z dantld $
 * Config helper
 */

#ifndef __CXXHTC__LIBRARY__CLASS__H__
#define __CXXHTC__LIBRARY__CLASS__H__

#include <cxxhtc_defs.h>
#include <string>
#include <set>
#include <stdexcept>

BEGIN_CXXHTC_NAMESPACE

using std::string;
using std::set;
using std::runtime_error;

class Config;

class ConfigHelperError : public runtime_error {
public:
    ConfigHelperError( const char* what );
};

class DefaultValues : public set<string> {
    DefaultValues();
public:
    explicit DefaultValues(const char *, ... ) throw();
    DefaultValues( const DefaultValues& ) throw();
    ~DefaultValues() throw();
    DefaultValues& operator=( const DefaultValues& ) throw();
};

/**
 * @ingroup Configurations
 * ConfigHelper - это обёрточный (wrapper) класс для помощи 
 * в работе с реализациями интерфейса Config при чтении параметров.
 * @see Config
 * Пример использования ConfigHelper:
 * @include ConfigHelper_example.cpp
 */
class ConfigHelper {
    std::string _prefix;
    const Config *_config;
    ConfigHelper();
public:
    enum ConfigType { COMMON, APPS, TASK, OBJECT };
    /**
     * Основной конструктор для создания экземпляра класса.
     * @param config - объект класса Config Вашего приложения.
     */
    ConfigHelper( const Config* config );
    /**
     * @param cfgType meaning of prefix object, task or application
     * @param prefix object or task name prefix which uses for making prop name
     * @param config configuration for reading
     */
    ConfigHelper( ConfigType cfgType, const std::string& prefix, const Config* config );
    ~ConfigHelper();
    /**
     * Прочитать текст по имени.
     * @param name - имя запрашиваемого параметра.
     * @return Возвращается значение запрошенного параметра.
     * @exception ConfigError возникает, если параметра с заданным 
     * именем не существует.
     */
    string getString( const string& name ) throw(ConfigError);
    /**
     * Прочитать текст по имени. Если параметр не существует,
     * то возвращается значение по умолчанию.
     * @param name - имя запрашиваемого параметра.
     * @param defVal - значение по умолчанию для заданного параметра.
     * @return Возвращается значение заданного параметра.
     */
    string getString( const string& name, string defVal ) throw();
    /**
     * Прочитать текст по имени. 
     * Значение параметра должно соответствовать набору
     * допустимых значений, определяемых вторым параметром.
     * @param name - имя запрашиваемого параметра.
     * @param defVals - набор допустимых значений.
     * @return Возвращается значение заданного параметра.
     * @exception ConfigError возникает, если заданный параметр не существует.
     * @exception ConfigHelperError возникает, если значение параметра 
     * не соответствует набору defVals.
     */
    string getString( const string& name, DefaultValues defVals ) throw(ConfigError,ConfigHelperError);
    /**
     * Прочитать текст по имени. 
     * Значение параметра должно соответствовать набору
     * допустимых значений, определяемых третьим параметром.
     * Если параметр не существует, то возвращается значение по умолчанию.
     * @param name - имя запрашиваемого параметра.
     * @param defVal - значение по умолчанию для запрошенного параметра.
     * @param defVals - набор допустимых значений.
     * @return Возвращается значение заданного параметра.
     * @exception ConfigHelperError возникает, если значение параметра 
     * не соответствует набору defVals.
     */
    string getString( const string& name, string defVal, DefaultValues defVals ) throw(ConfigHelperError);
    
    /**
     * Прочитать параметр как целочисленное значение.
     * @param name - имя запрашиваемого параметра.
     * @exception ConfigHelperError возникает, если запрашиваемый параметр не существует.
     */
    int    getInt(    const string& name ) throw(ConfigHelperError);
    /**
     * Прочитать параметр как целочисленное значение.
     * Если параметр не существует, возвращается значение по умолчанию.
     * @param name - имя запрашиваемого параметра.
     * @param defVal - значение по умолчанию.
     */
    int    getInt(    const string& name, int defVal    ) throw(ConfigHelperError);
    /**
     * Прочитать параметр как целочисленное значение.
     * Значение параметра должно находиться в определенном диапазоне значений.
     * Если параметр не существует, возвращается значение по умолчанию.
     * @param name - имя запрашиваемого параметра.
     * @param defVal - значение параметра по умолчанию.
     * @param minVal - минимальное значение параметра.
     * @param maxVal - максимальное значение параметра.
     * @exception ConfigHelperError возникает, если запрашиваемый параметр не существует.
     */
    int    getInt(    const string& name, int defVal, int minVal, int maxVal ) throw(ConfigHelperError);
    /**
     * Read the integer values list. Each element of list divided by semicolon symbol.
     *
     */
    std::list<int> getIntList( const std::string& name ) throw(ConfigError,ConfigHelperError);
    /**
     * Read the string values list. Each element of list divided by semicolon symbol.
     *
     */
    std::list<std::string> getStringList( const std::string& name ) throw(ConfigError,ConfigHelperError);
    /**
     * Прочитать параметр как целочисленное значение.
     * Предполагается, что параметр записан в восьмеричной системе.
     */
    int    getOct(    const string& name ) throw(ConfigError,ConfigHelperError);
    /**
     * Прочитать параметр как целочисленное значение.
     * Предполагается, что параметр записан в восьмеричной системе.
     * В случае отсуствия параметра, вернётся значение по умолчанию.
     */
    int    getOct(    const string& name, int defVal    ) throw(ConfigHelperError);
    
    /**
     * Прочитать параметр как булево значение.
     * Допустимые значения - 0, 1, false, true, FALSE или TRUE.
     * @exception ConfigError возникает, если параметр не существует.
     * @exception ConfigHelperError возникает, если параметр существует, но неверно
     * объявлен.
     */
    bool   getBool(   const string& name ) throw(ConfigError,ConfigHelperError);
    /**
     * Прочитать параметр как булево значение.
     * Допустимые значения - 0, 1, false, true, FALSE или TRUE.
     * Если параметр не существует, возвращается значение по умолчанию.
     * @param name - имя запрашиваемого параметра.
     * @param defVal - значение параметра по умолчанию.
     * @exception ConfigHelperError возникает, если параметр существует, но неверно
     * объявлен.
     */
    bool   getBool(   const string& name, bool defVal   ) throw(ConfigHelperError);

private:
    ConfigType _cfgType;
};

END_CXXHTC_NAMESPACE
#endif // __CXXHTC__LIBRARY__CLASS__H__
