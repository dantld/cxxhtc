/**
 * $Id: Config.cpp 216 2011-08-18 10:54:36Z dantld $
 * All implementations for Config interface and related classes.
 */

#include "../Config.h"
#include "../DynamicLoader.h"
#include <map>
#include <string>
#include <sstream>
#include <stdio.h>

using namespace std;

BEGIN_CXXHTC_NAMESPACE

ConfigError::ConfigError(const char * message )  :
    _message( message )
{
}

ConfigError::~ConfigError()  throw()
{
}

const char* ConfigError::what() const throw() {
    return _message.c_str();
}

Config::Config() {
}

Config::~Config() {
}

ModifiableConfig::ModifiableConfig() {
}

ModifiableConfig::~ModifiableConfig() {
}


BEGIN_CXXHTC_CONFIG_NAMESPACE

typedef DynamicLoader< AbstractFactory > AbstractFactoryLoader;

typedef map< std::string, void* > FactoriesMap;
typedef map< std::string, void* > TasksMap;
typedef map< std::string, void* > ObjectsMap;
typedef map< std::string, std::string > PropertiesMap;

class SimpleConfigImpl : public SimpleConfig {
    FactoriesMap  _factories;
    TasksMap      _tasks;
    ObjectsMap    _objects;
    PropertiesMap _properties;
    ConfigDestroyListener *_listener;


    void* getFactoryByName( const char* )  const throw(ConfigError);
    void* getFactoryByName( const std::string& )  const throw(ConfigError);

    void* getObjectByName( const char* )   const throw(ConfigError);
    void* getObjectByName( const std::string& )   const throw(ConfigError);

    void* getTaskByName( const char* )     const throw(ConfigError);
    void* getTaskByName( const std::string& )     const throw(ConfigError);

    std::string getProperty( const char* ) const throw(ConfigError);
    std::string getProperty( const std::string& ) const throw(ConfigError);

    std::string getPassword( const std::string& s, const std::string &u ) const throw(ConfigError) { return ""; }

    void setDestroyListener( ConfigDestroyListener* ) throw();

    void setFactory( const char*, void* ) throw(ConfigError);
    void setFactory( const std::string&, void* ) throw(ConfigError);

    void setObject( const char*, void* ) throw(ConfigError);
    void setObject( const std::string&, void* ) throw(ConfigError);

    void setTask( const char*, void* ) throw(ConfigError);
    void setTask( const std::string&, void* ) throw(ConfigError);

    void setProperty( const char*, const char* ) throw(ConfigError);
    void setProperty( const std::string&, const std::string& ) throw(ConfigError);
    void setProperty( const char*, int ) throw(ConfigError);
    void setProperty( const std::string&, int ) throw(ConfigError);
public:
    SimpleConfigImpl() throw();
    ~SimpleConfigImpl() throw();
};

SimpleConfigImpl::SimpleConfigImpl() throw() :
    _listener(0)
{
}

SimpleConfigImpl::~SimpleConfigImpl() throw() {
}

void* SimpleConfigImpl::getFactoryByName( const char* k )  const throw(ConfigError) {
    FactoriesMap::const_iterator it = _factories.find( std::string(k) );
    if( it == _factories.end() ) {
	ostringstream os;
	os << "Can not find factory with name: \"" << k << "\"" << ends;
	throw ConfigError( os.str(). c_str() );
    }
    return it->second;
}
void* SimpleConfigImpl::getFactoryByName( const std::string& n )  const throw(ConfigError) {
    return getFactoryByName( n.c_str() );
}

void* SimpleConfigImpl::getObjectByName( const char* k )   const throw(ConfigError) {
    ObjectsMap::const_iterator it = _objects.find( std::string(k) );
    if( it == _objects.end() ) {
	ostringstream os;
	os << "Can not find objects by name: \"" << k << "\"" <<  ends;
	throw ConfigError( os.str(). c_str() );
    }
    return it->second;
}
void* SimpleConfigImpl::getObjectByName( const std::string& n )   const throw(ConfigError) {
    return getObjectByName( n.c_str() );
}

void* SimpleConfigImpl::getTaskByName( const char* k )     const throw(ConfigError) {
    TasksMap::const_iterator it = _tasks.find( std::string(k) );
    if( it == _tasks.end() ) {
	ostringstream os;
	os << "Can not find tasks by name: \"" << k << "\"" << ends;
	throw ConfigError( os.str(). c_str() );
    }
    return it->second;
}
void* SimpleConfigImpl::getTaskByName( const std::string& n )     const throw(ConfigError) {
    return getTaskByName( n.c_str() );
}

