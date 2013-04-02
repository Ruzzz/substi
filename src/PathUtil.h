//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#pragma once


class PathUtil
{
public:

#ifdef _WIN32
    static const char PATH_SEPARATOR = '\\';
#else  // OTHERS OS
    static const char PATH_SEPARATOR = '/';
#endif  // _WIN32

    template<typename TString, typename TChar>
    static void removeFileNameInPlace(TString &fullFileName, const TChar separator)
    {
        if (!fullFileName.empty())
        {
            TString::size_type pos = fullFileName.find_last_of(separator);
            if (TString::npos != pos)
                fullFileName.erase(pos);
            else
                fullFileName.clear();
        }
    }

    template<typename TString>
    static void removeFileNameInPlace(TString &fullFileName)
    {
        removeFileNameInPlace(fullFileName, TString::value_type(PATH_SEPARATOR));
    }

    template<typename TString, typename TChar>
    static bool hasLastPathSeparator(const TString &path, const TChar separator)
    {
        return !path.empty() && path[path.size() - 1] == separator;
    }

    template<typename TString>
    static void hasLastPathSeparator(const TString &path)
    {
        return hasLastPathSeparator(path, TString::value_type(PATH_SEPARATOR));
    }

    template<typename TString, typename TChar>
    static void addLastPathSeparatorInPlace(TString &path, const TChar separator)
    {
        if (!hasLastPathSeparator(path, separator))
            path.append(1, separator);
    }

    template<typename TString>
    static void addLastPathSeparatorInPlace(TString &path)
    {
        addLastPathSeparatorInPlace(path, TString::value_type(PATH_SEPARATOR));
    }

private:
    PathUtil();
    PathUtil(PathUtil&);
    void operator=(PathUtil&);
};