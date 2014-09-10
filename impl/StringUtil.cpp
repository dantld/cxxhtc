/**
 * String utility
 * $Id: StringUtil.cpp 244 2014-02-20 05:17:27Z dantld $
 */
 
#include <string.h>
#include "StringUtil.h"
#include <regex.h>
#include <stdarg.h>
#include <iostream>

BEGIN_CXXHTC_NAMESPACE

using namespace std;

typedef list<string> StringsList;

// trim() - remove space from both sides of string
char *trim(char *lpStr)
{
	ltrim(lpStr);
	return rtrim(lpStr);
}

// ltrim() - remove space from left side of string
char *ltrim(char *lpStr)
{
	char *p = lpStr;

	for (; *p != 0 && *p == ' '; p++) ;
	if (p != lpStr)
		strcpy(lpStr, p);
	return lpStr;
}

// rtrim() - remove space from right side of string
char *rtrim(char *S)
{
	for (char *p = S + strlen(S) - 1; p >= S && *p == ' '; *p-- = 0) ;
	return S;
}

std::string& trim( std::string &str ) {
    string::size_type pos = str.find_last_not_of(' ');
    if(pos != string::npos) {
        str.erase(pos + 1);
	pos = str.find_first_not_of(' ');
	if(pos != string::npos) str.erase(0, pos);
    }
    else str.erase(str.begin(), str.end());
    return str;
}

std::string& ltrim( std::string &str ) {
    string::size_type pos = str.find_first_not_of(' ');
    if( pos != string::npos ) str.erase( 0 , pos );
    else str.erase(str.begin(), str.end());
    return str;
}

std::string& rtrim( std::string &str ) {
    string::size_type pos = str.find_last_not_of(' ');
    if( pos != string::npos ) str.erase(pos + 1);
    else str.erase(str.begin(), str.end());
    return str;
}

int changeSymbols(char *lpStr, char cFinded, char cReplaced)
{
	int i = 0;

	for (; *lpStr; lpStr++)
	{
		if (*lpStr == cFinded)
			*lpStr = cReplaced, i++;
	}
	return i;
}

/**
 * Make string with upper case contents
 */
std::string string_toupper( const std::string& s ) throw()
{
    std::locale loc;
    std::string::const_iterator it = s.begin();
    std::string result;
    for( ; it != s.end(); it++ ) {
	result += std::toupper( *it, loc );
    }
    return result;
}

/**
 * Make string with lower case contents
 */
std::string string_tolower( const std::string& s ) throw()
{
    std::locale loc;
    std::string::const_iterator it = s.begin();
    std::string result;
    for( ; it != s.end(); it++ ) {
	result += std::tolower( *it, loc );
    }
    return result;
}

const int MAX_BREAK_PART_LENGTH = 80;

void break_String( 
    int maxCh,
    const char *str,
    StringsList& list,
    int maxParts, 
    const char *start_marker,
    const char *rest_marker 
    )
{
    int partLength = maxCh;
    if( str == NULL ) return;
    int length = strlen( str );
    if( length == 0 ) return;
    int start_marker_length = 0;
    int rest_marker_length = 0;
    std::string start_marker_string;
    std::string rest_marker_string;
    if( start_marker != NULL ) {
	start_marker_length = strlen( start_marker );
	start_marker_string = start_marker;
    }
    if( rest_marker != NULL ) {
	rest_marker_length = strlen( rest_marker );
	rest_marker_string = rest_marker;
    }
    if( partLength > MAX_BREAK_PART_LENGTH ) partLength = MAX_BREAK_PART_LENGTH;
    partLength -= start_marker_length;
    if( length <= partLength ) {
	list.push_back( start_marker_string + str );
	return;
    }

    for( const char *p = str; *p != 0;  ) {
	int offset = partLength - 1;
	if( p + offset - str >= length ) offset = str + length - p;
	const char *end = p + offset;
	for( ; end >= p && strchr(" .,!?:;", *end ) == NULL; end-- );
	if( end <= p ) end = p + offset; else if( *end ) end++;
	std::string bstr( p, end - p );
	if( list.empty() ) {
	    list.push_back( start_marker_string + bstr );
	} else {
	    list.push_back( rest_marker_string + bstr );
	}
	p = end;
	if( maxParts > 0 && list.size() >= maxParts ) {
	    break;
	}
	if( p >= str + length ) break;
	if( list.size() == 1 ) {
	    partLength = maxCh;
	    if( partLength > MAX_BREAK_PART_LENGTH ) partLength = MAX_BREAK_PART_LENGTH;
	    partLength -= rest_marker_length;
	}
    }
}