std::string SimpleConfigImpl::getProperty( const char* k ) const throw(ConfigError) {
    PropertiesMap::const_iterator it = _properties.find( std::string(k) );
    if( it == _properties.end() ) {
	ostringstream os;
	os << "Can not find property by name: \"" << k << "\"" << ends;
	throw ConfigError( os.str(). c_str() );
    }
    return it->second;
}
std::string SimpleConfigImpl::getProperty( const std::string& n ) const throw(ConfigError) {
    return getProperty( n.c_str() );
}

void SimpleConfigImpl::setDestroyListener( ConfigDestroyListener* l ) throw() {
}

void SimpleConfigImpl::setFactory( const char* k, void* v ) throw(ConfigError) {
    FactoriesMap::const_iterator it = _factories.find( std::string(k) );
    if( it != _factories.end() ) {
	ostringstream os;
	os << "Factory with name: \"" << k << "\" already exists" << ends;
	throw ConfigError( os.str(). c_str() );
    }
    if( !_factories.insert( FactoriesMap::value_type( string(k), v ) ).second ) {
	ostringstream os;
	os << "Insert factory failed: \"" << k << "\"" << ends;
	throw ConfigError( os.str(). c_str() );
    }
}
void SimpleConfigImpl::setFactory( const std::string& k, void* v ) throw(ConfigError) {
    setFactory( k.c_str(), v );
}

void SimpleConfigImpl::setObject( const char* k, void* v ) throw(ConfigError) {
    ObjectsMap::const_iterator it = _objects.find( std::string(k) );
    if( it != _objects.end() ) {
	ostringstream os;
	os << "Object with name: \"" << k << "\" already exists" << ends;
	throw ConfigError( os.str(). c_str() );
    }
    if( !_objects.insert( ObjectsMap::value_type( string(k), v ) ).second ) {
	ostringstream os;
	os << "Insert object failed: \"" << k << "\"" << ends;
	throw ConfigError( os.str(). c_str() );
    }
}
void SimpleConfigImpl::setObject( const std::string& k, void* v ) throw(ConfigError) {
    setObject( k.c_str(), v );
}

void SimpleConfigImpl::setTask( const char* k, void* v ) throw(ConfigError) {
    TasksMap::const_iterator it = _tasks.find( std::string(k) );
    if( it != _tasks.end() ) {
	ostringstream os;
	os << "Task with name: \"" << k << "\" already exists" << ends;
	throw ConfigError( os.str(). c_str() );
    }
    if( !_tasks.insert( TasksMap::value_type( string(k), v ) ).second ) {
	ostringstream os;
	os << "Insert task failed: \"" << k << "\"" << ends;
	throw ConfigError( os.str(). c_str() );
    }
}
void SimpleConfigImpl::setTask( const std::string& k, void* v ) throw(ConfigError) {
    setTask( k.c_str(), v );
}

void SimpleConfigImpl::setProperty( const char* k, const char* v ) throw(ConfigError) {
    PropertiesMap::iterator it = _properties.find( std::string(k) );
    if( it != _properties.end() ) {
	it->second = string(v);
	return;
    }
    if( !_properties.insert( PropertiesMap::value_type( string(k), string(v) ) ).second ) {
	ostringstream os;
	os << "Insert property failed: \"" << k << "\"" << ends;
	throw ConfigError( os.str(). c_str() );
    }
}
void SimpleConfigImpl::setProperty( const std::string& k, const std::string& v ) throw(ConfigError) {
    setProperty( k.c_str(), v.c_str() );
}
void SimpleConfigImpl::setProperty( const char* k, int v ) throw(ConfigError) {
    char intCVal[32];
    snprintf( intCVal, 32, "%d", v );
    setProperty( k, intCVal );
}
void SimpleConfigImpl::setProperty( const std::string& k, int v ) throw(ConfigError) {
    setProperty( k.c_str(), v );
}


SimpleConfig* SimpleConfig::createInstance() throw() {
    return new SimpleConfigImpl;
}

END_CXXHTC_CONFIG_NAMESPACE

END_CXXHTC_NAMESPACE
