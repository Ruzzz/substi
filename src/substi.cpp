//
// Project: Substitute
// Date:    2013-03-28
// Author:  Ruzzz <ruzzzua[]gmail.com>
//


#if defined(_WIN32) && defined(_DEBUG)
#define _CRTDBG_MAPALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <iostream>
#include <ctime>

#include "defs.h"
#include "Substitute.h"
#include "CppEscapedUtil.h"
#include "FileUtil.h"
#include "PathUtil.h"
#include "Options.h"


const char USAGE[] =
{
    "Substitute v"VERSION" by Ruzzz\n"
    "Substitute string in file.\n"
    "Usage: substi [options] filename old-string [new-string]\n"
    "\n"
    "Options:\n"
    "  -remove                  - Remove old-string from file.\n"
    "  -nobackup|-rewrite       - Don't create backup file <filename.original>\n"
    "                             or rewrite backup file if exists.\n"
    "\n"
    "Use only ANSI characters and/or c/c++ escape sequences.\n"
    "Supported: \\n \\t \\v \\b \\r \\f \\a \\\\ \\? \\' \\\" \\xH \\xHH \\0.\n"
    "\n"
    "Examples:\n"
    "  substi file.txt hi hello\n"
    "  substi file.txt \"General Public License\" GPL\n"
    "  substi -rewrite file.txt \\n \\r\\n\n"
    "  substi -nobackup file.txt \\r\\n \\n\n"
    "  substi -remove file.txt \\0\n"
    
};


class AutoDeleteFileHelper
{
public:
    AutoDeleteFileHelper(const tstring *fileName) :
        fileName_(fileName)
    {
    }

    ~AutoDeleteFileHelper()
    {
        if (fileName_ && !fileName_->empty() && FileUtil::existsFile(fileName_->c_str()))
            FileUtil::deleteFile(fileName_->c_str());
    }

private:
    AutoDeleteFileHelper();
    const tstring *fileName_;
};


int _tmain(const int argc, const tchar *argv[])
{
    if (argc == 1)
    {
        std::cout << USAGE;
        return EXIT_FAILURE;
    }
    else
    {
        Substi::Options options(argc, argv);
        if (!options.isValid())
            MAIN_ABORT(options.getLastError().toString())
        else if (!FileUtil::existsFile(options.fileName()))
            MAIN_ABORT("File not exists")

        else
        {
            // TODO If size of oldString == size of newString
            //   and set -writeDirect then write direct.
            //   Dev: If not use -nobackup then copy file, not move.
            tstring tempFileName;
            if (options.noBackup())
            {
                tstring path(options.fileName());
                PathUtil::removeFileNameInPlace(path);
                PathUtil::addLastPathSeparatorInPlace(path);
                if (!FileUtil::getTemporaryFileName(path, tempFileName))
                    MAIN_ABORT("Can not get temporary filename from system")
            }
            else
            {
                tempFileName.assign(options.fileName());
                tempFileName += _T(".original");
            }

            // if use temporary file then force rewrite 
            if (!FileUtil::moveFile(options.fileName(), tempFileName.c_str(),
                options.noBackup() || options.rewrite()))
            {
                if (options.noBackup())
                    PRINT_ERROR("Can not create temporary file")
                else if (!options.rewrite() && FileUtil::existsFile(tempFileName.c_str()))
                    PRINT_ERROR("Backup file exists, use -rewrite")
                else
                    PRINT_ERROR("Can not create backup file")
                return EXIT_FAILURE;
            }
            else
            {
                AutoDeleteFileHelper autoDeleteTemp(options.noBackup() ? &tempFileName : nullptr);
                Substitute::Substituter s;
                clock_t startTime = clock();
                if (!s.process(tempFileName.c_str(), options.fileName(), options.oldString(), options.newString()))
                {
                    // If error then  restore file
                    FileUtil::moveFile(tempFileName.c_str(), options.fileName(), true);
                    MAIN_ABORT(s.getLastError().toString())
                }
                else
                {
                    // If not found then restore file
                    if (s.numFound() == 0)
                        FileUtil::moveFile(tempFileName.c_str(), options.fileName(), true);
                    // Found 5300 occurrences in 3 seconds
                    clock_t totalTime = clock() - startTime;
                    double seconds = static_cast<double>(totalTime) / CLOCKS_PER_SEC;
                    std::cout << "Found " << s.numFound() << " occurrences in " << seconds << " seconds.\n";
                }
            }
        }
    }

#if defined(_WIN32) && defined(_DEBUG)
    _CrtDumpMemoryLeaks();
#endif
    return EXIT_SUCCESS;
}
