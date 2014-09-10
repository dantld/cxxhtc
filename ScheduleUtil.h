/**
 * Time utitlity
 *
 * $Id: ScheduleUtil.h 164 2010-03-11 11:32:16Z dantld $
 */

#ifndef __CXXHTC__LIBRARY__SCHEDULE_UTIL__H__
#define __CXXHTC__LIBRARY__SCHEDULE_UTIL__H__

#include <cxxhtc_defs.h>
#include <string>
#include <stdexcept>
#include <time.h>
#include <SmartPointers.h>

BEGIN_CXXHTC_NAMESPACE
BEGIN_CXXHTC_UTIL_NAMESPACE

/**
 * @defgroup timeutil miscellaneous time functions, classes and etc.
 * 
 */
/*@{*/
/**
 * SchedulerParser - simple parser for string like crontab used.
 * Current implementation does not support a ranges and enumerations
 * only exactly sets numbers.
 */
class SchedulerParser : public Obj {
public:
    typedef SmartPtr<SchedulerParser> Ptr;
    SchedulerParser() throw();
    virtual ~SchedulerParser() throw();
    /**
     * Set, parse and apply the shcedule string.
     * The implementation have to clear all states before apply new schedule string.
     * @param schedule string with new schedule
     * @exception when string does not match the format or value are lay outside the domain.
     */
    virtual void setString( const std::string& schedule ) throw(std::runtime_error) = 0;
    /**
     * Check that requested value of minute are used by schedule.
     * @return true if requested value are used by schedule.
     */
    virtual bool isMin(     int v ) const throw() = 0;
    /**
     * Check that requested value of hour are used by schedule.
     * @return true if requested value are used by schedule.
     */
    virtual bool isHour(    int v ) const throw() = 0;
    /**
     * Check that requested value of day of month are used by schedule.
     * @return true if requested value are used by schedule.
     */
    virtual bool isDay(     int v ) const throw() = 0;
    /**
     * Check that requested value of month are used by schedule.
     * @return true if requested value are used by schedule.
     */
    virtual bool isMonth(   int v ) const throw() = 0;
    /**
     * Check that requested value of day of week are used by schedule.
     * @return true if requested value are used by schedule.
     */
    virtual bool isWeekDay( int v ) const throw() = 0;
    /**
     * Today based on data (day of month and month) and week day.
     * @return true if event must occurs today else false
     */
    virtual bool isToday() const throw() = 0;
    /**
     * Check that event must occurs now.
     * @return true if event must occurs now else false
     */
    virtual bool isNow() const throw() = 0;
    /**
     * Get task name
     */
    virtual const std::string& getTaskName() const throw() = 0;
    /**
     * Get task arguments
     */
    virtual const std::string& getTaskArgs() const throw() = 0;
    /**
     * @return seconds pass from the last event
     */
    virtual time_t getPassed() const throw() = 0;
    /**
     * @return seconds remain to the next event
     */
    virtual time_t getRemain() const throw() = 0;
    /**
     * Create the default implementation offer by the library.
     * @return the smart pointer on implementation instance.
     */
    static Ptr createInstance() throw();
};

class ScheduleItem : public Obj {
public:
    typedef SmartPtr<ScheduleItem> Ptr;
    ScheduleItem() throw();
    virtual ~ScheduleItem() throw();
    virtual bool isNow() const throw() = 0;
    virtual const std::string& getTaskName() const throw() = 0;
    virtual const std::string& getTaskArgs() const throw() = 0;
};

class ScheduleList : public Obj {
public:
    typedef SmartPtr<ScheduleList> Ptr;
    ScheduleList() throw();
    virtual ~ScheduleList() throw();
    virtual void addEvent( const std::string& schStr ) throw(std::runtime_error) = 0;
    
    virtual void begin() throw() = 0;
    virtual bool end() throw() = 0;
    virtual void next() throw() = 0;
    virtual ScheduleItem::Ptr current() throw() = 0;

    virtual bool empty() const throw() = 0;

    static Ptr createInstance() throw();
};

/*@}*/

END_CXXHTC_UTIL_NAMESPACE
END_CXXHTC_NAMESPACE

#endif // __CXXHTC__LIBRARY__SCHEDULE_UTIL__H__
