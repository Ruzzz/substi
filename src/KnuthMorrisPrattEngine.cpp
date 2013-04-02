//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#include <streambuf>
#include <cstdio>  // EOF

#include "KnuthMorrisPrattEngine.h"


void KnuthMorrisPrattEngine::search()
{
    if (!eof_)
    {
        Bytes &pattern = *data_.oldBytes;
        Bytes::size_type patternLen = pattern.size();
        Bytes::size_type j = 0;
        std::streambuf *pBuf = data_.sourceFile->rdbuf();
        
        while (true)
        {
            int ch = pBuf->sbumpc();
            if (EOF == ch)
            {
                writeLast();
                break;
            }
            while ((j > 0) && (j >= patternLen || pattern[j] != static_cast<char>(ch)))
                j = prefixes_[j - 1];
            if (pattern[j] == static_cast<char>(ch))
                ++j;
            if (j == patternLen)
            {
                writeLast(patternLen);
                writeNewBytes();

                j = 0;  // text:           000, rule: 00 > 11
                        // found:          [00]0 and 0[00]
                        // invalid result: 1111
                        // valid result :  110
            }
        }
    }

    eof_ = true;
}


void KnuthMorrisPrattEngine::writeLast(const Bytes::size_type skipLastBytes /* = 0 */)
{
    std::streambuf *pBuf = data_.sourceFile->rdbuf();
    std::streampos current = pBuf->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
    std::streamoff offsetToPrevius = current - previusPos_;  // > 0 !!!
    previusPos_ = current;
    std::streamoff numBytes = offsetToPrevius - skipLastBytes;
    if (numBytes > 0)
    {
        pBuf->pubseekoff(-offsetToPrevius, std::ios_base::cur, std::ios_base::in);
        copyFromCurrent(numBytes);
        pBuf->pubseekoff(offsetToPrevius - numBytes, std::ios_base::cur, std::ios_base::in);
    }
}


void KnuthMorrisPrattEngine::initialize_()
{
    Bytes &pattern = *data_.oldBytes;
    computePrefixes(pattern, prefixes_);
}


void KnuthMorrisPrattEngine::computePrefixes(const Bytes &pattern, Prefixes &prefixes)
{
    // TODO Check pattern.size()
    Bytes::size_type len = pattern.size();
    prefixes.resize(len);
    prefixes[0] = 0;
    for (Bytes::size_type j = 0, i = 1; i < len; ++i)
    {
        while (j > 0 && pattern[i] != pattern[j])
            j = prefixes[j - 1];
        if (pattern[i] == pattern[j])
            ++j;
        prefixes[i] = j;
    }
}
