//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#pragma once


#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#else
#include <fstream>
#endif

#include "defs.h"


class FileUtil
{
public:

#ifdef _WIN32

    static bool copyFile(const char *from, const char *to, bool rewrite = false)
    {
        return ::CopyFileA(from, to, rewrite ? FALSE : TRUE) != FALSE;
    }

    static bool copyFile(const wchar_t *from, const wchar_t *to, bool rewrite = false)
    {
        return ::CopyFileW(from, to, rewrite ? FALSE : TRUE) != FALSE;
    }

    static bool moveFile(const char *from, const char *to, bool rewrite = false)
    {
        DWORD flags = MOVEFILE_WRITE_THROUGH;
        if (rewrite)
            flags |= MOVEFILE_REPLACE_EXISTING;
        return ::MoveFileExA(from, to, flags) != FALSE;
    }

    static bool moveFile(const wchar_t *from, const wchar_t *to, bool rewrite = false)
    {
        DWORD flags = MOVEFILE_WRITE_THROUGH;
        if (rewrite)
            flags |= MOVEFILE_REPLACE_EXISTING;
        return ::MoveFileExW(from, to, flags) != FALSE;
    }

    static bool deleteFile(const char *filename)
    {
        return ::DeleteFileA(filename) != FALSE;
    }

    static bool deleteFile(const wchar_t *filename)
    {
        return ::DeleteFileW(filename) != FALSE;
    }

    static bool existsFile(const char *fileName)
    {
        return ::GetFileAttributesA(fileName) != DWORD(-1);
    }

    static bool existsFile(const wchar_t *fileName)
    {
        return ::GetFileAttributesW(fileName) != DWORD(-1);
    }

    static bool getTemporaryFileName(const std::string &path, std::string &result, const char *prefix = "su_")
        /// Note: Len of path must be <= MAX_PATH - 14, see WinApi Docs about GetTempFileName
    {
        char fileName[MAX_PATH];
        if (::GetTempFileNameA(path.c_str(), prefix, 0, fileName) == 0)
            return false;

        result.assign(fileName);
        return true;
    }

    static bool getTemporaryFileName(const std::wstring &path, std::wstring &result, const wchar_t *prefix = L"su_")
        /// Note: Len of path must be <= MAX_PATH - 14, see WinApi Docs about GetTempFileName
    {
        wchar_t fileName[MAX_PATH];
        if (::GetTempFileNameW(path.c_str(), prefix, 0, fileName) == 0)
            return false;

        result.assign(fileName);
        return true;
    }

#else  // OTHERS OS

    static const char PATH_SEPARATOR = '/';

    // TODO std::rename()
    // TODO std::remove()

    template<typename TChar>
    static bool existsFile(const TChar *fileName)
    {
        return std::ifstream(fileName) != nullptr;
    }

#endif  // _WIN32

private:
    FileUtil();
    FileUtil(FileUtil&);
    void operator=(FileUtil&);
};
