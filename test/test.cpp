//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#include <gtest/gtest.h>

#include "HexUtilTest.h"
#include "CppEscapedUtilTest.h"
//#include "StringUtilTest.h"
#include "KnuthMorrisPrattEngineTest.h"
#include "SubstituteTest.h"


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    // testing::GTEST_FLAG(also_run_disabled_tests) = true;
    return RUN_ALL_TESTS();
}
