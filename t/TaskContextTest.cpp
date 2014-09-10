/**
 * $Id: TaskContextTest.cpp 152 2010-02-25 10:15:26Z dantld $
 *
 */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../Task.h"

class TaskContextTest : public CppUnit::TestFixture {
    std::string _name;
public:
    TaskContextTest() { }
    void setUp();
    void set_read_value();
    void read_notexists_value();
    void tearDown();

    CPPUNIT_TEST_SUITE( TaskContextTest );
	CPPUNIT_TEST(set_read_value);
	CPPUNIT_TEST_EXCEPTION(read_notexists_value,std::domain_error);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( TaskContextTest );

void TaskContextTest::setUp() {
}
void TaskContextTest::set_read_value() {
    CXXHTC::TaskContextImpl tctxImpl;
    tctxImpl.setProp( "my-pointer-01", &tctxImpl );
    CXXHTC::TaskContext* tctx = &tctxImpl;
    CPPUNIT_ASSERT( &tctxImpl == tctx->getProp( "my-pointer-01" ) );
    
    int dummyInt = 0;
    tctxImpl.setProp( "my-int-01", dummyInt );
    CPPUNIT_ASSERT( dummyInt == tctx->getInt( "my-int-01" ) );
    
    std::string dummyStr("my-string");
    tctxImpl.setProp( "my-string-01", dummyStr );
    CPPUNIT_ASSERT( dummyStr == tctx->getString( "my-string-01" ) );
}
void TaskContextTest::read_notexists_value() {
    CXXHTC::TaskContextImpl tctxImpl;
    CXXHTC::TaskContext* tctx = &tctxImpl;
    tctx->getProp("note-exists-value");
}
void TaskContextTest::tearDown() {
}
