/**
 * Time utitlity
 *
 * $Id: ScheduleUtilImpl.cpp 221 2011-11-18 09:03:51Z dantld $
 */

#include <set>
#include <stdio.h>
#include <stdlib.h>
#include "../ScheduleUtil.h"
#define __CXXHTC__LIBRARY__PRIVATE__INCLUDE__
#include "./ScheduleUtil_Private.h"
#undef __CXXHTC__LIBRARY__PRIVATE__INCLUDE__
#include "../StringUtil.h"

BEGIN_CXXHTC_NAMESPACE
BEGIN_CXXHTC_UTIL_NAMESPACE

/**
 * @defgroup timeutil miscellaneous time functions, classes and etc.
 * 
 */
/*@{*/
/**
 * SchedulerParser - simple parser for string like crontab used.
 *
 */
SchedulerParser::SchedulerParser() throw() {
}
SchedulerParser::~SchedulerParser() throw() {
}


SchedulerParser_ITEST::SchedulerParser_ITEST() throw() {
}

SchedulerParser_ITEST::SchedulerParser_ITEST( SchedulerParser::Ptr ptr ) throw() :
    _ptr(ptr)
{
}

SchedulerParser_ITEST::~SchedulerParser_ITEST() throw() {
}
void SchedulerParser_ITEST::setNow( time_t &timer ) throw() {
    SchedulerParserImpl *impl = dynamic_cast<SchedulerParserImpl*>(_ptr.operator->());
    impl->_overrideNow = true;
    impl->_overrideTimer = timer;
}
void SchedulerParser_ITEST::resetNow() throw() {
    SchedulerParserImpl *impl = dynamic_cast<SchedulerParserImpl*>(_ptr.operator->());
    impl->_overrideNow = false;
    impl->_overrideTimer = -1;
}
    
const int ALL_VALUES_ENABLE = -1;


SchedulerParserImpl::SchedulerParserImpl() throw() :
    _overrideNow(false) /* Use current time as "now" */
{
}

template <class S> bool isValueInSet( const S& s, int value ) 
{
    if( s.find( ALL_VALUES_ENABLE ) != s.end() ) {
	return true;
    }
    return s.find( value ) != s.end();
}

bool SchedulerParserImpl::isMin( int v ) const throw()
{
    return isValueInSet( _minutes, v );
}
bool SchedulerParserImpl::isHour( int v ) const throw()
{
    return isValueInSet( _hours, v );
}
bool SchedulerParserImpl::isDay( int v ) const throw()
{
    return isValueInSet( _days, v );
}
bool SchedulerParserImpl::isMonth( int v ) const throw()
{
    return isValueInSet( _month, v );
}
bool SchedulerParserImpl::isWeekDay( int v ) const throw()
{
    return isValueInSet( _wdays, v );
}

/**
 * RE schedule pattern.
 * Based on a cron table.
 */
static const char* SCHD_PATTERN_STR = 
"[[:space:]]*"
"(\\*|[[:digit:]]+(,[[:digit:]]+)*|[[:digit:]]+-[[:digit:]]+)[[:space:]]+"
"(\\*|[[:digit:]]+(,[[:digit:]]+)*|[[:digit:]]+-[[:digit:]]+)[[:space:]]+"
"(\\*|[[:digit:]]+(,[[:digit:]]+)*|[[:digit:]]+-[[:digit:]]+)[[:space:]]+"
"(\\*|[[:digit:]]+(,[[:digit:]]+)*|[[:digit:]]+-[[:digit:]]+)[[:space:]]+"
"(\\*|[[:digit:]]+(,[[:digit:]]+)*|[[:digit:]]+-[[:digit:]]+)[[:space:]]+"
"([-_.0-9a-zA-Z]+)([[:space:]]+.*)*"
;
static const char SCHD_PATTERN_ASTERISK = '*';
static const char SCHD_PATTERN_COMMA = ',';
static const char SCHD_PATTERN_HYPHEN = '-';

static int atoi( const std::string& s ) 
{
    int val = -1;
#ifdef _TIMEUTIL_DEBUG
fprintf( stderr, "Custom \"atoi\" try to SETUP VALUE: \"%s\"\n", s.c_str() );
#endif
    if( sscanf( s.c_str(), "%d", &val ) != 1 ) {
	throw std::runtime_error("Schedule parser: Failed to convert from string to int");
    }
    return val;
}

