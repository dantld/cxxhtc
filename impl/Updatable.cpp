/**
 * $Id$
 * CXXHTC library
 * Updatable iface.
 * Updatable interface need for objects which
 * require update from time to time.
 * For using updatable infrastructure classes
 * must implement Updatable interface, add configuration
 * for updating the instance.
 */

#include <stdexcept>
#include <string>

#include "../cxxhtc_defs.h"
#include "../Updatable.h"

BEGIN_CXXHTC_NAMESPACE

UpdateError::UpdateError() :
    _message("Unnamed update error")
{
}
UpdateError::UpdateError( const std::string &message ) :
    _message(message)
{
}
UpdateError::~UpdateError() throw()
{
}
const char* UpdateError::what() const throw() 
{
    return _message.c_str();
}


Updatable::Updatable() throw()
{
}

Updatable::~Updatable() throw()
{
}

END_CXXHTC_NAMESPACE

