/**
 * $Id: ConfigHelper.cpp 236 2013-05-13 04:40:08Z dantld $
 * Config helper
 */

#include "Config.h"
#include "ConfigHelper.h"
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <limits.h>
#include "../Protocol.h"

BEGIN_CXXHTC_NAMESPACE

using std::string;
using std::list;
class Config;

#define THROW_CONFIG_HELPER_ERROR( text, name ) \
{ \
    char buffer[256] = ""; \
    assert(0<=snprintf(buffer, 256, "%s. Parameter name: %s.", text, name)); \
    throw ConfigHelperError(buffer); \
}

ConfigHelperError::ConfigHelperError( const char* what ) :
    runtime_error(what)
{
}
    

DefaultValues::DefaultValues() {
}

DefaultValues::DefaultValues(const char *p1, ... ) throw() {
    va_list args;
    int argno = 0;
    va_start( args, p1 );
    if( p1 != NULL ) {
	insert( string( p1 ) );
	const char *p = 0;
	while( (p = va_arg( args, const char* )) != NULL ) {
	    insert( string(p) );
	}
    }
    va_end( args );
}

DefaultValues::DefaultValues( const DefaultValues& o ) throw() :
    set<string>(o)
{
}

DefaultValues& DefaultValues::operator=( const DefaultValues& o ) throw() {
    set<string>::operator=(o);
    return *this;
}
       
DefaultValues::~DefaultValues() throw() {
}

/**
 * ConfigHelper wrapper class who can work with implementations of @Config iface
 */
ConfigHelper::ConfigHelper() {
}

ConfigHelper::ConfigHelper( const Config* config ) :
    _cfgType(ConfigHelper::COMMON),
    _config(config)
{
}

ConfigHelper::ConfigHelper(
    ConfigType cfgType,
    const std::string& prefix,
    const Config* config ) 
    :
    _cfgType(cfgType),
    _prefix(prefix),
    _config(config)
{
}

ConfigHelper::~ConfigHelper() {
}

static std::string makePropName( 
    ConfigHelper::ConfigType cfgType,
    const std::string& prefix,
    const std::string& propName
    )
{
    std::string fullPropName;
    switch( cfgType ) {
	case ConfigHelper::COMMON:
	    return propName;
	case ConfigHelper::APPS:
	    fullPropName = "app.";
	    break;
	case ConfigHelper::TASK:
	    fullPropName = "task.";
	    break;
	case ConfigHelper::OBJECT:
	    fullPropName = "object.";
	    break;
    }
    if( prefix.length() > 0 ) {
	fullPropName += prefix + "." + propName;
    } else {
	fullPropName += propName;
    }
    return fullPropName;
}

string ConfigHelper::getString( const string& name ) throw(ConfigError)
{
    std::string n = makePropName( _cfgType, _prefix, name );
    return _config->getProperty( n.c_str() );
}
    
string ConfigHelper::getString( const string& name, string defVal ) throw()
try { 
    return getString( name );
} catch(const std::exception & ) {
    return defVal;
}

string ConfigHelper::getString(
    const string& name,
    DefaultValues defVals 
    ) throw(ConfigError,ConfigHelperError)
{
    string val = getString( name.c_str() );
    DefaultValues::iterator it = defVals.find( val );
    if( it == defVals.end() )
	THROW_CONFIG_HELPER_ERROR( "Failed find value in set", name.c_str() );
    return val;
}

string ConfigHelper::getString( 
    const string& name,
    string defVal, 
    DefaultValues defVals 
    )
    throw(ConfigHelperError)
{
    string val;
    try { 
	std::string n = makePropName( _cfgType, _prefix, name );
	val = _config->getProperty( n.c_str() );
    } catch( const ConfigError & ) {
	val = defVal;
    }
    DefaultValues::iterator it = defVals.find( val );
    if( it == defVals.end() ) {
	THROW_CONFIG_HELPER_ERROR( "Failed find value in set", name.c_str() );
    }
    return val;
}

int ConfigHelper::getInt( const string& name )
    throw(ConfigHelperError)
try { 
    std::string n = makePropName( _cfgType, _prefix, name );
    string val = _config->getProperty( n.c_str() );
    int intVal = 0;
    if( sscanf( val.c_str(), "%d", &intVal ) == 1 ) {
	return intVal;
    }
    THROW_CONFIG_HELPER_ERROR( "Failed cast to integer", name.c_str() );
} catch( const ConfigError & ) {
    THROW_CONFIG_HELPER_ERROR( "No such property of integer type", name.c_str() );
}
 

int    ConfigHelper::getInt(    const string& name, int defVal    ) throw(ConfigHelperError)
try { 
    std::string n = makePropName( _cfgType, _prefix, name );
    string val = _config->getProperty( n.c_str() );
    int intVal = 0;
    if( sscanf( val.c_str(), "%d", &intVal ) == 1 ) {
	return intVal;
    }
    THROW_CONFIG_HELPER_ERROR( "Failed cast to integer", name.c_str() );
} catch( const ConfigError & ) {
    return defVal;
}

int ConfigHelper::getInt(
    const string& name,
    int defVal,
    int minVal,
    int maxVal
    )
    throw(ConfigHelperError)
