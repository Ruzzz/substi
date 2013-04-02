//
// Project: Substitute
// Date:    2013-04-02
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#pragma once


#include "SearchInFileEngineBase.h"


class SearchInFileEngineFactory
{
public:
    static SearchInFileEngineBase* get(SearchInFileEngineBase::Type type);

private:
    SearchInFileEngineFactory();
    SearchInFileEngineFactory(SearchInFileEngineFactory&);
    void operator=(SearchInFileEngineFactory&);
};
