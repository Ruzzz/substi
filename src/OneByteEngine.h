//
// Project: Substitute
// Date:    2013-04-02
// Author:  Ruzzz <ruzzzua[]gmail.com>
//

#pragma once


#include <iterator>

#include "SearchInFileEngineBase.h"


class OneByteEngine : public SearchInFileEngineBase
{
public:
    OneByteEngine() : SearchInFileEngineBase() { }
    virtual ~OneByteEngine() { }

    virtual void search()
    {
        if (!eof_)
        {
            if (!data_.newBytes->size())
                removeByte();
            else if (data_.newBytes->size() == 1)
                replaceByteByByte();
            else
                replaceByteByText();
        }
        eof_ = true;
    }

private:
    OneByteEngine(OneByteEngine&);
    void operator=(OneByteEngine&);

    void replaceByteByText()
    {
        std::istreambuf_iterator<char> sourceIt(data_.sourceFile->rdbuf()), end;
        std::ostreambuf_iterator<char> destIt(data_.destFile->rdbuf());
        char pattern = (*data_.oldBytes)[0],
             newValue = (*data_.newBytes)[0];

        for (;sourceIt != end; ++sourceIt, ++destIt)
        {
            if (*sourceIt == pattern)
            {
                // writeNewBytes();
                for (Bytes::size_type i = 0, l = data_.newBytes->size(); i < l; ++i)
                    *destIt++ = (*data_.newBytes)[i];
                ++numFound_;
            }
            else
                *destIt = *sourceIt;
        }
    }

    void replaceByteByByte()
    {
        std::istreambuf_iterator<char> sourceIt(data_.sourceFile->rdbuf()), end;
        std::ostreambuf_iterator<char> destIt(data_.destFile->rdbuf());
        char pattern = (*data_.oldBytes)[0],
             newValue = (*data_.newBytes)[0];

        for (;sourceIt != end; ++sourceIt, ++destIt)
        {
            if (*sourceIt == pattern)
            {
                *destIt = newValue;
                ++numFound_;
            }
            else
                *destIt = *sourceIt;
        }
    }

    void removeByte()
    {
        std::istreambuf_iterator<char> sourceIt(data_.sourceFile->rdbuf()), end;
        std::ostreambuf_iterator<char> destIt(data_.destFile->rdbuf());
        char pattern = (*data_.oldBytes)[0];

        for (;sourceIt != end; ++sourceIt)
        {
            if (*sourceIt != pattern)
            {
                *destIt = *sourceIt;
                ++destIt;
            }
            else
                ++numFound_;
        }
    }
};
