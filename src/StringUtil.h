//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#pragma once


#include <vector>

#include "defs.h"


class StringUtil
{
public:

#ifdef _WIN32
    static bool wideToAnsiArray(std::vector<char> &result, const wchar_t *s, int size = -1);
        /// Convert from wide string to vector of ANSI character, without '/0'
#endif  // _WIN32

#ifdef _UNICODE
    static bool prepareAnsi(const tchar *s, std::vector<char> &result)
    {
        return StringUtil::wideToAnsiArray(result, s);
    }
    
    static bool prepareAnsi(const tchar *s, std::string &result)
    {
        std::vector<char> buffer;
        if (StringUtil::wideToAnsiArray(buffer, s))
        {
            result.assign(buffer.begin(), buffer.end());
            return true;
        }
        else
            return false;
    }  
#else  // NOT _UNICODE    
    static bool prepareAnsi(const tchar *s, std::vector<char> &result)
    {
        result.assign(s, s + strlen(s));
        return true;
    }
    
    static bool prepareAnsi(const tchar *s, std::string &result)
    {
        result.assign(s);
        return true;
    } 
#endif  // _UNICODE

private:
    StringUtil();
    StringUtil(StringUtil&);
    void operator=(StringUtil&);
};
