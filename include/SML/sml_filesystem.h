#ifndef INCLUDE_SML_FILESYSTEM_H
#define INCLUDE_SML_FILESYSTEM_H

/*
* All platforms
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SML_FS_MAX_NAME_LEN 256
#define SML_FS_STRLEN_LITERAL(s) (sizeof(s) / sizeof(s[0]) - 1)

/*
 * Windows?
 */
#if defined(_MSC_VER) || defined(__MINGW32__)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

/* define bool */
#define bool int
#define true  1
#define false 0

/*
 * NOTE: to use windows unicode paths macros UNICODE and _UNICODE
 * must be defined globally
 */
#if defined(SML_FS_UNICODE) || defined(UNICODE) || defined(_UNICODE)

#if !defined(SML_FS_UNICODE)
#define SML_FS_UNICODE
#endif

#if !defined(UNICODE)
#define UNICODE
#endif

#if !defined(_UNICODE)
#define _UNICODE
#endif

#endif /* UNICODE */

#include <tchar.h>

#define SML_FS_CHAR TCHAR
#define SML_FS_STR(s) _TEXT(s)
#define SML_FS_STRLEN(s) _tcslen(s)
#define SML_FS_STRCPY(dst, src) _tcscpy(dst, src)
#define SML_FS_STRNCPY(dst, src, cnt) _tcsncpy(dst, src, cnt)
#define SML_FS_PATH_CAT_LITERAL(path, literal) SML_FS_pathCatStrN(path, SML_FS_STR(literal), SML_FS_STRLEN_LITERAL(SML_FS_STR(literal)))
#define SML_FS_SEP SML_FS_STR('\\')
#define SML_FS_SEP_ALT SML_FS_STR('/')

#define SML_FS_MAX_PATH_LEN MAX_PATH

#if defined(_MSC_VER)

/* extra length needed to append "\\*" for windows find file functions */
#define SML_FS_PATH_EXTRA_LEN 2

#else /* must be MINGW */

#define SML_FS_PATH_EXTRA_LEN 0
#define SML_FS_GETCWD  _tgetcwd

/* on mingw we use dir/dirent */
#include <dirent.h>
#include <stddef.h>
#include <libgen.h>
#include <sys/stat.h>

#if defined(SML_FS_UNICODE)
    #define SML_FS_READDIR  _wreaddir // TODO: why not use _txxxdir??
    #define SML_FS_OPENDIR  _wopendir
    #define SML_FS_CLOSEDIR _wclosedir

    typedef _WDIR SML_FS_DIR;
    typedef struct _wdirent SML_FS_DIRENT;
#else
    #define SML_FS_READDIR  readdir
    #define SML_FS_OPENDIR  opendir
    #define SML_FS_CLOSEDIR closedir

    typedef DIR SML_FS_DIR;
    typedef struct dirent SML_FS_DIRENT; 
#endif
#endif /* defined(_MSC_VER) */


/*
* Unix/apple?
*/
#elif defined(__APPLE__) || defined(unix) || defined(__unix) || defined(__unix__)

#include <dirent.h>
#include <stddef.h>
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

/*
 * NOTE: This has to appear before defined(BSD)
 * since sys/param.h defines the BSD environment
 */
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/param.h>
#endif

#if defined (__linux__) || defined(BSD)
#include <limits.h>
#endif

#define SML_FS_CHAR char
#define SML_FS_STR(s) s
#define SML_FS_STRLEN(s) strlen(s)
#define SML_FS_STRCPY(dst, src) strcpy(dst, src)
#define SML_FS_STRNCPY(dst, src, cnt) strncpy(dst, src, cnt)

#define SML_FS_PATH_CAT_LITERAL(path, literal) SML_FS_pathCatStrN(path, SML_FS_STR(literal), SML_FS_STRLEN_LITERAL(SML_FS_STR(literal)))
#define SML_FS_SEP SML_FS_STR('/')
#define SML_FS_SEP_ALT SML_FS_STR('\\')

