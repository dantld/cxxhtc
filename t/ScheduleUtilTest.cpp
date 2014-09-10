/**
 * $Id: ScheduleUtilTest.cpp 164 2010-03-11 11:32:16Z dantld $
 *
 */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../ScheduleUtil.h"
#define __CXXHTC__LIBRARY__PRIVATE__INCLUDE__
#include "../impl/ScheduleUtil_Private.h"
#undef __CXXHTC__LIBRARY__PRIVATE__INCLUDE__
#include <iomanip>

class TimeUtilTest : public CppUnit::TestFixture {
    CXXHTC::util::SchedulerParser::Ptr _s;
public:
    TimeUtilTest();
    void setUp();
    void test_wrong();
    void test_right();
    void test_today();
    void test_now();
    void test_task_name();
    void test_future();
    void test_list();
    void tearDown();

    CPPUNIT_TEST_SUITE( TimeUtilTest );
	CPPUNIT_TEST(test_wrong);
	CPPUNIT_TEST(test_right);
	CPPUNIT_TEST(test_today);
	CPPUNIT_TEST(test_now);
	CPPUNIT_TEST(test_task_name);
	CPPUNIT_TEST(test_future);
	CPPUNIT_TEST(test_list);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( TimeUtilTest );

TimeUtilTest::TimeUtilTest() :
    _s(CXXHTC::util::SchedulerParser::createInstance())
{
}

void TimeUtilTest::setUp() {
}
void TimeUtilTest::test_wrong() {
    bool pass = false;
    try {
	_s->setString("A * * * * sdfwe9");
    } catch( const std::exception &e ) {
	pass = true;
    }
    CPPUNIT_ASSERT( pass );
}
void TimeUtilTest::test_right() {
    _s->setString("* * * * * task_name_here");
    CPPUNIT_ASSERT( _s->isMin(59)     );
    CPPUNIT_ASSERT( _s->isHour(23)     );
    CPPUNIT_ASSERT( _s->isDay(1)  );
    CPPUNIT_ASSERT( _s->isMonth(1)     );
    CPPUNIT_ASSERT( _s->isWeekDay(1)   );
    _s->setString("1 * * * * task_name_here");
    CPPUNIT_ASSERT( _s->isMin(1) );
    _s->setString("1 2 * * * task_name_here");
    CPPUNIT_ASSERT( _s->isMin(1) );
    CPPUNIT_ASSERT( _s->isHour(2));
    CPPUNIT_ASSERT( _s->isDay(1)  );
    CPPUNIT_ASSERT( _s->isMonth(1)     );
    CPPUNIT_ASSERT( _s->isWeekDay(1)   );
    _s->setString("1 2 3 * * task_name_here");
    CPPUNIT_ASSERT( _s->isMin(1)     );
    CPPUNIT_ASSERT( _s->isHour(2)    );
    CPPUNIT_ASSERT( _s->isDay(3));
    _s->setString("1 2 3 4 * task_name_here");
    CPPUNIT_ASSERT( _s->isMin(1)       );
    CPPUNIT_ASSERT( _s->isHour(2)      );
    CPPUNIT_ASSERT( _s->isDay(3)  );
    CPPUNIT_ASSERT( _s->isMonth(4)     );
    _s->setString("1 2 3 4 5 task_name_here");
    CPPUNIT_ASSERT( _s->isMin(1)      );
    CPPUNIT_ASSERT( _s->isHour(2)     );
    CPPUNIT_ASSERT( _s->isDay(3) );
    CPPUNIT_ASSERT( _s->isMonth(4)    );
    CPPUNIT_ASSERT( _s->isWeekDay(5)  );
    
    _s->setString("1,20,30,50 9-18 3 4 5 task_name_here");
    
    CPPUNIT_ASSERT( !_s->isMin(21)     );
    CPPUNIT_ASSERT( _s->isMin( 1)      );
    CPPUNIT_ASSERT( _s->isMin(20)      );
    CPPUNIT_ASSERT( _s->isMin(50)      );
    
    CPPUNIT_ASSERT( !_s->isHour(8)     );
    CPPUNIT_ASSERT( !_s->isHour(19)     );
    CPPUNIT_ASSERT( _s->isHour(9)     );
    CPPUNIT_ASSERT( _s->isHour(10)     );
    CPPUNIT_ASSERT( _s->isHour(18)     );
    
    CPPUNIT_ASSERT( _s->isDay(3)  );
    CPPUNIT_ASSERT( _s->isMonth(4)     );
    CPPUNIT_ASSERT( _s->isWeekDay(5)   );
}
void TimeUtilTest::test_today() {
    time_t timer;
    time( &timer );
    struct tm ltm;
    localtime_r( &timer, &ltm );
    char schStr[128] = "";

    sprintf( schStr, "%d %d * * * task_name_here", ltm.tm_min, ltm.tm_hour );
    _s->setString( schStr );
    CPPUNIT_ASSERT( _s->isToday() );

    sprintf( schStr, "* * %d %d * task_name_here", ltm.tm_mday, ltm.tm_mon );
    _s->setString( schStr );
    CPPUNIT_ASSERT( _s->isToday() );

    sprintf( schStr, "* * %d %d %d task_name_here", ltm.tm_mday, ltm.tm_mon, ltm.tm_wday );
    _s->setString( schStr );
    CPPUNIT_ASSERT( _s->isToday() );

    sprintf( schStr, "* * * * %d task_name_here", ltm.tm_wday );
    _s->setString( schStr );
    CPPUNIT_ASSERT( _s->isToday() );

    sprintf( schStr, "* * * * %d task_name_here", ltm.tm_wday == 6 ?  --ltm.tm_wday : ++ltm.tm_wday);
    _s->setString( schStr );
    CPPUNIT_ASSERT( !_s->isToday() );

    sprintf( schStr, "* * %d * * task_name_here", ltm.tm_mday >= 28 ?  --ltm.tm_mday : ++ltm.tm_mday );
    _s->setString( schStr );
    CPPUNIT_ASSERT( !_s->isToday() );

    sprintf( schStr, "* * * %d * task_name_here", ltm.tm_mon > 12 ?  --ltm.tm_mon : ++ltm.tm_mon );
    _s->setString( schStr );
    CPPUNIT_ASSERT( !_s->isToday() );
}
void TimeUtilTest::test_now() {
    time_t timer;
    time( &timer );
    struct tm ltm;
    localtime_r( &timer, &ltm );
    char schStr[128] = "";

    sprintf( schStr, "* * * * * task_name_here" );
    _s->setString( schStr );
    CPPUNIT_ASSERT( _s->isNow() );

    sprintf( schStr, "%d * * * * task_name_here", ltm.tm_min );
    _s->setString( schStr );
    CPPUNIT_ASSERT( _s->isNow() );

    sprintf( schStr, "%d %d * * * task_name_here MIN HOUR", ltm.tm_min, ltm.tm_hour );
    _s->setString( schStr );
    CPPUNIT_ASSERT( _s->isNow() );

    sprintf( schStr, "%d %d %d * * task_name_here", ltm.tm_min, ltm.tm_hour, ltm.tm_mday );
    _s->setString( schStr );
    CPPUNIT_ASSERT( _s->isNow() );

    sprintf( schStr, "%d %d %d %d * task_name_here", ltm.tm_min, ltm.tm_hour, ltm.tm_mday, ltm.tm_mon );
    _s->setString( schStr );
    CPPUNIT_ASSERT( _s->isNow() );

    sprintf( schStr, "%d * * * * task_name_here", ltm.tm_min == 59 ? --ltm.tm_min : ++ltm.tm_min );
    _s->setString( schStr );
    CPPUNIT_ASSERT( !_s->isNow() );
}
void TimeUtilTest::test_task_name() {
    _s->setString( "* * * * * task_name_here task_args" );
    CPPUNIT_ASSERT( _s->getTaskName() == std::string("task_name_here") );
    CPPUNIT_ASSERT( _s->getTaskArgs() == std::string("task_args") );

    _s->setString( "* * * * * task_name_here.0-2 task_arg_01 task_arg_02 " );
    CPPUNIT_ASSERT( _s->getTaskName() == std::string("task_name_here.0-2") );
    CPPUNIT_ASSERT( _s->getTaskArgs() == std::string("task_arg_01 task_arg_02") );
}
void TimeUtilTest::test_future() {
    char schStr[128] = "";
    CXXHTC::util::SchedulerParser_ITEST itest(_s);
    time_t timer;
    time( &timer );
    struct tm ltm;
    localtime_r( &timer, &ltm );

    sprintf( schStr, "%d %d * * * TEST TOMORROW", ltm.tm_min, ltm.tm_hour );
    _s->setString( schStr );

    ltm.tm_mday = (ltm.tm_mday > 28) ? 1 : ltm.tm_mday + 1;
    timer = mktime( &ltm );
    itest.setNow( timer );
    
    CPPUNIT_ASSERT( _s->isToday() );
    CPPUNIT_ASSERT( _s->isNow() );
}
void TimeUtilTest::test_list() {
    CXXHTC::util::ScheduleList::Ptr l = CXXHTC::util::ScheduleList::createInstance();

    char schStr[128] = "";
    time_t timer;
    time( &timer );
    struct tm ltm;
    localtime_r( &timer, &ltm );

    sprintf( schStr, "%d %d * * * TESTLIST TOMORROW", ltm.tm_min, ltm.tm_hour );

    CPPUNIT_ASSERT( l->empty() );
    
    l->addEvent( schStr );

    CPPUNIT_ASSERT( !l->empty() );
    
    for( int i = 0; i < 7; i++ ) { // Simulate the week
	for( l->begin(); !l->end(); l->next() ) {
	    CXXHTC::util::ScheduleItem::Ptr item = l->current();
	    CXXHTC::util::ScheduleItem_ITEST itest( item );
	    CPPUNIT_ASSERT( item->isNow() );
	    CPPUNIT_ASSERT( !item->isNow() );
	    // Simulate that one minute is past.
	    timer += 60;
	    itest.setNow( timer );
	    CPPUNIT_ASSERT( !item->isNow() );
	    timer -= 60;
	    // Simulate the next day    
	    timer += 3600*24;
	    itest.setNow( timer );
//std::cerr << "NEXT DAY: " << ctime( &timer ) << std::endl;
	}
    }
}
void TimeUtilTest::tearDown() {
    CXXHTC::util::SchedulerParser_ITEST itest(_s);
    itest.resetNow();
}
