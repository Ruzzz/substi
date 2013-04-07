//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#pragma once


#include "HexUtil.h"


class CppEscapedUtil
{
public:
    template <typename TInIt, typename TOutIt>
    static bool decode(TInIt &first, const TInIt &last, TOutIt &out, unsigned int hexNumsLimit = 2)
        /// Supported: \n \t \v \b \r \f \a \\ \? \' \" \xH...(see hexNumsLimit) \0
        /// Not supported: octal number \000 - \377
    {
        bool escaped = false;
        while (first < last)
        {
            if (escaped)  // Escaped
            {
                if ('x' == *first)  // Hex \xHH
                {
                    ++first;
                    unsigned int b;
                    unsigned int count = HexUtil::toUInt(first, last, b, hexNumsLimit > 2 ? 2 : hexNumsLimit);
                    if (0 == count)
                        return false;
                    *out++ = static_cast<HexUtil::Byte>(b);
                    escaped = false;
                    continue;
                }
                else  // Special characters \n \r \t ...
                {
                    unsigned char c = TABLE.data[static_cast<unsigned char>(*first)];
                    if (255 == c)
                        return false;
                    *out++ = c;
                }
                escaped = false;
            }
            else
            {
                if ('\\' == *first)
                    escaped = true;
                else
                    *out++ = *first;
            }
            ++first;
        }
        return !escaped;  // escaped flag must be reset
    }

    template <typename TCharContainer>
    static bool decodeInPlace(TCharContainer &container, unsigned int hexNumsLimit = 2)
        /// See decode()
    {
        TCharContainer newContainer;
        newContainer.reserve(container.size() * 2);
        bool result = decode(container.begin(), container.end(), std::back_inserter(newContainer), hexNumsLimit);
        container.swap(newContainer);
        return result;
    }

private:
    CppEscapedUtil();
    CppEscapedUtil(CppEscapedUtil&);
    void operator=(CppEscapedUtil&);

    struct TableTag { unsigned char data[256]; };
    static TableTag initialize()
    {
        TableTag t;
        memset(t.data, 255, ARRAY_SIZE(t.data));
        t.data['n'] = '\n';
        t.data['t'] = '\t';
        t.data['v'] = '\v';
        t.data['b'] = '\b';
        t.data['r'] = '\r';
        t.data['f'] = '\f';
        t.data['a'] = '\a';
        t.data['\\'] = '\\';
        t.data['?']  = '\?';
        t.data['\''] = '\'';
        t.data['"']  = '\"';
        t.data['0']  = '\0';
        return t;
    };
    static TableTag TABLE;
};
