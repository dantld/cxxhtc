/**
 * Common errors - is a set of custom exception classes for
 * 		   using it in a suitable cases of errors
 *
 * $Id: CommonErrors.cpp 213 2011-08-11 10:05:35Z dantld $
 */
#include "CommonErrors.h"
#include <sstream>
#include <string.h>

BEGIN_CXXHTC_NAMESPACE

BadSqlReturnCode::BadSqlReturnCode() {
}
BadSqlReturnCode::~BadSqlReturnCode() throw() {
}
BadSqlReturnCode::BadSqlReturnCode( const BadSqlReturnCode& o ) {
    _message = o._message;
    _returnCode = o._returnCode;
    _sqlStatement = o._sqlStatement;
}
BadSqlReturnCode::BadSqlReturnCode( const char* message, int retcode, const char *sqlStatement ) :
    _returnCode(retcode),
    _sqlStatement(sqlStatement)
{
    std::ostringstream os;
    os << message << " RetVal: " << _returnCode << ". Stmt: \"" << _sqlStatement + "\"";
    _message = os.str().c_str();
}
const char* BadSqlReturnCode::what() const throw() {
    return _message.c_str();
}
int BadSqlReturnCode::getReturnCode() const throw() {
    return _returnCode;
}
const char* BadSqlReturnCode::getSqlStatement() const throw() {
    return _sqlStatement.c_str();
}

ResourceBusy::ResourceBusy(const char* message) :
    _message(message)
{
}
ResourceBusy::~ResourceBusy() throw() {
}
const char* ResourceBusy::what() const throw() {
    return _message;
}
 
/* exception from loding streams*/
BadLoading::BadLoading() {
}
BadLoading::~BadLoading() throw() {
}
BadLoading::BadLoading(const BadLoading& o ) {
    _message = o._message;
    _returnCode = o._returnCode;
}
BadLoading::BadLoading( const char* message, int retcode) :
    _message(message),
    _returnCode(retcode)
{
}
const char* BadLoading::what() const throw() {
    return _message.c_str();
}
int BadLoading::getReturnCode() const throw() {
    return _returnCode;
}

SyncError::SyncError( int e, const char* m ) throw() :
    _errno(e)
{
    strncpy( _message, m, MAX_MESSAGE_LENGTH-1 )[MAX_MESSAGE_LENGTH-1] = 0;
}
SyncError::~SyncError() throw() {
}
SyncError::SyncError( const SyncError& r ) throw() {
    _errno = r._errno;
    strcpy( _message, r._message );
}
SyncError& SyncError::operator=( const SyncError& r ) throw() {
    _errno = r._errno;
    strcpy( _message, r._message );
    return *this;
}
const char* SyncError::what() const throw() {
    return _message;
}



END_CXXHTC_NAMESPACE