void fillSetOfInt( SchedulerParserImpl::SetOfInt& s, const std::string &str, char divider ) 
{
    std::string::size_type pos1 = 0;
    std::string::size_type pos2 = str.find( divider );
    while( pos2 != std::string::npos ) {
	s.insert( atoi( str.substr( pos1, pos2 - pos1 ) ) );
	pos1 = pos2 + 1;
	pos2 = str.find( divider, pos1 );
    }
    s.insert( atoi( str.substr( pos1 ) ) );
}

void fillSetOfInt( SchedulerParserImpl::SetOfInt& s, const std::string &str ) 
{
    std::string::size_type pos2 = str.find( SCHD_PATTERN_HYPHEN );
    int start = -1;
    int finish = -1;
    char divider;
    if( sscanf( str.c_str(), "%d%c%d", &start, &divider, &finish ) != 3 ) {
	throw std::runtime_error("Schedule parser: Hyphen format wrong");
    }
    for( int i = start; i <= finish; i++ ) {
	s.insert( i );
    }
}

void fillSetOfInt( SchedulerParserImpl::SetOfInt& s, int value ) 
{
    std::pair< SchedulerParserImpl::SetOfInt::iterator, bool > r;
    r = s.insert( value );
    if( !r.second ) {
	throw std::runtime_error("Schedule parser: Failed to insert value in a set of integers");
    }
}

template <class S> void setupSetValues( S &s, const std::string& v ) 
{
    if( v.find( SCHD_PATTERN_ASTERISK ) != std::string::npos ) {
#ifdef _TIMEUTIL_DEBUG
fprintf(stderr,"SETUP ASTERISK VALUES: %s\n", v.c_str() );
#endif
	fillSetOfInt( s, ALL_VALUES_ENABLE );
    } else if( v.find( SCHD_PATTERN_COMMA ) != std::string::npos ) {
#ifdef _TIMEUTIL_DEBUG
fprintf(stderr,"SETUP COMMA SEPARATED VALUES: %s\n", v.c_str() );
#endif
	fillSetOfInt( s, v, SCHD_PATTERN_COMMA );
    } else if( v.find( SCHD_PATTERN_HYPHEN ) != std::string::npos ) {
#ifdef _TIMEUTIL_DEBUG
fprintf(stderr,"SETUP HYPHEN SEPARATED VALUES: %s\n", v.c_str() );
#endif
	fillSetOfInt( s, v );
    } else {
#ifdef _TIMEUTIL_DEBUG
fprintf(stderr,"SETUP JUST VALUE (NON SEPARATED VALUES): %s\n", v.c_str() );
#endif
	fillSetOfInt( s, atoi( v ) );
    }
}

void SchedulerParserImpl::setString( const std::string& schedule ) 
    throw(std::runtime_error)
try {
    RegExp re( SCHD_PATTERN_STR );
    StringsVector v;

    if( !re.match( schedule, v ) ) {
	throw std::runtime_error("Schedule parser: string must have only \"[-,*0-9][[:space:]]\". See man for crontab.");
    }

    _minutes.clear();
    _hours.clear();
    _days.clear();
    _month.clear();
    _wdays.clear();
    
    setupSetValues( _minutes, v[0] );
    setupSetValues( _hours,   v[2] );
    setupSetValues( _days,    v[4] );
    setupSetValues( _month,   v[6] );
    setupSetValues( _wdays,   v[8] );
    
    _taskName = v[10];
    _taskArgs = trim(v[11]);

#ifdef _TIMEUTIL_DEBUG
fprintf( stderr, "TASK NAME \"%s\"\n", _taskName.c_str() );
fprintf( stderr, "TASK ARGS \"%s\"\n", _taskArgs.c_str() );
for( SetOfInt::const_iterator i = _minutes.begin(); i != _minutes.end(); i++ ) {
    fprintf( stderr, "MINUTES: %d\n", *i );
}
#endif
} catch( const std::exception& e ) {
    throw std::runtime_error( e.what() );
}

bool SchedulerParserImpl::isToday() const throw()
{
    time_t now ;
    if( _overrideNow ) {
	now = _overrideTimer;
    } else {
	time( &now );
    }
    struct tm ltm;
    localtime_r( &now, &ltm );
    if( isWeekDay( ltm.tm_wday ) &&
	isDay(     ltm.tm_mday ) &&
	isMonth(   ltm.tm_mon ) ) {
	return true;
    }
    return false;
}
bool SchedulerParserImpl::isNow() const throw()
{
    if( !isToday() ) return false;
    time_t now ;
    if( _overrideNow ) {
	now = _overrideTimer;
    } else {
	time( &now );
    }
    struct tm ltm;
    localtime_r( &now, &ltm );
    if( isMin( ltm.tm_min ) && isHour( ltm.tm_hour ) ) {
	return true;
    }
    return false;
}
const std::string& SchedulerParserImpl::getTaskName() const throw() {
    return _taskName;
}
const std::string& SchedulerParserImpl::getTaskArgs() const throw() {
    return _taskArgs;
}
time_t SchedulerParserImpl::getPassed() const throw() {
    // TODO implement this
    return -1;
}
time_t SchedulerParserImpl::getRemain() const throw() {
    // TODO implement this
    return -1;
}
SchedulerParser::Ptr SchedulerParser::createInstance() throw() {
    return Ptr(new SchedulerParserImpl);
}

