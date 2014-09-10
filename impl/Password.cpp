/**
 * $Id: Password.cpp 213 2011-08-11 10:05:35Z dantld $
 * C++ High Tool Cat Kit Classes library.
 * Iface for retrieve passwords from external file
 * by server name and user name pair.
 */
#include "../Password.h"
#include "../StringUtil.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
BEGIN_CXXHTC_NAMESPACE

PasswordError::~PasswordError() throw() {}
    
class PasswordErrorImpl : public PasswordError {
    std::string _message;
public:
    PasswordErrorImpl(const char *whatsUp) throw() : _message(whatsUp) 
    {
    }
    ~PasswordErrorImpl() throw() {}
    const char* what() const throw() { return _message.c_str(); }
};

Password::Password() throw() {
}

Password::~Password() throw() {
}


class PasswordImpl : public Password {
    static char PasswordPattern[];
    FILE *_fd;
    std::string _file_name;
public:
    PasswordImpl() throw();
    ~PasswordImpl() throw();
    virtual void setFileName( const std::string& file_name ) {
	_file_name = file_name;
	open();
    }
    virtual std::string getPassword( const std::string& s, const std::string& u ) throw(PasswordError) {
	return find( s, u );
    }
    void open() throw(PasswordError);
    void close() throw(PasswordError);
    std::string find( const std::string& s, const std::string& u ) throw(PasswordError);
};

char PasswordImpl::PasswordPattern[] = "^%s:%s:(.+)$";

PasswordImpl::PasswordImpl() throw() :
    _fd(0)
{
}

PasswordImpl::~PasswordImpl() throw() {
    close();
}

void PasswordImpl::open()  throw(PasswordError) {
    _fd = ::fopen( _file_name.c_str(), "r" );
    if( _fd == NULL ) {
        throw PasswordErrorImpl("Password: Cannot open file");
    }
}
		    
void PasswordImpl::close() throw(PasswordError) {
    if( _fd != NULL ) fclose(_fd);
    _fd = NULL;
}

std::string PasswordImpl::find( const std::string& s, const std::string& u ) throw(PasswordError)
{
    char reBuf[256];
    snprintf( reBuf, 256, PasswordPattern, s.c_str(), u.c_str() );
    RegExp re( reBuf );
    char buffer[1024];
    char password[128];
    bool found = false;
    while( fgets( buffer, 120, _fd ) != NULL ) {
            buffer[strlen(buffer)-1] = 0;
	    if( re.match( buffer, 1, password, 128 ) ) {
		found = true;
		break;
	    }
    }
    if( !found ) {
	throw PasswordErrorImpl("Password: not found");
    }
    return std::string(password);
}

			    
Password::Ptr Password::create() {
    return Password::Ptr(new PasswordImpl());
}

END_CXXHTC_NAMESPACE
