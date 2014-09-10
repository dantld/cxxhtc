/**
 * $Id: StringUtilTest.cpp 244 2014-02-20 05:17:27Z dantld $
 *
 */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../StringUtil.h"
#include <iomanip>
#include <string.h>

class StringUtilTest : public CppUnit::TestFixture {
    std::string _name;
public:
    StringUtilTest() { }
    void setUp();
    void test_ltrim();
    void test_rtrim();
    void test_trim();
    void test_changeSymbols();
    void test_breakString();
    void test_breakString2();
    void test_toupper();
    void test_tolower();
    void tearDown();

    CPPUNIT_TEST_SUITE( StringUtilTest );
	CPPUNIT_TEST(test_ltrim);
	CPPUNIT_TEST(test_rtrim);
	CPPUNIT_TEST(test_trim);
	CPPUNIT_TEST(test_changeSymbols);
	CPPUNIT_TEST(test_breakString);
	CPPUNIT_TEST(test_breakString2);
	CPPUNIT_TEST(test_toupper);
	CPPUNIT_TEST(test_tolower);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( StringUtilTest );

void StringUtilTest::setUp() {
}
void StringUtilTest::test_ltrim() {
    const char t1[] = {0x20,0x20,0x20,'t','e','s','t',' ','v','a','l','u','e',0};
    char t2[64];
    strcpy( t2, t1 );
    kisc::ltrim( t2 );
    CPPUNIT_ASSERT( strcmp( t2, "test value" ) == 0 );
    std::string t3 = t1;
    kisc::ltrim( t3 );
    CPPUNIT_ASSERT( t3 == std::string("test value") );
}
void StringUtilTest::test_rtrim() {
    const char t1[] = {'t','e','s','t',' ','v','a','l','u','e',0x20,0x20,0x20,0};
    char t2[64];
    strcpy( t2, t1 );
    kisc::rtrim( t2 );
    CPPUNIT_ASSERT( strcmp( t2, "test value" ) == 0 );
    std::string t3 = t1;
    kisc::rtrim( t3 );
    CPPUNIT_ASSERT( t3 == std::string("test value") );
}
void StringUtilTest::test_trim() {
    const char t1[] = {0x20,0x20,0x20,'t','e','s','t',' ','v','a','l','u','e',0x20,0x20,0x20,0};
    char t2[64];
    strcpy( t2, t1 );
    kisc::trim( t2 );
    CPPUNIT_ASSERT( strcmp( t2, "test value" ) == 0 );
    std::string t3 = t1;
    kisc::trim( t3 );
    CPPUNIT_ASSERT( t3 == std::string("test value") );
}
void StringUtilTest::test_changeSymbols() {
    const char t1[] = {'t','e','s','t','-','v','a','l','u','e','-','o','k',0};
    char t2[64];
    strcpy( t2, t1 );
    CPPUNIT_ASSERT( kisc::changeSymbols( t2, '-', ' ') == 2 );
    CPPUNIT_ASSERT( strcmp( t2, "test value ok" ) == 0 );
    strcpy( t2, t1 );
    CPPUNIT_ASSERT( kisc::changeSymbols( t2, '.', ' ') == 0 );
    CPPUNIT_ASSERT( strcmp( t2, "test-value-ok" ) == 0 );
}
const char *shortText = "On Itanium systems";
const char *longText = 
"On Itanium systems running HP-UX 11v2/v3 for "
"displaying system information HP introduced "
"a new command called machinfo. Running machinfo "
"command on Itanium HP-UX systems (servers) "
"not only displays CPU information, memory "
"information, firmware version, HP-UX system "
"model but it also displays system serial number."
" If you want to find out the Itanium HP-UX system "
"model and system serial information you would "
"need to run machinfo. In the example above HP-UX "
"system mode type is rx6600 and also displays "
"system serial number."
    ;
const int len  = 40;
const int len2 = 60;
void StringUtilTest::test_breakString() {
    {
	std::string result;
	kisc::StringsList list;
	kisc::break_String( len, longText, list );
	for( kisc::StringsList::iterator i = list.begin(); i != list.end(); i++ ) {
	    CPPUNIT_ASSERT( i->length() <= len );
	    result += *i;
	}
	int segLen = strspn( longText, result.c_str() );
	CPPUNIT_ASSERT( segLen == strlen(longText) );
    }
    {
	kisc::StringsList list;
	kisc::break_String( len, longText, list, 3 );
	CPPUNIT_ASSERT( 3 == list.size() );
    }
    {
	kisc::StringsList list;
	kisc::break_String( len, "", list );
	CPPUNIT_ASSERT( 0 == list.size() );
    }
    {
	kisc::StringsList list3;
	kisc::break_String( len, shortText, list3, 0, ":77A:" );
	bool first = true;
	for( kisc::StringsList::iterator i = list3.begin(); i != list3.end(); i++ ) {
	    CPPUNIT_ASSERT( i->length() <= len );
	    if( first ) {
		CPPUNIT_ASSERT( 0 == (*i).find(":77A:") );
	    }
	    first = false;
	}
    }
    {
	kisc::StringsList list3;
	kisc::break_String( len, longText, list3, 0, ":77A:" );
	bool first = true;
	for( kisc::StringsList::iterator i = list3.begin(); i != list3.end(); i++ ) {
	    CPPUNIT_ASSERT( i->length() <= len );
	    if( first ) {
		CPPUNIT_ASSERT( 0 == (*i).find(":77A:") );
	    }
	    first = false;
	}
    }
    {
	kisc::StringsList list3;
	kisc::break_String( len, longText, list3, 0, "77A", "//" );
	bool skip_first = true;
	for( kisc::StringsList::iterator i = list3.begin(); i != list3.end(); i++ ) {
//std::cerr << "\"" << *i << "\"" << std::endl;
	    CPPUNIT_ASSERT( i->length() <= len );
	    if( !skip_first ) {
		CPPUNIT_ASSERT( 0 == (*i).find("//") );
	    } else {
		CPPUNIT_ASSERT( 0 == (*i).find("77A") );
	    }
	    skip_first = false;
	}
    }
    {
char buf[] = 
    "1234567890"
    "1234567890"
    "1234567890"
    "1234567890"
    "1234567890"
    "1234567890"
    "1234567890"
;
	int len = 70;
	kisc::StringsList list;
	kisc::break_String( len, buf, list, 6, ":72:" );
	bool skip_first = true;
	CPPUNIT_ASSERT( list.size() <= 6 );
	for( kisc::StringsList::iterator i = list.begin(); i != list.end(); i++ ) {
//std::cerr  << std::endl<< "\"" << *i << "\"";
	    CPPUNIT_ASSERT( i->length() <= len );
	    if( !skip_first ) {
	    } else {
		CPPUNIT_ASSERT( 0 == (*i).find(":72:") );
	    }
	    skip_first = false;
	}
    }
}
void StringUtilTest::test_breakString2() {
    size_t srcLength = strlen(longText);
    size_t chunkSize = 70;
    size_t dstLength = srcLength + (srcLength / 70 * 2) + 2;
    size_t lastChunkSize = srcLength % 70;
    char* destBuf = new char[dstLength];
    int copied = kisc::break_String( destBuf, longText, srcLength, chunkSize );
    CPPUNIT_ASSERT( dstLength == copied );
    char *p0 = destBuf, *p1 = destBuf;
//std::cerr << std::endl;
    for( int i = 0; i < copied; i++, p0++ ) {
	if( *p0 == 0x0D && i < copied && *(p0+1) == 0x0A ) {
//std::cerr << "Chunk size: " << p0 - p1 << std::endl;
	    if( i < dstLength - lastChunkSize ) {
		CPPUNIT_ASSERT( p0 - p1 == chunkSize );
	    } else {
		CPPUNIT_ASSERT( p0 - p1 == lastChunkSize );
	    }
//	    std::string s;
//	    s.assign( p1, p0 - p1 );
//std::cerr << s;
	    p1 = p0 + 2;
	    p0 += 2;
	}
    }
    delete destBuf;
}

void StringUtilTest::test_toupper()
{
    std::string result = kisc::string_toupper("hello world!!!");
    CPPUNIT_ASSERT( result == "HELLO WORLD!!!" );
}

void StringUtilTest::test_tolower()
{
    std::string result = kisc::string_tolower("HELLO WORLD!!!");
    CPPUNIT_ASSERT( result ==  "hello world!!!");
}

void StringUtilTest::tearDown() {
}