ScheduleItem::ScheduleItem() throw() {}
ScheduleItem::~ScheduleItem() throw() {}

ScheduleItemImpl::ScheduleItemImpl() throw()
{
}

ScheduleItemImpl::ScheduleItemImpl( SchedulerParser::Ptr schParser ) throw() :
	_schParser(schParser),
	_lastMinute(61) 
{
}

ScheduleItemImpl::~ScheduleItemImpl() throw()
{
}

bool ScheduleItemImpl::isNow() const throw() {
	bool retVal = _schParser->isNow();
	if( retVal ) {
	    time_t timer;
	    time( &timer );
	    struct tm ltm;
	    localtime_r( &timer, &ltm );
	    if( ltm.tm_min != _lastMinute ) {
		_lastMinute = ltm.tm_min;
	    } else {
		retVal = false;
	    }
	} else {
	    _lastMinute = 61;
	}
    return retVal;
}
const std::string& ScheduleItemImpl::getTaskName() const throw() {
    return _schParser->getTaskName();
}
const std::string& ScheduleItemImpl::getTaskArgs() const throw() {
    return _schParser->getTaskArgs();
}

ScheduleItem_ITEST::ScheduleItem_ITEST() throw() {
}
ScheduleItem_ITEST::ScheduleItem_ITEST(ScheduleItem::Ptr ptr) throw() :
    _ptr(ptr)
{
}
ScheduleItem_ITEST::~ScheduleItem_ITEST() throw() {
}
void ScheduleItem_ITEST::setNow( time_t &timer ) throw() {
    ScheduleItemImpl *item = dynamic_cast<ScheduleItemImpl*>( _ptr.operator->() );
    SchedulerParser_ITEST itest(item->_schParser);
    itest.setNow( timer );
}
void ScheduleItem_ITEST::resetNow() throw() {
    ScheduleItemImpl *item = dynamic_cast<ScheduleItemImpl*>( _ptr.operator->() );
    SchedulerParser_ITEST itest(item->_schParser);
    itest.resetNow();
}

class ScheduleListImpl : public ScheduleList {
    typedef std::list<ScheduleItem::Ptr> schedule_list;
    typedef std::list<ScheduleItem::Ptr>::iterator schedule_list_iter;
    schedule_list _list;
    schedule_list_iter _iter;
public:
    ScheduleListImpl() throw();
    ~ScheduleListImpl() throw();
    void addEvent( const std::string& schStr ) throw(std::runtime_error);

    void begin() throw();
    bool end() throw();
    void next() throw();
    
    bool empty() const throw();

    ScheduleItem::Ptr current() throw();
};

ScheduleList::ScheduleList() throw() {
}
ScheduleList::~ScheduleList() throw() {
}

ScheduleListImpl::ScheduleListImpl() throw() {
    _iter = _list.begin();
}
ScheduleListImpl::~ScheduleListImpl() throw() {
    _list.clear();
}
void ScheduleListImpl::addEvent( const std::string& schStr ) throw(std::runtime_error) {
    SchedulerParser::Ptr schPrs = SchedulerParser::createInstance();
    schPrs->setString( schStr );
    _list.push_back( ScheduleItem::Ptr( new ScheduleItemImpl( schPrs ) ) );
}

void ScheduleListImpl::begin() throw() {
    _iter = _list.begin();
}
bool ScheduleListImpl::end() throw() {
    return _iter == _list.end();
}
void ScheduleListImpl::next() throw() {
    _iter++;
}
ScheduleItem::Ptr ScheduleListImpl::current() throw() {
    return *_iter;
}

bool ScheduleListImpl::empty() const throw() {
    return _list.empty();
}
    
ScheduleList::Ptr ScheduleList::createInstance() throw() {
    return Ptr(new ScheduleListImpl);
}

/*@}*/
END_CXXHTC_UTIL_NAMESPACE
END_CXXHTC_NAMESPACE
