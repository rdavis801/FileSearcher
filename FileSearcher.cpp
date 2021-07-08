#include "FileSearcher.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// 0 args = search & categorize the whole hard drive
// file(s) = search for given files in whole hard drive
// folder(s) = search & categorize all folders
// file(s) + folder(s) = search given folder for files
// -s to follow sym links
// -all to include hidden files in the search
int main(int argc, char *argv[])
{
    try 
    {  
        DIR *pdir = NULL;
        struct dirent *pent = NULL;

        bool followSym = false;
        bool followHidden = false;

        vector<string*> * arg = readArgs(argc, argv);

        pdir = opendir(".");

        if (pdir == nullptr)
        {
            exit(1);
        }
        
        for(int i = 0; i < arg->size(); i++)
        {
            if(arg->at(SYMLINK) != nullptr)
            {
                followSym = true;
            }
            else if (arg->at(FOLLOW_HIDDEN) != nullptr)
            {
                followHidden = true;
            }
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

        //cout << "\n\n" << pent->d_name << " " << pent->d_type << "\n\n";
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
                    cout << "\n\n" << pent->d_name << "\n\n";
                    exit(0);
                    dirParents.push_front(opendir(pent->d_name));
                    break;
                }
            // Assumed to be a typical file
            default:
                cout << "\ndefault\n";
                char *pathBuffer = getcwd(NULL, 0);
                SPEC_FILE f(pent->d_name, pent->d_ino, pathBuffer);
                filesList.push_back(f);
                break;
        }        
    }
    cout << endl << endl;
    return filesList;

}

vector<string*> *readArgs(int argc, char *argv[])
{
    vector<string*> * pRead = new vector<string*>(RESULTS_END);

    for(int i = 1; i < argc && i < 100; i++)
    {
        if(strcmp(argv[i], "-s") == 0)
        {
            auto it = pRead->begin() + SYMLINK;
            pRead->insert(it, new string);
        }
        else if (strcmp(argv[i], "-all"))
        {
            auto it = pRead->begin() + FOLLOW_HIDDEN;
            pRead->insert(it, new string);
        }
    }
    return pRead;
}