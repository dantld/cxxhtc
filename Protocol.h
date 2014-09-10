/**
 * Message logger
 * $Id: Protocol.h 111 2009-12-28 12:24:40Z olegm $
 */

#ifndef __MESSAGE__LOGGER__CLASS__H__
#define __MESSAGE__LOGGER__CLASS__H__
#include <cxxhtc_defs.h>
#include <stdarg.h>
#include <AbstractFactory.h>

BEGIN_CXXHTC_NAMESPACE

class Config;
/**
 * Интерфейс Protocol - абстрактный класс для ведения журналов в Вашем приложении.
 */
class Protocol {
protected:
    Protocol();
    virtual ~Protocol() throw();
public:
    /**
     * @deprecated
     * Запись различных сообщений.
     * @param fmt - формат строки, подобный используемому в функции printf() из stdio.
     */
    virtual void log(const char *fmt, ...) throw() = 0;
    /**
     * Запись сообщения об ошибке.
     * @param source - указатель на строку, завершённую нулём, с информацией об источнике данного сообщения.
     * @param fmt - формат строки, подобный используемому в функции printf() из stdio.
     */
    virtual void logError( const char* source, const char *fmt, ... ) throw() = 0;
    /**
     * Запись сообщения о предупреждении.
     * @param source - указатель на строку, завершённую нулём, с информацией об источнике данного сообщения.
     * @param fmt - формат строки, подобный используемому в функции printf() из stdio.
     */
    virtual void logWarning( const char* source, const char *fmt, ... ) throw() = 0;
    /**
     * Запись информационного сообщения.
     * @param source - указатель на строку, завершённую нулём, с информацией об источнике данного сообщения.
     * @param fmt - формат строки, подобный используемому в функции printf() из stdio.
     */
    virtual void logInfo( const char* source, const char *fmt, ... ) throw() = 0;
    /**
     * Запись отладочного сообщения.
     * @param source - указатель на строку, завершённую нулём, с информацией об источнике данного сообщения.
     * @param fmt - формат строки, подобный используемому в функции printf() из stdio.
     */
    virtual void logDebug( const char* source, int level, const char *fmt, ... ) throw() = 0;
    static void create( const Config* );
    static void destroy();
};


extern Protocol *protocol;

END_CXXHTC_NAMESPACE
#endif //  __MESSAGE__LOGGER__CLASS__H__


