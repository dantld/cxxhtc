/**
 * $Id: Password.h 97 2009-11-26 09:32:02Z dantld $
 * C++ High Tool Cat Kit Classes library.
 * Iface for retrieve passwords from external file
 * by server name and user name pair.
 */
#ifndef __CPPHTC__PASSWORD__CLASS__H__
#define __CPPHTC__PASSWORD__CLASS__H__
#include <cxxhtc_defs.h>
#include <string>
#include <stdexcept>
#include <SmartPointers.h>

BEGIN_CXXHTC_NAMESPACE

class PasswordError : public std::exception {
public:
    ~PasswordError() throw();
};

class Password : public Obj {
protected:
    Password() throw();
public:
    virtual ~Password() throw();
    typedef SmartPtr<Password> Ptr;
    virtual void setFileName( const std::string& ) = 0 ;
    virtual std::string getPassword( const std::string&, const std::string& ) throw(PasswordError) = 0;
    static Ptr create();
};
END_CXXHTC_NAMESPACE
#endif