/* PATH_MAX might not be defined on every system */
#ifdef PATH_MAX
#define SML_FS_MAX_PATH_LEN PATH_MAX
#else
#define SML_FS_MAX_PATH_LEN (4096)
#endif

#define SML_FS_PATH_EXTRA_LEN 0

#define SML_FS_GETCWD   getcwd
#define SML_FS_READDIR  readdir
#define SML_FS_OPENDIR  opendir
#define SML_FS_CLOSEDIR closedir

/* alias for dir/dirent */
typedef DIR SML_FS_DIR;
typedef struct dirent SML_FS_DIRENT; 


/*
* Unknown platform
*/
#else
#error "Unknown platform"
#endif


typedef struct SML_FS_Path {
    SML_FS_CHAR buf[SML_FS_MAX_PATH_LEN];  /**< internal null terminated buffer */
    unsigned int len;                      /**< length without null termination */
} SML_FS_Path;

typedef struct SML_FS_File {
    SML_FS_CHAR* extension;                /**< pointer to extension, points to name internally */
    SML_FS_Path path;                      /**< complete file path */
    SML_FS_CHAR name[SML_FS_MAX_NAME_LEN]; /**< buffer containing file name */

#if !defined(_MSC_VER)
    bool statLoaded;                       /**< status loaded flag */
#if defined(__MINGW32__)
    struct _stat stat;                     /**< status struct */
#else
    struct stat stat;                      /**< status struct */
#endif
#endif
    bool isDir;                            /**< true if it is a directory */
    bool isReg;                            /**< true if it is a regular file */
    bool isSym;                            /**< true if it is a symbolic link */
} SML_FS_File;

typedef struct SML_FS_Dir {
    SML_FS_Path path;                      /**< path of the directory */
    SML_FS_File *files;                    /**< pointer to files array - currently unused */
    unsigned int fileCount;                /**< size of files array */

#if defined(_MSC_VER)
    HANDLE handle;                         /**< directory stream handle */
    WIN32_FIND_DATA findData;              /**< find data */
#else
    SML_FS_DIR *d;                         /**< posix directory stream */
    SML_FS_DIRENT *e;                      /**< posix directory entry */
#endif

    bool hasNext;                          /**< true if d has another entry */
} SML_FS_Dir;

bool SML_FS_pathCatPath(SML_FS_Path *dst, const SML_FS_Path *src);
bool SML_FS_pathCatStr(SML_FS_Path *path, const SML_FS_CHAR *str);
bool SML_FS_pathCatStrN(SML_FS_Path *path, const SML_FS_CHAR *str, unsigned int strLen);
void SML_FS_pathTrim(SML_FS_Path *path);
unsigned int SML_FS_pathFindLastSep(SML_FS_Path *path);
void SML_FS_pathToParent(SML_FS_Path *path);
bool SML_FS_pathFromUtf8N(SML_FS_Path *path, const char *str, unsigned int strLen);
void SML_FS_strToPath(SML_FS_Path *dst, const SML_FS_CHAR *src, unsigned int srcLen);
unsigned int SML_FS_strToPathStr(SML_FS_CHAR *dst, const SML_FS_CHAR *src, unsigned int srcLen);

void SML_FS_getCwd(SML_FS_Path *path);
bool SML_FS_openDir(SML_FS_Dir *dir, SML_FS_Path *path);
bool SML_FS_peekNextFile(SML_FS_Dir *dir, SML_FS_File *file);
bool SML_FS_getNextFile(SML_FS_Dir *dir, SML_FS_File *file);
void SML_FS_advanceFile(SML_FS_Dir *dir);
void SML_FS_closeDir(SML_FS_Dir *dir);
bool SML_FS_exists(SML_FS_Path *path);


static inline bool SML_FS_pathFromUtf8(SML_FS_Path *path, const char *str)
{
    return SML_FS_pathFromUtf8N(path, str, strlen(str));
}

#ifdef __cplusplus
}
#endif




#endif /* INCLUDE_SML_FILESYSTEM_H */
