//
// Project: Substitute
// Date:    2013-04-02
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#include <iostream>


#include "KnuthMorrisPrattEngine.h"
#include "OneByteEngine.h"

#include "SearchInFileEngineFactory.h"


SearchInFileEngineBase* SearchInFileEngineFactory::get(SearchInFileEngineBase::Type type)
{

#ifdef _DEBUG
    switch (type)
    {
        case SearchInFileEngineBase::Type::KNUTH_MORRIS_PRATT: std::cout << "KNUTH_MORRIS_PRATT\n"; break;
        case SearchInFileEngineBase::Type::ONE_BYTE:           std::cout << "ONE_BYTE\n"; break;
    }
#endif // _DEBUG

    switch (type)
    {
        case SearchInFileEngineBase::Type::KNUTH_MORRIS_PRATT: return new KnuthMorrisPrattEngine;
        case SearchInFileEngineBase::Type::ONE_BYTE:           return new OneByteEngine;
        default:                 return nullptr;
    }
}
