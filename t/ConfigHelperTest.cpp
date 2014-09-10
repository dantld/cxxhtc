/**
 * $Id: ConfigHelperTest.cpp 236 2013-05-13 04:40:08Z dantld $
 *
 */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../Config.h"
#include "../ConfigHelper.h"

#include <map>
#include <string>
using namespace std;
using namespace kisc;


class MyConfig : public Config {
    typedef map< string, string > Properties;
    typedef map< string, string >::iterator PropertiesIterator;
    typedef map< string, string >::const_iterator PropertiesIteratorC;
    typedef map< string, string >::value_type ValueType;
    Properties _props;
public:
    MyConfig() {
	_props["test1.int.value"] = "1";
	_props["test1.int.max.value"] = "101";
	_props["test1.int.min.value"] = "0";
	_props["test2.bool.false"] = "false";
	_props["test2.bool.true"] = "true";
	_props["test2.bool.false.0"] = "0";
	_props["test2.bool.true.1"] = "1";
	_props["test3.string.info"] = "information";
	_props["test4.octal.mode"] = "644";
	_props["test5.bool.failed"] = "failed";
	_props["test6.octal.failed"] = "888";
	_props["test7.string.range.value"] = "hour";
	_props["test8.string.range.value"] = "day";
	_props["object.my_object_name.string.value"] = "this string value for object";
	_props["task.my_task_name.string.value"] = "this string value for task";
	_props["app.my_app_name.string.value"] = "this string value for application";
	_props["app.my_app_name.int.list"] = "0;0x1;0x02;03;4;5;6;7;010;9;10";
	_props["app.my_app_name.int.list.0"] = "0;1;2;3;4;bad-val;5;6;7;8;9;10";
	_props["app.my_app_name.string.list.good.0"] = "0;1;2;3;4;5;6;7;8;9;10;11;12;13;14;15";
	_props["app.my_app_name.string.list.good.1"]  = "";
	_props["app.my_app_name.string.list.bad.0"]  = "one;;three";
    }	
    
    ~MyConfig() {
	PropertiesIterator i = _props.begin();
	do {
//	    cout << i->first << "=" << i->second << endl;
	    _props.erase(i++);
	} while(  i != _props.end() );
	_props.clear();
    }
    
    void* getFactoryByName( const char* )  const throw(ConfigError)
    {
	return 0;
    }
    void* getFactoryByName( const std::string& )  const throw(ConfigError)
    {
	return 0;
    }
    void* getObjectByName( const char* )   const throw(ConfigError)
    {
	return 0;
    }
    void* getObjectByName( const std::string& )   const throw(ConfigError)
    {
	return 0;
    }
    void* getTaskByName( const char* )     const throw(ConfigError)
    {
	return 0;
    }
    void* getTaskByName( const std::string& )     const throw(ConfigError)
    {
	return 0;
    }
    virtual std::string getProperty( const char* p ) const throw(ConfigError)
    {
	PropertiesIteratorC i = _props.find(p);
	if( i == _props.end() ) throw ConfigError(p);
	return i->second;
    }
    virtual std::string getProperty( const std::string& p ) const throw(ConfigError)
    {
	return getProperty( p.c_str() );
    }
    std::string getPassword( const std::string& s, const std::string &u ) const throw(ConfigError) {
	return "";
    }

    void setDestroyListener(ConfigDestroyListener*) throw()
    {
    }

};


class ConfigHelperTest : public CppUnit::TestFixture {
    MyConfig _config;
    std::string _name;
public:
    ConfigHelperTest() { }
    void setUp();
    void testDefaultValues();
    void test01();
    void test02();
    void testCheckMaxValue();
    void testCheckMinValue();
    void testCheckIntDefValue();
    void testCheckIntDefRangeValue();
    void testCheckIntRangeValue();
    void testBool();
    void testString();
    void testOct();
    void testOctFailed();
    void testOctFailed1();
    void testBoolFailed();
    void testBoolFailed1();
    void testStringFailed1();
    void testStringDefVal();
    void testStringValues();
    void testStringValuesFailed();
    void testStringDefValuesFailed();
    void testObjectsProperties();
    void testTasksProperties();
    void testAppsProperties();
    void getIntList();
    void getIntList0();
    void getStringListGood0();
    void getStringListGood1();
    void getStringListBad0();
    void tearDown();

