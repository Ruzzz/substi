//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#pragma once


#include <vector>

#include "defs.h"
#include "StringUtil.h"


namespace Substitute {


typedef std::vector<char> Bytes;


class Error
{
public:
    enum Code
    {
        OK,

        CANNOT_OPEN_FILE,
        CANNOT_WRITE_FILE,
        CANNOT_READ_FILE,

        STRINGS_ARE_EQUAL,
        INVALID_ANSI_IN_OLD_STRING,
        INVALID_ANSI_IN_NEW_STRING,
        INVALID_SPECIAL_IN_OLD_STRING,
        INVALID_SPECIAL_IN_NEW_STRING,

        INTERNAL_1
    };

    Error(Code code = OK) : code_(code) {}
    Error(const Error &other) : code_(other.code_) {}
    const Error& operator=(const Error &other) { code_ = other.code_; return *this; }
    const Error& operator=(const Code &code) { code_ = code; return *this; }

    const Code code() const { return code_; }
    const char *toString() const;

private:
    Code code_;
};


bool operator==(const Error &lhs, const Error &rhs);
bool operator==(const Error &lhs, const Error::Code &rhs);
bool operator==(const Error::Code &lhs, const Error &rhs);


class Substituter
{
public:
    Substituter() : founded_(0) {}
    const unsigned int numFound() const { return founded_; }
    bool isValid() const { return lastError_ == Error::OK; }
    const Error &getLastError() const { return lastError_; }

    bool process(const tchar *sourceFileName, const tchar *destFileName,
                 const tchar *oldString, const tchar *newString);

#ifndef TESTING
private:
#endif
    Substituter(Substituter&);
    void operator=(Substituter&);

    bool process_(std::istream &sourceFile, std::ostream &destFileName,
                  Bytes &oldBytes, Bytes &newBytes);
    bool prepareStrings_(const tchar *oldString, const tchar *newString,
                         Bytes &oldBytes, Bytes &newBytes);

    unsigned int founded_;
    Error lastError_;
};


}  // namespace Substitute
