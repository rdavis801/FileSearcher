#include "FileSearcher.h"
#include <iostream>
#include <unistd.h>

// 0 args = search & categorize the whole hard drive
// file(s) = search for given files in whole hard drive
// folder(s) = search & categorize all folders
// file(s) + folder(s) = search given folder for files
// -s to follow sym links
int main(int argc, char *argv[])
{
    try {
    DIR *pdir = NULL;
    struct dirent *pent = NULL;
  /* TODO: add additional file checks
    // No args
    // if(argc == 1)
    // {
    //     pdir = opendir("/");
    // }*/

    pdir = opendir(".");

    if (pdir == nullptr)
    {
        exit(1);
    }
    
    for(SPEC_FILE f : readDir(pdir))
    {
        cout << f.name << " with size of " << f.size << " found at " << f.path << endl;
    }
    }
    catch (exception e)
    {
        cout << e.what() << endl;
    }

    /*while(pent = readdir(pdir))
    {
        cout << pent->d_name << " " << pent->d_ino << endl;
    }*/
}

list<SPEC_FILE> readDir(DIR *pdir, bool followSym)
{
    long long       count = 0;
    struct dirent  *pent  = nullptr;
    list<SPEC_FILE> filesList;
    list<DIR *>     dirParents;

    dirParents.push_front(pdir);

    // While there is something to read from pdir
    while(dirParents.size() > 0)
    {
        pent = readdir(dirParents.front());
        
        if(pent == nullptr)
        {
            dirParents.pop_front();
            chdir("..");
            continue;
        }
        // Ignore the '.' and '..' directories
        if(pent->d_name[0] == '.' or pent->d_name == "..")
            continue;

        // Determine the type
        switch(pent->d_type)
        {
            // Type is a Directory
            case DT_DIR:
                // Follow the sub-directory
                dirParents.push_front(opendir(pent->d_name));
                break;
            // Type is a Sym Link
            case DT_LNK:
                // Only follow the link if requested
                if(followSym)
                {
                    dirParents.push_front(opendir(pent->d_name));
                    break;
                }
            // Assumed to be a typical file
            default:
                char *pathBuffer = getcwd(NULL, 0);
                SPEC_FILE f(pent->d_name, pent->d_ino, pathBuffer);
                filesList.push_back(f);
                break;
        }        
    }
    cout << endl << endl;
    return filesList;

}

list<SPEC_FILE> getLocation(int argc, char *argv[])
{
    list<SPEC_FILE> filesList;
    for(int i = 1; i < argc; i++)
    {
        
    }

    return filesList;
}