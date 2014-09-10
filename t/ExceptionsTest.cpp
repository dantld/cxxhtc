/**
 * $Id: ExceptionsTest.cpp 213 2011-08-11 10:05:35Z dantld $
 *
 */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string.h>
#include "../CommonErrors.h"

class ExceptionsTest : public CppUnit::TestFixture {
    std::string _name;
public:
    ExceptionsTest() { }
    void setUp();
    void test01();
    void tearDown();

    CPPUNIT_TEST_SUITE( ExceptionsTest );
	CPPUNIT_TEST(test01);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( ExceptionsTest );

void ExceptionsTest::setUp() {
}
void ExceptionsTest::test01() {
    std::string sqlStmt = "Select * from any";
    try {
        throw kisc::BadSqlReturnCode( "Main", -9999, sqlStmt.c_str() );
    } catch( const kisc::BadSqlReturnCode &e ) {
        CPPUNIT_ASSERT( strstr( e.what(), "Main" ) != NULL );
        CPPUNIT_ASSERT( strstr( e.what(), "-9999" ) != NULL );
        CPPUNIT_ASSERT( strstr( e.what(), sqlStmt.c_str() ) != NULL );
	CPPUNIT_ASSERT( e.getReturnCode() == -9999 );
	CPPUNIT_ASSERT( strcmp( e.getSqlStatement(), "Select * from any" ) == 0 );
    }
}
void ExceptionsTest::tearDown() {
}


