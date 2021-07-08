#include <string>
#include <list>
#include <vector>
#include <dirent.h>

using namespace std;

enum result
{
    FILES = 0,
    FOLDERS,
    SYMLINK,
    FOLLOW_HIDDEN,
    RESULTS_END
};

struct SPEC_FILE
{
    char *name;
    ino_t size;
    char *path;

    SPEC_FILE(char *pName, ino_t pSize, char *pPath = nullptr)
    {
        name = pName;
        size = pSize;
        path = pPath;
    }
};

list<SPEC_FILE> readDir(DIR *pdir, bool followSym = false);

vector<string*> *readArgs(int argc, char *argv[]);