/**
 * Copy and break string.
 * Break long line on chunk with requested bytes length.
 * Be shure that destination buffer has enough space.
 * @parameter pD - destination buffer
 * @parameter pS - source buffer
 * @parameter lS - length of source buffer
 * @parameter chunkSize - size of break chunk
 * @return bytes copied to destination buffer
 */
int break_String( char *pD, const char *pS, int lS, int chunkSize ) {
    int lD = 0;
    for( int i = 0, l = 1; i < lS; i++, l++, lD++ ) {
	*pD++ = *pS++;
	if( l >= chunkSize ) {
	    l = 0;
	    lD += 2;
	    *pD++ = 0x0D;
	    *pD++ = 0x0A;
	}
    }
    *pD++ = 0x0D;
    *pD++ = 0x0A;
    lD += 2;
    return lD;
}


RegExpError::RegExpError(const char*msg) throw() : std::logic_error(msg) {
}

RegExp::RegExp( const char * reval ) throw(RegExpError) 
{
    _re = new regex_t;
    if( regcomp( (regex_t*)_re, reval, REG_EXTENDED ) != 0 ) {
	throw RegExpError("reg pattern compile error");
    }
}

RegExp::~RegExp() throw() {
    regfree( (regex_t*)_re );
    delete (regex_t*)_re;
}

bool RegExp::match( const char *str, int c, ... ) throw(RegExpError) {
	int reg_match_count = c;
	if( c != 0 && c != ((regex_t*)_re)->re_nsub ) {
	    throw RegExpError("Number of parenthesized subexpression not match");
	}
	va_list args;
	va_start( args, c );
	regmatch_t *match = new regmatch_t[ reg_match_count + 1 ];
        int error = regexec( (regex_t*)_re, str, reg_match_count + 1, match, 0 );
        if( error == REG_NOMATCH ) {
	    delete [] match;
	    va_end(args);
	    return false;
	} else if (error != 0) {
	    char buffer[256];
	    int msize = regerror( error, (regex_t*)_re, buffer, 256 );
	    delete []match;
	    va_end(args);
	    throw RegExpError( buffer );
	}
	for( int i = 1; i <= reg_match_count; i++ ) {
	    char* buf = va_arg( args, char* );
	    int   len = va_arg( args, int );
	    if( len - 1 > match[i].rm_eo - match[i].rm_so ) {
		len = match[i].rm_eo - match[i].rm_so;
	    }
    	    strncpy( buf, str + match[i].rm_so, len );
	    buf[len] = 0;
	}
	va_end(args);
	delete [] match;
	return true;
}

bool RegExp::match( const std::string& str, StringsVector& v )
    throw(RegExpError)
{
	int reg_match_count = ((regex_t*)_re)->re_nsub;
	regmatch_t *match = new regmatch_t[ reg_match_count + 1 ];
        int error = regexec( (regex_t*)_re, str.c_str(), reg_match_count + 1, match, 0 );
        if( error == REG_NOMATCH ) {
	    delete [] match;
	    return false;
	} else if (error != 0) {
	    char buffer[256];
	    int msize = regerror( error, (regex_t*)_re, buffer, 256 );
	    delete []match;
	    throw RegExpError( buffer );
	}
	for( int i = 1; i <= reg_match_count; i++ ) {
	    std::string strPart;
	    strPart.assign( str.c_str() + match[i].rm_so, match[i].rm_eo - match[i].rm_so );
	    v.push_back( strPart );
//fprintf(stderr,"%d)\t%s\n", i, strPart.c_str() );
	}
	delete [] match;
	return true;
}

END_CXXHTC_NAMESPACE
