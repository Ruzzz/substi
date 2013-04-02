#pragma once
// win 1251


#include "Substitute.h"


TEST(SubstituteTest, prepareStrings_)
{
    Substitute::Bytes b1, b2;
    Substitute::Substituter substituter;

    ASSERT_TRUE(substituter.prepareStrings_(_T("\\n text ÚÂÍÒÚ"), nullptr, b1, b2));
    ASSERT_TRUE(std::equal(b1.cbegin(), b1.cend(), "\n text ÚÂÍÒÚ"));
    ASSERT_EQ(0, b2.size());

    ASSERT_TRUE(substituter.prepareStrings_(_T("\\n book ÍÌË„‡"), _T("\\n ÚÂÍÒÚ text"), b1, b2));
    ASSERT_TRUE(std::equal(b1.cbegin(), b1.cend(), "\n book ÍÌË„‡"));
    ASSERT_TRUE(std::equal(b2.cbegin(), b2.cend(), "\n ÚÂÍÒÚ text"));

    ASSERT_FALSE(substituter.prepareStrings_(nullptr, nullptr, b1, b2));
}


TEST(SubstituteTest, process_replace_text)
{
    std::string textOld("a€cda€sca€cda€ia");
    Substitute::Bytes bytesOld;
    bytesOld.assign(textOld.begin(), textOld.end());

    std::string textNew("new_text");
    Substitute::Bytes bytesNew;
    bytesNew.assign(textNew.begin(), textNew.end());

    std::istringstream ssOld(std::string("    a€cda€sca€cda€ia tex a€cda€sca€cda€ia "));
    std::ostringstream ssNew;

    Substitute::Substituter substituter;
    ASSERT_TRUE(substituter.process_(ssOld, ssNew, bytesOld, bytesNew));
    ASSERT_EQ(2, substituter.numFound());
    ASSERT_STREQ("    new_text tex new_text ", ssNew.str().c_str());
}


TEST(SubstituteTest, process_replace_text_check_sequential_search)
{
    // See commnets in KnuthMorrisPrattEngine::search()
    // ... j = 0; ...

    Substitute::Bytes bytesOld(2);
    bytesOld[0] = 1;
    bytesOld[1] = 1;

    Substitute::Bytes bytesNew(2);
    bytesNew[0] = '\xFF';
    bytesNew[1] = '\xFF';

    std::istringstream ssOld(std::string(
        "Test€\x01\x01\x01\x01\xAA\x01\x01\x01\xFF"));
    std::ostringstream ssNew;

    Substitute::Substituter substituter;
    ASSERT_TRUE(substituter.process_(ssOld, ssNew, bytesOld, bytesNew));
    ASSERT_EQ(3, substituter.numFound());

    std::string etalon(
        "Test€\xFF\xFF\xFF\xFF\xAA\xFF\xFF\x01\xFF");
    ASSERT_STREQ(etalon.c_str(), ssNew.str().c_str());
}


TEST(SubstituteTest, process_remove_text)
{
    std::string textOld("text");
    Substitute::Bytes bytesOld;
    bytesOld.assign(textOld.begin(), textOld.end());

    std::istringstream ssOld(std::string("    text tex"));
    std::ostringstream ssNew;

    Substitute::Substituter substituter;
    ASSERT_TRUE(substituter.process_(ssOld, ssNew, bytesOld, Substitute::Bytes()));
    ASSERT_EQ(1, substituter.numFound());
    ASSERT_STREQ("     tex", ssNew.str().c_str());
}


TEST(DISABLED_SubstituteTest, process_remove_one_byte_stress)
{
    Substitute::Bytes pattern;
    pattern.push_back('\0');
    const int SIZE = 10 * 1024 * 1024;
    std::istringstream ssOld(std::string(SIZE, '\0'));
    std::ostringstream ssNew;

    Substitute::Substituter substituter;
    ASSERT_TRUE(substituter.process_(ssOld, ssNew, pattern, Substitute::Bytes()));
    ASSERT_EQ(SIZE, substituter.numFound());
    ASSERT_STREQ("", ssNew.str().c_str());
}


TEST(DISABLED_SubstituteTest, process_replace_one_byte_stress)
{
    Substitute::Bytes bytesOld;
    bytesOld.push_back('a');

    Substitute::Bytes bytesNew;
    bytesNew.push_back('b');

    const int SIZE = 10 * 1024 * 1024;
    std::istringstream ssOld(std::string(SIZE, 'a'));
    std::ostringstream ssNew;

    Substitute::Substituter substituter;
    ASSERT_TRUE(substituter.process_(ssOld, ssNew, bytesOld, bytesNew));
    ASSERT_EQ(SIZE, substituter.numFound());
    std::string etalon(SIZE, 'b');
    ASSERT_STREQ(etalon.c_str(), ssNew.str().c_str());
}


TEST(DISABLED_SubstituteTest, process_replace_one_byte_by_text_stress)
{
    Substitute::Bytes bytesOld;
    bytesOld.push_back('a');

    std::string sNew("new_text");
    Substitute::Bytes bytesNew;
    bytesNew.assign(sNew.begin(), sNew.end());

    const int SIZE = 10 * 1024 * 1024;
    std::istringstream ssOld(std::string(SIZE, 'a'));
    std::ostringstream ssNew;

    Substitute::Substituter substituter;
    ASSERT_TRUE(substituter.process_(ssOld, ssNew, bytesOld, bytesNew));
    ASSERT_EQ(SIZE, substituter.numFound());
    std::string etalon;
    for (int i = SIZE; i--;)
        etalon.append("new_text");
    ASSERT_STREQ(etalon.c_str(), ssNew.str().c_str());
}
