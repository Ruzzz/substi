//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#pragma once


#include <string>

#include "defs.h"
#include "StringUtil.h"


namespace Substi {


class Error
{
public:
    friend class Options;

    enum Code
    {
        OK,

        WRONG_ARGUMENTS_NUMBER,
        INVALID_ARGUMENT
    };

    Error(Code code = OK) : code_(code) {}
    Error(const Error &other) : code_(other.code_) {}
    const Error& operator=(const Error &other) { code_ = other.code_; return *this; }
    const Error& operator=(const Code &code) { code_ = code; return *this; }

    const Code code() const { return code_; }
    const char *toString() const;

private:

    void setInvalidArgument(const tchar *invalidArgument)
    {
        code_ = INVALID_ARGUMENT;

        invalidArgumentError_.assign("Invalid option");
        std::string argument;
        if (StringUtil::prepareAnsi(invalidArgument, argument))
        {
            invalidArgumentError_ += ": ";
            invalidArgumentError_ += argument;
        }
    }

    Code code_;
    std::string invalidArgumentError_;
};


bool operator==(const Error &lhs, const Error &rhs);
bool operator==(const Error &lhs, const Error::Code &rhs);
bool operator==(const Error::Code &lhs, const Error &rhs);


class Options
{
public:
    Options(const int argc, const tchar *argv[]) :
        fileName_(nullptr),
        oldString_(nullptr),
        newString_(nullptr),
        noBackup_(false),
        rewrite_(false)
    {
        parse_(argc, argv);
    }

    bool isValid() const { return lastError_ == Error::OK; }
    const Error &getLastError() const { return lastError_; }

    tchar *fileName()  const { return fileName_; };
    tchar *oldString() const { return oldString_; };
    tchar *newString() const { return newString_; };
    bool noBackup()    const { return noBackup_; };
    bool rewrite()     const { return rewrite_; };

private:
    Options();

    void parse_(const int argc, const tchar *argv[])
        // If all options is valid and valid count of arguments then return true
    {
        bool remove = false;
        int i = 1;
        while (i < argc && '-' == *argv[i])
        {
            if (_tcscmp(argv[i], _T("-rewrite")) == 0)
                rewrite_ = true;
            else if (_tcscmp(argv[i], _T("-nobackup")) == 0)
                noBackup_ = true;
            else if (_tcscmp(argv[i], _T("-remove")) == 0)
                remove = true;
            else
            {
                lastError_.setInvalidArgument(argv[i]);
                return;
            }
            ++i;
        }

        int numValidArg = remove ? 2 : 3;
        if (argc != i + numValidArg)  // filename old-string new-string
        {
            lastError_ = Error::WRONG_ARGUMENTS_NUMBER;
            return;
        }

        fileName_ = const_cast<tchar *>(argv[i]);
        oldString_ = const_cast<tchar *>(argv[i + 1]);
        if (!remove)
            newString_ = const_cast<tchar *>(argv[i + 2]);
    }

    Error lastError_;

    tchar *fileName_;
    tchar *oldString_;
    tchar *newString_;
    bool noBackup_;
    bool rewrite_;
};


}  // namespace Substi
