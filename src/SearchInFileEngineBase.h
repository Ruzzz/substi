//
// Project: Substitute
// Date:    2013-04-02
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#pragma once


#include <istream>
#include <ostream>
#include <vector>


class SearchInFileEngineBase
{
public:
    typedef std::vector<char> Bytes;

    enum Type
    {
        KNUTH_MORRIS_PRATT,
        ONE_BYTE
    };

    struct Data
    {
        std::istream *sourceFile;
        std::ostream *destFile;
        Bytes *oldBytes;
        Bytes *newBytes;
    };

    SearchInFileEngineBase() : copyBuffer_(COPY_BUUFER_SIZE), numFound_(0), previusPos_(0), eof_(true) { }
    virtual ~SearchInFileEngineBase() {};
    
    void initialize(Data &data);
    virtual void search() = 0;

    const unsigned int numFound() const { return numFound_; }
    const bool eof() const { return eof_; }

protected:
    virtual void initialize_() {};
    void copyFromCurrent(std::streamoff numBytes);
    void writeNewBytes();
    
private:
    SearchInFileEngineBase(SearchInFileEngineBase&);
    void operator=(SearchInFileEngineBase&);

    static const unsigned int COPY_BUUFER_SIZE = 64 * 1024;
    Bytes copyBuffer_;

protected:
    Data data_;
    unsigned int numFound_;
    std::streampos previusPos_;
    bool eof_;
};
