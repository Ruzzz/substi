//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#pragma once


#include <cstring>  // memset

#include "defs.h"  // ARRAY_SIZE


class HexUtil
{
public:
    typedef unsigned char Byte;

    static bool charValue(const char ch, Byte &result)
        /// Convert hex character to byte number.
        /// Note: <result> may be corrupt even when return false.
    {
        result = TABLE.data[static_cast<unsigned char>(ch)];
        return result <= '\x0F';
    }

    static bool charValue2(const char ch, Byte &result)
        /// Convert hex character to byte number.
        /// Note: <result> may be corrupt even when return false.
    {
        result = ch;
        if (result > 'f' || result < '0') return false;
        if (result >= 'a')           result -= 'a' - 10;
        else if (result >= 'A')      result -= 'A' - 10;
        else if (result >= '0')      result -= '0';
        return result <= '\x0F';
    }

    static bool toByte(const char s[2], Byte &result)
        /// Convert 2 hex characters to byte number.
    {
        Byte hb, lb;
        bool ok;
        if (ok = charValue(s[0], hb) && charValue(s[1], lb))
            result = (hb << 4) | lb;
        return ok;
    }

    template <typename TInIt>
    static bool toByte(const TInIt &first, const TInIt &last, Byte &result)
        /// Convert 2 hex characters to byte number.
    {
        return (first + 1 < last) ? toByte(first, result) : false;
    }

    template <typename TInIt>
    static unsigned int toUInt(TInIt &first, const TInIt &last,
        unsigned int &result, const unsigned int limit = 8)
        /// Convert at least 1 (if second non hex) character to byte number,
        ///   but not more than <limit>.
        /// Note: <result> may be corrupt even when return 0.
        /// Set <first> after hex characters.
        /// Return number of extracted characters.
    {
        const unsigned int limit_ = limit > 8 ? 8 : limit;
        Byte b;
        result = 0;
        unsigned int count = 0;
        while (count < limit_ && first != last && charValue(*first, b))
        {
            ++count;
            result = (result << 4) | b;
            ++first;
        }
        return count;
    }


    template <typename TInIt>
    static unsigned int toUInt64(TInIt &first, const TInIt &last,
        uint64_t &result, const unsigned int limit = 16)
        /// Convert at least 1 (if second non hex) character to byte number,
        ///   but not more than <limit>.
        /// Note: <result> may be corrupt even when return 0.
        /// Set <first> after hex characters.
        /// Return number of extracted characters.
    {
        const unsigned int limit_ = limit > 16 ? 16 : limit;
        Byte b;
        result = 0;
        unsigned int count = 0;
        while (count < limit_ && first != last && charValue(*first, b))
        {
            ++count;
            result = (result << 4) | b;
            ++first;
        }
        return count;
    }


private:
    HexUtil();
    HexUtil(HexUtil&);
    void operator=(HexUtil&);

    struct TableTag { unsigned char data[256]; };
    static TableTag initialize()
    {
        TableTag t;
        memset(t.data, 255, ARRAY_SIZE(t.data));
        for (char i = '0'; i <= '9'; ++i)
            t.data[i] = i - '0';
        for (char i = 'A'; i <= 'F'; ++i)
            t.data[i] = i - ('A' - 10);
        for (char i = 'a'; i <= 'f'; ++i)
            t.data[i] = i - ('a' - 10);
        return t;
    };
    static TableTag TABLE;
};