try { 
    std::string n = makePropName( _cfgType, _prefix, name );
    string val = _config->getProperty( n.c_str() );
    int intVal = 0;
    if( sscanf( val.c_str(), "%d", &intVal ) == 1 ) {
	if( intVal < minVal ) {
	    THROW_CONFIG_HELPER_ERROR( "Value less than minimum", name.c_str() );
	}
	if( intVal > maxVal ) {
	    THROW_CONFIG_HELPER_ERROR( "Value greater than maximum", name.c_str() );
	}
	return intVal;
    }
    THROW_CONFIG_HELPER_ERROR( "Failed cast to integer", name.c_str() );
} catch( const ConfigError & ) {
    return defVal;
}

/**
 * Read the integer values list. Each element of list divided by semicolon symbol.
 *
 */
std::list<int> ConfigHelper::getIntList(
    const std::string& name 
    ) throw(ConfigError,ConfigHelperError)
{
    std::list<int> retVal;
    std::string n = makePropName( _cfgType, _prefix, name );
    string val = _config->getProperty( n.c_str() );
    if( val.length() == 0 ) {
	THROW_CONFIG_HELPER_ERROR( "Coudn`t process empty string", name.c_str() );
    }
    while( 1 ) {
	std::string::size_type pr_pos = 0;
	std::string::size_type nx_pos = 0;
	std::stringstream vs;
	nx_pos = val.find( ';' );
	std::string::size_type len;
	if( nx_pos == std::string::npos ) len = val.length() - pr_pos;
	else len = nx_pos - pr_pos;
	if( len == 0 ) {
	    THROW_CONFIG_HELPER_ERROR( "Failed cast value to integer: zero length", name.c_str() );
	} else if( len > 16 ) {
	    THROW_CONFIG_HELPER_ERROR( "Failed cast value to integer:  length exceed 16", name.c_str() );
	}
	std::string strVal = val.substr( pr_pos, len );
	int intVal = INT_MAX;
	if( strVal.length() > 2 && strVal[0] == '0' && strVal[1] == 'x' ) {
	    if( sscanf( strVal.c_str(), "%x", &intVal )  != 1 )
		THROW_CONFIG_HELPER_ERROR( "Failed cast value to integer", name.c_str() );
	} else if( strVal.length() > 1 && strVal[0] == '0' ) {
	    if( sscanf( strVal.c_str(), "%o", &intVal )  != 1 )
		THROW_CONFIG_HELPER_ERROR( "Failed cast value to integer", name.c_str() );
	} else {
	    if( sscanf( strVal.c_str(), "%d", &intVal )  != 1 )
		THROW_CONFIG_HELPER_ERROR( "Failed cast value to integer", name.c_str() );
	}
	//fprintf( stderr, "strVal \"%s\" == %d ", strVal.c_str(), intVal );
	retVal.push_back( intVal );
	if( nx_pos == std::string::npos || nx_pos == val.length() - 1 ) break;
	val = val.substr( nx_pos + 1 );
    }
    return retVal;
}
/**
 * Read the string values list. Each element of list divided by semicolon symbol.
 *
 */
std::list<std::string> ConfigHelper::getStringList( const std::string& name ) throw(ConfigError,ConfigHelperError) {
    std::list<std::string> retVal;
    std::string n = makePropName( _cfgType, _prefix, name );
    string val = _config->getProperty( n.c_str() );
    if( val.length() == 0 ) {
	return retVal;
    }
    while( 1 ) {
	std::string::size_type pr_pos = 0;
	std::string::size_type nx_pos = 0;
	std::stringstream vs;
	nx_pos = val.find( ';' );
	std::string::size_type len;
	if( nx_pos == std::string::npos ) len = val.length() - pr_pos;
	else len = nx_pos - pr_pos;
	if( len == 0 ) {
	    THROW_CONFIG_HELPER_ERROR( "Failed read string list value: zero length", name.c_str() );
	}
	std::string strVal = val.substr( pr_pos, len );
	retVal.push_back( strVal );
	if( nx_pos == std::string::npos || nx_pos == val.length() - 1 ) break;
	val = val.substr( nx_pos + 1 );
    }
    return retVal;
}


int ConfigHelper::getOct( const string& name ) throw(ConfigError,ConfigHelperError)
{
    std::string n = makePropName( _cfgType, _prefix, name );
    string val = _config->getProperty( n.c_str() );
    int octVal = 0;
    if( sscanf( val.c_str(), "%o", &octVal ) == 1 ) {
	return octVal;
    }
    THROW_CONFIG_HELPER_ERROR( "Failed cast to octal integer", name.c_str() );
}


int ConfigHelper::getOct( const string& name, int defVal ) throw(ConfigHelperError)
try {
    std::string n = makePropName( _cfgType, _prefix, name );
    string val = _config->getProperty( n.c_str() );
    return getOct( name );
} catch( const ConfigError & ) {
    return defVal;
}
 
    

bool ConfigHelper::getBool( const string& name ) throw(ConfigError,ConfigHelperError)
{ 
    std::string n = makePropName( _cfgType, _prefix, name );
    string val = _config->getProperty( n.c_str() );
    std::transform( val.begin(), val.end(), val.begin(), (int(*)(int)) std::toupper );
    if( val == "TRUE" || val == "1" ) {
	return true;
    } else if( val == "FALSE" || val == "0" ) {
	return false;
    }
    THROW_CONFIG_HELPER_ERROR( "Failed cast to bool", name.c_str() );
}

bool   ConfigHelper::getBool(   const string& name, bool defVal   ) throw(ConfigHelperError)
try { 
    return getBool( name );
} catch( const ConfigError & ) {
    return defVal;
}

END_CXXHTC_NAMESPACE
