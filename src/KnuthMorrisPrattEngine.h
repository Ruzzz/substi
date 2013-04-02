//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#pragma once


#include "SearchInFileEngineBase.h"


class KnuthMorrisPrattEngine : public SearchInFileEngineBase
{
public:
    KnuthMorrisPrattEngine() : SearchInFileEngineBase() { }
    virtual ~KnuthMorrisPrattEngine() { }
    virtual void search();
    typedef std::vector<int> Prefixes;

protected:
    virtual void initialize_();

private:
    KnuthMorrisPrattEngine(KnuthMorrisPrattEngine&);
    void operator=(KnuthMorrisPrattEngine&);
    void writeLast(const Bytes::size_type skipLastBytes = 0);
    Prefixes prefixes_;

#ifndef TESTING
private:
#else
public:
#endif
    static void computePrefixes(const Bytes &pattern, Prefixes &prefixes);
};