    CPPUNIT_TEST_SUITE( ConfigHelperTest );
	CPPUNIT_TEST(testDefaultValues);
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST_EXCEPTION(test02,ConfigHelperError);
	CPPUNIT_TEST_EXCEPTION(testCheckMaxValue,ConfigHelperError);
	CPPUNIT_TEST_EXCEPTION(testCheckMinValue,ConfigHelperError);
	CPPUNIT_TEST_EXCEPTION(testBoolFailed,ConfigHelperError);
	CPPUNIT_TEST_EXCEPTION(testOctFailed, ConfigHelperError);
	CPPUNIT_TEST_EXCEPTION(testBoolFailed1, ConfigError);
	CPPUNIT_TEST_EXCEPTION(testOctFailed1, ConfigError);
	CPPUNIT_TEST_EXCEPTION(testStringFailed1, ConfigError);
	CPPUNIT_TEST_EXCEPTION(testStringValuesFailed, ConfigHelperError);
	CPPUNIT_TEST(testBool);
	CPPUNIT_TEST(testString);
	CPPUNIT_TEST(testOct);
	CPPUNIT_TEST(testCheckIntDefValue);
	CPPUNIT_TEST(testCheckIntDefRangeValue);
	CPPUNIT_TEST(testCheckIntRangeValue);
	CPPUNIT_TEST(testStringDefVal);
	CPPUNIT_TEST(testStringValues);
	CPPUNIT_TEST_EXCEPTION(testStringDefValuesFailed,ConfigHelperError);
	CPPUNIT_TEST( testObjectsProperties );
	CPPUNIT_TEST( testTasksProperties );
	CPPUNIT_TEST( testAppsProperties );
	CPPUNIT_TEST( getIntList );
	CPPUNIT_TEST_EXCEPTION( getIntList0, ConfigHelperError );
	CPPUNIT_TEST( getStringListGood0 );
	CPPUNIT_TEST( getStringListGood1 );
	CPPUNIT_TEST_EXCEPTION( getStringListBad0, ConfigHelperError );
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( ConfigHelperTest );

void ConfigHelperTest::setUp() {
}

void ConfigHelperTest::testDefaultValues()
{
    DefaultValues defVals("sec","min","hour","day","week","month","year",NULL);
    CPPUNIT_ASSERT( defVals.find( "sec" ) != defVals.end() );
    CPPUNIT_ASSERT( defVals.find( "min" ) != defVals.end() );
    CPPUNIT_ASSERT( defVals.find( "hour" ) != defVals.end() );
    CPPUNIT_ASSERT( defVals.find( "day" ) != defVals.end() );
    CPPUNIT_ASSERT( defVals.find( "week" ) != defVals.end() );
    CPPUNIT_ASSERT( defVals.find( "month" ) != defVals.end() );
    CPPUNIT_ASSERT( defVals.find( "year" ) != defVals.end() );
    CPPUNIT_ASSERT( defVals.find( "unknown" ) == defVals.end() );
}

void ConfigHelperTest::test01() {
    ConfigHelper ch( &_config );
    CPPUNIT_ASSERT( 1 == ch.getInt( "test1.int.value" ) );
}
void ConfigHelperTest::test02() {
    ConfigHelper ch( &_config );
    ch.getInt( "test1.int.unknown.value" );
}
void ConfigHelperTest::testCheckMaxValue() {
    ConfigHelper ch( &_config );
    ch.getInt( "test1.int.max.value", 50, 1, 100 );
}
void ConfigHelperTest::testCheckMinValue() {
    ConfigHelper ch( &_config );
    ch.getInt( "test1.int.min.value", 50, 1, 100 );
}
void ConfigHelperTest::testCheckIntDefValue() {
    ConfigHelper ch( &_config );
    CPPUNIT_ASSERT( 50 == ch.getInt( "test1.int.unknown.value", 50 ) );
}
void ConfigHelperTest::testCheckIntDefRangeValue() {
    ConfigHelper ch( &_config );
    CPPUNIT_ASSERT( 50 == ch.getInt( "test1.int.unknown.default.value", 50, 1, 100 ) );
}
void ConfigHelperTest::testCheckIntRangeValue() {
    ConfigHelper ch( &_config );
    CPPUNIT_ASSERT( 1 == ch.getInt( "test1.int.value", 50, 1, 100 ) );
}
void ConfigHelperTest::testBool() {
    ConfigHelper ch( &_config );
    CPPUNIT_ASSERT( false == ch.getBool( "test2.bool.false", true ) );
    CPPUNIT_ASSERT( true == ch.getBool( "test2.bool.true", false ) );
    CPPUNIT_ASSERT( false == ch.getBool( "test2.bool.false.0", true ) );
    CPPUNIT_ASSERT( true == ch.getBool( "test2.bool.true.1", false ) );
}
void ConfigHelperTest::testString() {
    ConfigHelper ch( &_config );
    CPPUNIT_ASSERT( std::string("information") == ch.getString( "test3.string.info", "" ) );
}
void ConfigHelperTest::testOct() {
    ConfigHelper ch( &_config );
    CPPUNIT_ASSERT( 0644 == ch.getOct( "test4.octal.mode", 0 ) );
}
void ConfigHelperTest::testBoolFailed() {
    ConfigHelper ch( &_config );
    ch.getBool( "test5.bool.failed", true );
}
void ConfigHelperTest::testOctFailed() {
    ConfigHelper ch( &_config );
    ch.getOct( "test6.octal.failed", 0 );
}
void ConfigHelperTest::testBoolFailed1() {
    ConfigHelper ch( &_config );
    ch.getBool( "unknown.bool.value" );
}
void ConfigHelperTest::testOctFailed1() {
    ConfigHelper ch( &_config );
    ch.getOct( "unknown.octal.value" );
}
void ConfigHelperTest::testStringFailed1() {
    ConfigHelper ch( &_config );
    ch.getString( "unknown.string.value" );
}
void ConfigHelperTest::testStringDefVal() {
    ConfigHelper ch( &_config );
    CPPUNIT_ASSERT( std::string("Default Value") == ch.getString( "unknown.string.value", "Default Value" ) );
    CPPUNIT_ASSERT( std::string("information")   == ch.getString( "test3.string.info",    "Default Value" ) );
}
void ConfigHelperTest::testStringValues() {
    ConfigHelper ch( &_config );
    CPPUNIT_ASSERT( std::string("hour") == 
	ch.getString( "test7.string.range.value", DefaultValues("sec","min","hour",0) ) );
}
void ConfigHelperTest::testStringValuesFailed() {
    ConfigHelper ch( &_config );
    ch.getString( "test8.string.range.value", DefaultValues("sec","min","hour",0) );
}
void ConfigHelperTest::testStringDefValuesFailed() {
    ConfigHelper ch( &_config );
    ch.getString( "test8.string.range.value", "year", DefaultValues("sec","min","hour",0) );
}
void ConfigHelperTest::testObjectsProperties() {
    ConfigHelper ch( ConfigHelper::OBJECT, "my_object_name", &_config );
    CPPUNIT_ASSERT( ch.getString( "string.value" ) == "this string value for object" );
    CPPUNIT_ASSERT( ch.getString( "unknown.string.value", "Default value" ) == "Default value" );
    CPPUNIT_ASSERT( ch.getString( "string.value", "Default value" ) == "this string value for object" );
}
void ConfigHelperTest::testTasksProperties() {
    ConfigHelper ch( ConfigHelper::TASK, "my_task_name", &_config );
    CPPUNIT_ASSERT( ch.getString( "string.value" ) == "this string value for task" );
    CPPUNIT_ASSERT( ch.getString( "unknown.string.value", "Default value" ) == "Default value" );
    CPPUNIT_ASSERT( ch.getString( "string.value", "Default value" ) == "this string value for task" );
}
void ConfigHelperTest::testAppsProperties() {
    ConfigHelper ch( ConfigHelper::APPS, "my_app_name", &_config );
    CPPUNIT_ASSERT( ch.getString( "string.value" ) == "this string value for application" );
}
void ConfigHelperTest::getIntList() {
    ConfigHelper ch( ConfigHelper::APPS, "my_app_name", &_config );
    std::list<int> intList = ch.getIntList("int.list");
    int testVal = 0;
    for( std::list<int>::iterator it = intList.begin(); it != intList.end(); it++ ) {
	CPPUNIT_ASSERT( *it == testVal++ );
    }
}
void ConfigHelperTest::getIntList0() {
    ConfigHelper ch( ConfigHelper::APPS, "my_app_name", &_config );
    std::list<int> intList = ch.getIntList("int.list.0");
}
void ConfigHelperTest::getStringListGood0() {
    ConfigHelper ch( ConfigHelper::APPS, "my_app_name", &_config );
    std::list<std::string> strList = ch.getStringList("string.list.good.0");
    int testVal = 0;
    int intVal;
    for( std::list<std::string>::iterator it = strList.begin(); it != strList.end(); it++ ) {
	std::stringstream stream;
	stream << *it;
	stream >> intVal;
	CPPUNIT_ASSERT( intVal == testVal++ );
    }
}
void ConfigHelperTest::getStringListGood1() {
    ConfigHelper ch( ConfigHelper::APPS, "my_app_name", &_config );
    std::list<std::string> strList = ch.getStringList("string.list.good.1");
}
void ConfigHelperTest::getStringListBad0() {
    ConfigHelper ch( ConfigHelper::APPS, "my_app_name", &_config );
    std::list<std::string> strList = ch.getStringList("string.list.bad.0");
}
void ConfigHelperTest::tearDown() {
}


