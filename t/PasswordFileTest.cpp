/**
 * $Id: PasswordFileTest.cpp 223 2011-11-21 05:21:34Z dantld $
 *
 */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../Password.h"

class PasswordFileTest : public CppUnit::TestFixture {
    std::string _name;
public:
    PasswordFileTest() { }
    void setUp();
    void test01();
    void test02();
    void test03();
    void tearDown();

    CPPUNIT_TEST_SUITE( PasswordFileTest );
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST_EXCEPTION(test02,kisc::PasswordError);
	CPPUNIT_TEST_EXCEPTION(test03,kisc::PasswordError);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( PasswordFileTest );

void PasswordFileTest::setUp() {
    char name[256];
    strcpy( name, "passwordXXXXXX" );
    int fd = mkstemp( name );
    if( fd > -1 ) close(fd);
    _name = name;
    std::ofstream os(_name.c_str());
    if( os.bad() || os.fail() ) throw std::runtime_error("Failed to create output stream");
    os << "PLAT:mspd:1q2w3e4r" << std::endl;
    os.flush();
    os.close();
}
void PasswordFileTest::test01() {
    kisc::Password::Ptr pwf = kisc::Password::create();
    pwf->setFileName( _name );
    std::string pwd = pwf->getPassword("PLAT","mspd");
    CPPUNIT_ASSERT( pwd.compare("1q2w3e4r") == 0 );
}
void PasswordFileTest::test02() {
    kisc::Password::Ptr pwf = kisc::Password::create();
    pwf->setFileName( _name );
    pwf->getPassword("UNKNOWN","unknown");
}
void PasswordFileTest::test03() {
    kisc::Password::Ptr pwf = kisc::Password::create();
    pwf->setFileName( "UNKNOWN" );
    pwf->getPassword("PLAT","mspd");
}
void PasswordFileTest::tearDown() {
    unlink( _name.c_str() );
}


