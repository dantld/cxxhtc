/**
 * Smart pointers template classes.
 * $Id: SmartPointers.h 217 2011-08-19 12:06:59Z dantld $
 */

#ifndef __SMART__POINTERS__CLASSES__H__
#define __SMART__POINTERS__CLASSES__H__
#include <cxxhtc_defs.h>
#include <stdexcept>
#include <iostream>
#include <typeinfo>

BEGIN_CXXHTC_NAMESPACE

/**
 * Классический автоуказатель.
 */
template  <class T> class AutoPtr {
    AutoPtr() {}
    T *_pObj;
public:
    AutoPtr(T *pObj) : _pObj(pObj) {}
    ~AutoPtr() { delete _pObj; }
    T* operator->() { return _pObj; }
    const T* operator->() const { return _pObj; }
    T* getPtr() { return _pObj; }
    const T* getPtr() const { return _pObj; }
};

/**
 * Классический автоуказатель. Array version
 */
template  <class T> class ArrayAutoPtr {
    ArrayAutoPtr() {}
    T *_pObj;
public:
    ArrayAutoPtr(T *pObj) : _pObj(pObj) {}
    ~ArrayAutoPtr() { delete []_pObj; }
    T* operator->() { return _pObj; }
    const T* operator->() const { return _pObj; }
    T* get() { return _pObj; }
    const T* get() const { return _pObj; }
    T* release() { T* pObj = _pObj; _pObj = NULL; return pObj; }
    const T* release() const { const T* pObj = _pObj; _pObj = NULL; return pObj; }
};

/**
 * Вспомогательный класс - счётчик ссылок.
 */
class Obj {
    mutable int refcnt;
protected:
    Obj();
public:
    virtual ~Obj();
    int incRefCnt() throw();
    int decRefCnt() throw();
    int  getRefCnt() throw();
};

/**
 * Классический класс умных указателей.
 */
template <class T> class SmartPtr {
    mutable T *_pObj;
public:
    SmartPtr() : _pObj(0) {}
    explicit SmartPtr(T *pObj) throw() 
	: _pObj(pObj)
    {
	_pObj->incRefCnt(); 
    }
    SmartPtr( const SmartPtr<T> &p ) throw()
	: _pObj(p._pObj)
    {
	_pObj->incRefCnt(); 
    }
    ~SmartPtr() {
	if( _pObj != 0 ) {
	    if( _pObj->decRefCnt() == 0 ) {
		delete dynamic_cast<Obj*>(_pObj);
	    }
	}
    }
    SmartPtr& operator=( SmartPtr<T> &p ) { 
	if( _pObj != p._pObj ) {
	    if( _pObj != NULL && _pObj->decRefCnt() == 0 ) delete dynamic_cast<Obj*>(_pObj);
	    _pObj = p._pObj;
	    _pObj->incRefCnt(); 
	}
	return *this;
    }
    const SmartPtr& operator=( const SmartPtr<T> &p ) const { 
	if( _pObj != p._pObj ) {
	    if( _pObj != NULL && _pObj->decRefCnt() == 0 ) delete dynamic_cast<Obj*>(_pObj);
	    _pObj = p._pObj;
	    _pObj->incRefCnt();
	}
	return *this;
    }
    T* operator->() {
	if( _pObj == 0 ) {
	    std::string s = "Smart pointer \"";
	    s += typeid(T).name();
	    s += "\" has NULL object pointer";
	    throw std::runtime_error( s.c_str() );
	}
	return _pObj;
    }
    const T* operator->() const {
	if( _pObj == 0 ) {
	    std::string s = "Smart pointer \"";
	    s += typeid(T).name();
	    s += "\" has NULL object pointer";
	    throw std::runtime_error( s.c_str() );
	}
	return _pObj;
    }
    bool operator==( const SmartPtr<T>& r ) const {
	return *_pObj == *r._pObj;
    }
    bool operator!=( const SmartPtr<T>& r ) const {
	return *_pObj != *r._pObj;
    }
    friend std::ostream& operator<<( std::ostream &os, const SmartPtr<T>& ptr ) {
	return os << *ptr._pObj;
    }
};

/**
 * Классический класс разделяемых указателей.
 */
template <class T> class SharedPtr {
    mutable T *_pObj;
    SharedPtr() : _pObj(0) {}
public:
    SharedPtr(T *pObj) : _pObj(pObj) {}
    SharedPtr( const SharedPtr<T> &p ) : _pObj(p._pObj) { p._pObj = 0; }
    ~SharedPtr() { if( _pObj != 0 ) { delete _pObj; } }
    SharedPtr& operator=( const SharedPtr<T> &p ) { _pObj = p._pObj; p._pObj = 0; }
    T* operator->() { return _pObj; }
    const T* operator->() const { return _pObj; }
};


END_CXXHTC_NAMESPACE
#endif // __SMART__POINTERS__CLASSES__H__
