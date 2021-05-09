#include <string>
#include <list>
#include <dirent.h>

using namespace std;

enum result
{
    NO_ARGS,
    FILES,
    FOLDERS,
    FILES_FOLDERS,
    SYMLINK
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
list<SPEC_FILE> getLocation(int, char *[]);
