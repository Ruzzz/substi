#pragma once
// win 1251


#include "KnuthMorrisPrattEngine.h"


TEST(KnuthMorrisPrattEngineTest, computePrefixes)
{
    std::string s("a€cda€sca€cda€ia");
    KnuthMorrisPrattEngine::Bytes b;
    b.assign(s.begin(), s.end());

    KnuthMorrisPrattEngine::Prefixes p;
    KnuthMorrisPrattEngine::computePrefixes(b, p);

    KnuthMorrisPrattEngine::Prefixes etalon;
    etalon.push_back(0);
    etalon.push_back(0);
    etalon.push_back(0);
    etalon.push_back(0);
    etalon.push_back(1);
    etalon.push_back(2);
    etalon.push_back(0);
    etalon.push_back(0);
    etalon.push_back(1);
    etalon.push_back(2);
    etalon.push_back(3);
    etalon.push_back(4);
    etalon.push_back(5);
    etalon.push_back(6);
    etalon.push_back(0);
    etalon.push_back(1);
    ASSERT_TRUE(std::equal(p.cbegin(), p.cend(), etalon.cbegin()));
}
