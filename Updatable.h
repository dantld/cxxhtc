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
#ifndef __CXXHTC__LIBRARY__UPDATABLE__INTERFACE__H__
#define __CXXHTC__LIBRARY__UPDATABLE__INTERFACE__H__

BEGIN_CXXHTC_NAMESPACE

class UpdateError : public std::exception {
    std::string _message;
public:
    UpdateError();
    UpdateError( const std::string &message );
    ~UpdateError() throw();
    const char* what() const throw();
};

class Updatable {
protected:
     Updatable() throw();
    virtual ~Updatable() throw();
public:
    virtual void update() throw(UpdateError) = 0;
};

END_CXXHTC_NAMESPACE

#endif //__CXXHTC__LIBRARY__UPDATABLE__INTERFACE__H__
