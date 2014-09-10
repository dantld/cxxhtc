/**
 * String utitlity
 *
 * $Id: StringUtil.h 244 2014-02-20 05:17:27Z dantld $
 */

#ifndef __CXXHTC__LIBRARY__STRING_UTIL__H__
#define __CXXHTC__LIBRARY__STRING_UTIL__H__

#include <cxxhtc_defs.h>
#include <vector>
#include <list>
#include <string>
#include <stdexcept>

BEGIN_CXXHTC_NAMESPACE

/**
 * @defgroup stringutil Подпрограммы манипуляций со строками.
 * 
 */
/*@{*/
/**
 * Определение списка строк, используемых функцией break_String.
 */
typedef std::list<std::string> StringsList;
/**
 * The type definition of vector fo string.
 */
typedef std::vector<std::string> StringsVector;
/**
 * Подрезка строки слева и справа.
 * @param lpStr - указатель на строку для подрезки, завершённую нулём.
 */
char* trim(char* lpStr);
/**
 * Подрезка строки с левой стороны.
 * @param lpStr - указатель на строку для подрезки, завершённую нулём.
 */
char* ltrim(char* lpStr);
/**
 * Подрезка строки с правой стороны.
 * @param lpStr - указатель на строку для подрезки, завершённую нулём.
 */
char* rtrim(char* lpStr);
/**
 * Подрезка строки слева и справа.
 * @param str - ссылка на объект std::string.
 * @return ссылка на параметр str.
 */
std::string&  trim( std::string &str );
/**
 * Подрезка строки с левой стороны.
 * @param str - ссылка на объект std::string.
 * @return ссылка на параметр str.
 */
std::string& ltrim( std::string &str );
/**
 * Подрезка строки с правой стороны.
 * @param str - ссылка на объект std::string.
 * @return ссылка на параметр str.
 */
std::string& rtrim( std::string &str );
/**
 * Замена символов в строке.
 * @param lpStr - указатель на строку для замены символа, завершённую нулём.
 * @param cFinded - заменяемый символ.
 * @param cReplaced - новое значение заменяемого символа.
 * @return Количество заменённых символов, если замен не было, возвращается ноль.
 */
int changeSymbols(char* lpStr, char cFinded, char cReplaced);

/**
 * Make string with upper case contents
 */
std::string string_toupper( const std::string& s ) throw();

/**
 * Make string with lower case contents
 */
std::string string_tolower( const std::string& s ) throw();

/**
 * Разбить длинную строку на части с длиной не более maxCh.
 * Предусмотрен start_marker перед первой строкой.
 * Предусмотрен end_marker перед последующими строками.
 * Число частей ограничено maxParts.
 * Длина части строки, кроме того, ограничена параметром 
 * MAX_BREAK_PART_LENGTH, в настоящее время равным 80.
 */
void break_String(
    int maxCh,
    const char *str,
    StringsList& list,
    int maxParts = 0,
    const char *start_marker = NULL,
    const char *rest_marker = NULL
    );

/**
 * Copy and break string.
 * Break long line on chunk with requested bytes length.
 * Each chunk of string will be terminated by EOL (0x0D)
 * and NL (0x0A) symbols.
 * Be shure that destination buffer has enough space.
 * We suggest
 * @parameter pD - destination buffer
 * @parameter pS - source buffer
 * @parameter lS - length of source buffer
 * @parameter chunkSize - size of break chunk
 * @return bytes copied to destination buffer
 */
int break_String( char *pD, const char *pS, int lS, int chunkSize );

/**
 * Класс исключений для методов RegExp.
 */
class RegExpError : public std::logic_error {
public:
    RegExpError( const char *msg ) throw();
};

/**
 * Класс для выполнения регулярных выражений.
 */
class RegExp {
    void *_re;
public:
    /**
     * Создание экземпляра с запрошенным регулярным выражением.
     * @param reval - указатель на строку с регулярным выражением, завершённую нулём.
     * @exception Исключение RegExpError возникает, если регулярное выражение не удаётся выполнить. 
     */
    RegExp( const char * reval ) throw(RegExpError);
    ~RegExp() throw();
    /**
     * Поиск строк с помощью регулярных выражений.
     * Если Ваше регулярное выражение содержит описания групп,
     * например, типа '(\\d+)', Вы можете найти в просматриваемом 
     * тексте строки, соответствующие этим группам. 
     * С этой целью для каждой группы нужно указать
     * текстовый буфер, в который будет записан результат поиска, 
     * и длину данного буфера.
     * @param str - просматриваемый текст в виде указателя на строку,
     * завершённой нулём.
     * @param count - количество пар буфер-длина буфера или, что то же
     * самое, количество описаний групп в регулярном выражении. 
     * Значение параметра count не должно превышать 10.
     * После параметров str и count следуют пары вроде:
     * char* buf1, int buf_length1, char* buf2, int buf_length2, ...
     * Количество пар должно соответствовать параметру count.
     *
     * Пример использования:
     * @include RegExp_example.cpp
     * @return Возвращается true в случае соответствия строки регулярному выражению.
     * @exception Исключение RegExpError возникает, если regexec() возвращает ошибку.
     * Исключение возникает также, если количество буферов (параметр count) не соответствует
     * количеству групп.
     */
    bool match( const char *str, int count = 0, ... ) throw(RegExpError);
    bool match( const std::string& str, StringsVector& v ) throw(RegExpError);
};

/*@}*/

END_CXXHTC_NAMESPACE

#endif // __CXXHTC__LIBRARY__STRING_UTIL__H__
