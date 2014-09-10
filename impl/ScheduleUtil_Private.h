/**
 * Time utitlity.
 * It is a private inclue file dedicated only for testing
 * purpose.
 * The test driver for implementation of SchedulerParser iface.
 * $Id$
 */

#ifndef __CXXHTC__LIBRARY__PRIVATE__INCLUDE__
#error "This header file is private"
#endif

#ifndef __CXXHTC__LIBRARY__SCHEDULE_UTIL__PRIVATE__H__
#define __CXXHTC__LIBRARY__SCHEDULE_UTIL__PRIVATE__H__

BEGIN_CXXHTC_NAMESPACE
BEGIN_CXXHTC_UTIL_NAMESPACE

class SchedulerParser_ITEST  {
    SchedulerParser::Ptr _ptr;
    SchedulerParser_ITEST() throw();
public:
    SchedulerParser_ITEST( SchedulerParser::Ptr ptr ) throw();
    virtual ~SchedulerParser_ITEST() throw();
    /**
     * Override the "now" value
     */
    void setNow( time_t &timer ) throw();
    /**
     * Return back the using of "now" as a current time.
     */
    void resetNow() throw();
};

class SchedulerParserImpl : public SchedulerParser {
public:
    typedef std::set<int> SetOfInt;
private:
    SetOfInt _minutes;
    SetOfInt _hours;
    SetOfInt _days;
    SetOfInt _month;
    SetOfInt _wdays;

    std::string _taskName;    
    std::string _taskArgs;
    
    bool _overrideNow;
    time_t _overrideTimer;
public:
    SchedulerParserImpl() throw();
    virtual ~SchedulerParserImpl() throw() {}
    virtual void setString( const std::string& schedule ) throw(std::runtime_error);

    virtual bool isMin(     int v ) const throw();
    virtual bool isHour(    int v ) const throw();
    virtual bool isDay(     int v ) const throw();
    virtual bool isMonth(   int v ) const throw();
    virtual bool isWeekDay( int v ) const throw();
    virtual bool isToday() const throw();
    virtual bool isNow() const throw();
    
    virtual const std::string& getTaskName() const throw();
    virtual const std::string& getTaskArgs() const throw();
    
    virtual time_t getPassed() const throw();
    virtual time_t getRemain() const throw();
    static SchedulerParser* createInstance() throw();
    
    friend class SchedulerParser_ITEST;
};

class ScheduleItem_ITEST  {
    ScheduleItem::Ptr _ptr;
    ScheduleItem_ITEST() throw();
public:
    ScheduleItem_ITEST(ScheduleItem::Ptr) throw();
    virtual ~ScheduleItem_ITEST() throw();
    void setNow( time_t &timer ) throw();
    void resetNow() throw();
};

class ScheduleItemImpl : public ScheduleItem {
    ScheduleItemImpl() throw();
    SchedulerParser::Ptr _schParser;
    mutable int _lastMinute;
public:
    ScheduleItemImpl( SchedulerParser::Ptr schParser ) throw();
    virtual ~ScheduleItemImpl() throw();
    virtual bool isNow() const throw();
    virtual const std::string& getTaskName() const throw();
    virtual const std::string& getTaskArgs() const throw();
    
    friend class ScheduleItem_ITEST;
};


END_CXXHTC_UTIL_NAMESPACE
END_CXXHTC_NAMESPACE

#endif // __CXXHTC__LIBRARY__SCHEDULE_UTIL__PRIVATE__H__
