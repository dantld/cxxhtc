/**
 * Common errors - is a set of custom exception classes for
 * 		   using it in a suitable cases of errors
 *
 */
#ifndef __KISC__COMMON__ERRORS__H__
#define __KISC__COMMON__ERRORS__H__

#include <cxxhtc_defs.h>
#include <stdexcept>
#include <string>



BEGIN_CXXHTC_NAMESPACE

using std::exception;
using std::string;

const int MAX_MESSAGE_LENGTH = 80;
/**
 * Определение возвращаемого кода для общих исключений (common exception).
 *
**/
const int OUT_OF_RANGE		= -101;
const int RESURCE_IS_BUSY	= -102;
const int BAD_ALLOCATE		= -103;

const int FAILED_OPEN		= -201;
const int BAD_DATA_READ		= -202;


/**
 * Исключение следует применять при возникновении неожиданного
 * возвращаемого кода от функции или хранимой процедуры.
 */
class BadSqlReturnCode : public exception {
    int _returnCode;
    string _message;
    string _sqlStatement;
    BadSqlReturnCode();
public:
    ~BadSqlReturnCode() throw();
    BadSqlReturnCode( const char* message, int retcode, const char *sqlStatement );
    BadSqlReturnCode( const BadSqlReturnCode& );
    const char* what() const throw();
    int getReturnCode() const throw();
    const char* getSqlStatement() const throw();
};

/**
 * Исключение следует применять в случае возникновения 
 * ошибки при загрузке потока, выделении памяти, или если
 * не удаётся предоставить буфер для объекта.
 */
class BadLoading : public exception {
    int _returnCode;
    string _message;
    BadLoading();
public:
    ~BadLoading() throw();
    BadLoading( const char* message, int retcode);
    BadLoading( const BadLoading& );
    const char* what() const throw();
    int getReturnCode() const throw();
};

class ResourceBusy : public exception {
    const char* _message;
	ResourceBusy();
public:
    ResourceBusy ( const char* message);
    ~ResourceBusy() throw();
	const char* what() const throw();
};

class SyncError : public exception {
    int _errno;
    char _message[MAX_MESSAGE_LENGTH];
public:
    SyncError( int, const char* ) throw();
    ~SyncError() throw();
    SyncError( const SyncError& ) throw();
    SyncError& operator=( const SyncError& ) throw();
    const char* what() const throw();
};

END_CXXHTC_NAMESPACE

#endif //__KISC__COMMON__ERRORS__H__
