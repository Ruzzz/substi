#pragma once
// win 1251


#include "StringUtil.h"


TEST(StringUtilTest, wideToAnsiArray)
{
    std::vector<char> v;
    ASSERT_TRUE(StringUtil::wideToAnsiArray(v, L"test_тест"));
    ASSERT_TRUE(std::equal(v.cbegin(), v.cend(), "test_тест"));
}


TEST(StringUtilTest, prepareAnsi)
{
    std::vector<char> v;
    std::string s;

    ASSERT_TRUE(StringUtil::prepareAnsi(_T("test_тест"), v));
    ASSERT_TRUE(StringUtil::prepareAnsi(_T("test2_тест2"), s));

    ASSERT_TRUE(std::equal(v.cbegin(), v.cend(), "test_тест"));
    ASSERT_TRUE(s == "test2_тест2");
}
