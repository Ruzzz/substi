#pragma once


#include "CppEscapedUtil.h"


TEST(CppEscapedUtilTest, decodeInPlace_check_escaped)
{
    unsigned char t[256];
    memset(t, 0, ARRAY_SIZE(t));
    t['n'] = 1;
    t['t'] = 1;
    t['v'] = 1;
    t['b'] = 1;
    t['r'] = 1;
    t['f'] = 1;
    t['a'] = 1;
    t['\\'] = 1;
    t['?'] = 1;
    t['\''] = 1;
    t['"'] = 1;
    t['0'] = 1;

    for (int i = 0; i < 256; ++i)
    {
        std::vector<char> v(2);
        v[0] = '\\';
        v[1] = (char)i;
        if (1 == t[i])
            ASSERT_TRUE(CppEscapedUtil::decodeInPlace(v, 2));
        else
            ASSERT_FALSE(CppEscapedUtil::decodeInPlace(v, 2));
    }
}


TEST(CppEscapedUtilTest, decodeInPlace_dont_change_not_escaped)
{
    std::string s;
    for (int i = 0; i < 256; ++i)
    {
        char c  = (char)i;
        if (c != '\\')
            s += c;
    }
    std::vector<char> v(s.begin(), s.end());
    ASSERT_TRUE(CppEscapedUtil::decodeInPlace(v, 2));
    ASSERT_TRUE(std::equal(v.cbegin(), v.cend(), s.begin()));
}


TEST(CppEscapedUtilTest, decodeInPlace_hex)
{
    std::string s("\\x0 \\xf \\xF \\x00 \\xfff \\xFF");
    std::vector<char> v(s.begin(), s.end());
    ASSERT_TRUE(CppEscapedUtil::decodeInPlace(v, 2));

    std::vector<char> v2;
    v2.push_back('\x00');
    v2.push_back(' ');
    v2.push_back('\x0F');
    v2.push_back(' ');
    v2.push_back('\x0F');
    v2.push_back(' ');
    v2.push_back('\x00');
    v2.push_back(' ');
    v2.push_back('\xFF');
    v2.push_back('f');
    v2.push_back(' ');
    v2.push_back('\xFF');
    ASSERT_TRUE(std::equal(v.cbegin(), v.cend(), v2.begin()));
}
