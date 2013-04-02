//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#include <fstream>
#include <iterator>
#include <algorithm>

#include "Substitute.h"
#include "CppEscapedUtil.h"
#include "SearchInFileEngineFactory.h"


namespace Substitute {


//
//  Error
//


const char *Substitute::Error::toString() const
{
    switch (code_)
    {
        case OK:                            return "OK";

        case CANNOT_OPEN_FILE:              return "Cannot open file";
        case CANNOT_WRITE_FILE:             return "Error writing to file";
        case CANNOT_READ_FILE:              return "Error reading file";

        case STRINGS_ARE_EQUAL:             return "Strings are equal";
        case INVALID_ANSI_IN_OLD_STRING:    return "Invalid ANSI characters in old-string";
        case INVALID_ANSI_IN_NEW_STRING:    return "Invalid ANSI characters in new-string";
        case INVALID_SPECIAL_IN_OLD_STRING: return "Invalid escape sequences in old-string";
        case INVALID_SPECIAL_IN_NEW_STRING: return "Invalid escape sequences in new-string";

        case INTERNAL_1:                    return "Internal 1";
        default:                            return "Unknown error";
    }
}


bool operator==(const Error &lhs, const Error &rhs)
{
    return lhs.code() == rhs.code();
}


bool operator==(const Error &lhs, const Error::Code &rhs)
{
    return lhs.code() == rhs;
}


bool operator==(const Error::Code &lhs, const Error &rhs)
{
    return lhs == rhs.code();
}


//
//  Substituter
//


bool Substituter::process(const tchar *sourceFileName, const tchar *destFileName,
                          const tchar *oldString, const tchar *newString)
{
    Bytes oldBytes;
    Bytes newBytes;
    if (prepareStrings_(oldString, newString, oldBytes, newBytes))
    {
        std::ifstream sourceFile(sourceFileName, std::ios::in | std::ios::binary | std::ios::ate);
        if (sourceFile)
        {
            const auto size = sourceFile.tellg();
            if (size < oldBytes.size())  // TODO Its not error, simple not found?
                return true;
            sourceFile.seekg(0);

            std::fstream destFile(destFileName, std::ios::out | std::ios::trunc | std::ios::binary);
            if (destFile)
            {
                return process_(sourceFile, destFile, oldBytes, newBytes);
            }
            else
                lastError_ = Error::CANNOT_WRITE_FILE;
        }
        else
            lastError_ = Error::CANNOT_OPEN_FILE;
    }
    return false;
}


bool Substituter::process_(std::istream &sourceFile, std::ostream &destFile,
                           Bytes &oldBytes, Bytes &newBytes)
{
    // init
    std::ios::iostate sourcePrevState = sourceFile.exceptions();
    std::ios::iostate destPrevState = destFile.exceptions();
    sourceFile.exceptions(std::ios::badbit | std::ios::failbit);
    destFile.exceptions(std::ios::badbit | std::ios::failbit);
    lastError_ = Error::OK;

    // work
    // TODO Use simple search for small strings
    try
    {
        SearchInFileEngineBase::Type enginType;
        if (oldBytes.size() == 1)
            enginType = SearchInFileEngineBase::Type::ONE_BYTE;
        else
            enginType = SearchInFileEngineBase::Type::KNUTH_MORRIS_PRATT;

        std::auto_ptr<SearchInFileEngineBase> engine(SearchInFileEngineFactory::get(enginType));
        SearchInFileEngineBase::Data data = { &sourceFile, &destFile, &oldBytes, &newBytes };
        engine->initialize(data);
        engine->search();
        founded_ = engine->numFound();
    }
    catch (std::ios::failure /*&e*/)
    {
        if (!sourceFile)
            lastError_ = Error::CANNOT_READ_FILE;
        else
            lastError_ = Error::CANNOT_WRITE_FILE;
    }

    sourceFile.exceptions(sourcePrevState);
    destFile.exceptions(destPrevState);
    return lastError_ == Error::OK;
}


bool Substituter::prepareStrings_(const tchar *oldString, const tchar *newString,
                                  Bytes &oldBytes, Bytes &newBytes)
{
    if (!oldString)
    {
        lastError_ = Error::INTERNAL_1;
        return false;
    }

    if (newString && _tcscmp(oldString, newString) == 0)
    {
        lastError_ = Error::STRINGS_ARE_EQUAL;
        return false;
    }

    if (!StringUtil::prepareAnsi(oldString, oldBytes))
    {
        lastError_ = Error::INVALID_ANSI_IN_OLD_STRING;
        return false;
    }

    if (!CppEscapedUtil::decodeInPlace(oldBytes, 2))
    {
        lastError_ = Error::INVALID_SPECIAL_IN_OLD_STRING;
        return false;
    }

    if (newString)
    {
        if (!StringUtil::prepareAnsi(newString, newBytes))
        {
            lastError_ = Error::INVALID_ANSI_IN_NEW_STRING;
            return false;
        }

        if (!CppEscapedUtil::decodeInPlace(newBytes, 2))
        {
            lastError_ = Error::INVALID_SPECIAL_IN_NEW_STRING;
            return false;
        }
    }

    return true;
}


}  // namespace Substitute
