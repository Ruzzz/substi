//
// Project: Substitute
// Date:    2013-04-02
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#include "SearchInFileEngineBase.h"


void SearchInFileEngineBase::initialize(Data &data)
{
    data_.sourceFile = data.sourceFile;
    data_.destFile = data.destFile;
    data_.oldBytes = data.oldBytes;
    data_.newBytes = data.newBytes;
    numFound_ = 0;
    previusPos_ = 0;
    eof_ = false;
    initialize_();
}


void SearchInFileEngineBase::copyFromCurrent(const std::streamoff numBytes)
    /// Note: Moves pointer to +numBytes
{
    std::streambuf *pSourceBuf = data_.sourceFile->rdbuf();
    std::streambuf *pDestBuf = data_.destFile->rdbuf();
    Bytes::size_type BufferSize = copyBuffer_.size();
    std::streamoff bytes_ = numBytes;

    // TODO Errors
    while (bytes_ > 0)
    {
        std::streamsize size = bytes_ < BufferSize ? bytes_ : BufferSize;
        auto readed = pSourceBuf->sgetn(&copyBuffer_[0], size);
        auto writed = pDestBuf->sputn(&copyBuffer_[0], size);
        bytes_ -= readed;
    }
}


void SearchInFileEngineBase::writeNewBytes()
{
    ++numFound_;
    if (!data_.newBytes->size())
        return;
    // TODO Errors
    std::streambuf *pDestBuf = data_.destFile->rdbuf();
    auto writed = pDestBuf->sputn(&(*data_.newBytes)[0], data_.newBytes->size());
}
