#include "SML/sml_filesystem.h"


static inline void SML_FS_Dir_init(SML_FS_Dir *dir, SML_FS_Path *path);
static bool        SML_FS_Dir_open(SML_FS_Dir *me);
static inline void SML_FS_Dir_close(SML_FS_Dir *me);
static inline bool SML_FS_isSep(SML_FS_CHAR c);
static inline void SML_FS_pathCopy(SML_FS_Path *dst, SML_FS_Path *src);
static bool        SML_FS_fileFlags(SML_FS_Dir *dir, SML_FS_File *file);

/* only for posix API */
#if !defined(_MSC_VER)
static bool        SML_FS_File_loadStat(SML_FS_File *me);
#endif

bool SML_FS_pathCatPath(SML_FS_Path *dst, const SML_FS_Path *src)
{
    return SML_FS_pathCatStrN(dst, src->buf, src->len);
}

bool SML_FS_pathCatStrN(SML_FS_Path *path, const SML_FS_CHAR *str, unsigned int strLen)
{
    /*
     * Check if extra separator is necessary 
     * NOTE: we still check the last element of path although
     * it should already be trimmed if it's a valid path
     */
    unsigned int extraSep = 0;
    if (!SML_FS_isSep(str[0]) && !SML_FS_isSep(path->buf[path->len - 1])) {
        extraSep = 1;
    }

    /* not enough space for total path + null terminator? */
    if (path->len + extraSep + strLen >= SML_FS_MAX_PATH_LEN) {
        errno = ENAMETOOLONG;
        return false;
    }

    /* add extra separator */
    if (extraSep > 0) {
        path->buf[path->len++] = SML_FS_SEP;
        path->buf[path->len] = SML_FS_STR('\0');
    }

    path->len = path->len + SML_FS_strToPathStr(&path->buf[path->len] , str, strLen);
    SML_FS_pathTrim(path);
    return true;
}

bool SML_FS_pathCatStr(SML_FS_Path *path, const SML_FS_CHAR *str)
{
    return SML_FS_pathCatStrN(path, str, SML_FS_STRLEN(str));
}

static inline bool SML_FS_isSep(SML_FS_CHAR c) {
    return c == SML_FS_SEP || c == SML_FS_SEP_ALT;
}

static inline void SML_FS_Dir_init(SML_FS_Dir *me, SML_FS_Path *path)
{
    memset(me, 0, sizeof(*me));

    SML_FS_pathCopy(&me->path, path);

#if defined(_MSC_VER)
    me->handle = INVALID_HANDLE_VALUE;
#endif

    me->fileCount = UINT_MAX;
}

static bool SML_FS_Dir_open(SML_FS_Dir *me)
{
#if defined(_MSC_VER)

    /* temporarily add the wildcard for windows API - extra length is accounted for in SML_FS_openDir() */
    SML_FS_PATH_CAT_LITERAL(&me->path, "\\*");

#if (defined WINAPI_FAMILY) && (WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP)
    me->handle = FindFirstFileEx(me->path.buf, FindExInfoStandard, &me->findData, FindExSearchNameMatch, NULL, 0);
#else
    me->handle = FindFirstFile(me->path.buf, &me->findData);
#endif

    /* remove the wildcard again */
    SML_FS_pathToParent(&me->path);

    if (me->handle == INVALID_HANDLE_VALUE) {
        /* failed to open dir */
        errno = ENOENT;
        SML_FS_Dir_close(me);
        return false;
    }

    me->hasNext = true;

#else
    me->d = SML_FS_OPENDIR(me->path.buf);
    if (!me->d) {
        /* failed to open dir */
        SML_FS_Dir_close(me);
        return false;
    }
    
    me->e = SML_FS_READDIR(me->d);
    if (!me->e) {
        /* dir is empty */
        me->hasNext = false;
    } else {
        me->hasNext = true;
    }
#endif
    return true;
}

static inline void SML_FS_Dir_close(SML_FS_Dir *me)
{
    if (me->files) {
        free(me->files);
    }
    me->files = NULL;
    me->fileCount = UINT_MAX;
    me->hasNext = false;
    
#if defined(_MSC_VER)
    if (me->handle != INVALID_HANDLE_VALUE) {
        FindClose(me->handle);
    }
    me->handle = INVALID_HANDLE_VALUE;
#else
    if (me->d) {
        SML_FS_CLOSEDIR(me->d);
    }
    me->d = NULL;
    me->e = NULL;
#endif

}

static inline void SML_FS_pathCopy(SML_FS_Path *dst, SML_FS_Path *src)
{
    SML_FS_STRNCPY(dst->buf, src->buf, SML_FS_MAX_PATH_LEN);
    dst->len = src->len;
}

void SML_FS_pathTrim(SML_FS_Path *path)
{
    unsigned int len = path->len;
    while(len > 1 && SML_FS_isSep(path->buf[len - 1])) {
        --len;
    }
    path->buf[len] = SML_FS_STR('\0');
    path->len = len;
}

unsigned int SML_FS_pathFindLastSep(SML_FS_Path *path)
{
    unsigned int len = path->len;
    while (len > 1 && !SML_FS_isSep(path->buf[len - 1])) {
        --len;
    }
    return len - 1;
}

void SML_FS_pathToParent(SML_FS_Path *path)
{
    unsigned int sepIdx = SML_FS_pathFindLastSep(path);

#if defined(WIN32)
    /* 
     * On windows skip the trim once we arrive at the root of the drive, e.g. C:\
     * NOTE: this also preserves the root path on subsequent calls to SML_FS_pathToParent()
     */
    if (sepIdx > 0 && path->buf[sepIdx - 1] == SML_FS_STR(':')) {
        path->len = sepIdx + 1;
        path->buf[path->len] = SML_FS_STR('\0');
        return;
    }
#endif
    SML_FS_pathTrim(path);
}


unsigned int SML_FS_strToPathStr(SML_FS_CHAR *dst, const SML_FS_CHAR *src, unsigned int srcLen)
{
    bool prevSep = false;
    SML_FS_CHAR *p = dst;
    for (unsigned int i = 0; i < srcLen + 1; ++i) { /* include null termination */
        bool isSep = SML_FS_isSep(src[i]);
        if (isSep) {
            if (!prevSep) {
                *p++ = SML_FS_SEP;
            }
        } else {
            *p++ = src[i];
        }
#if defined(WIN32)
        /* on windows allow double separators in the beginning of network drive paths */
        prevSep = isSep && i;
#else
        prevSep = isSep;
#endif
    }
    return p - dst - 1;
}

void SML_FS_strToPath(SML_FS_Path *dst, const SML_FS_CHAR *src, unsigned int srcLen)
{
    dst->len = SML_FS_strToPathStr(dst->buf, src, srcLen);
    SML_FS_pathTrim(dst);
}

bool SML_FS_pathFromUtf8N(SML_FS_Path *path, const char *str, unsigned int strLen)
{
    /* empty path? */
    if (strLen == 0) {
        path->len = 1;
        path->buf[0] = SML_FS_STR('.');
        path->buf[1] = SML_FS_STR('\0');
        return false;
    }

    path->len = 0;
    path->buf[0] = SML_FS_STR('\0');
    
    /* too long? */
    if (strLen >= SML_FS_MAX_PATH_LEN) {
        errno = ENAMETOOLONG;
        return false;
    }
    
#if defined(SML_FS_UNICODE)
    size_t len;
    SML_FS_CHAR buf[SML_FS_MAX_PATH_LEN];
    if (mbstowcs_s(&len, buf, strLen + 1, str, SML_FS_MAX_PATH_LEN) != 0 || len == 0) {
        return false;
    }
    /* len includes null character -> subtract 1 */
    SML_FS_strToPath(path, buf, len - 1);

#else
    SML_FS_strToPath(path, str, strLen);
#endif

    return true;
}

void SML_FS_getCwd(SML_FS_Path *path)
{
#if (_MSC_VER)
    path->len = GetCurrentDirectory(SML_FS_MAX_PATH_LEN, path->buf);
#else
    SML_FS_GETCWD(path->buf, SML_FS_MAX_PATH_LEN);
    path->len = SML_FS_STRLEN(path->buf);
#endif
    SML_FS_pathTrim(path);
}

bool SML_FS_openDir(SML_FS_Dir *dir, SML_FS_Path *path)
{
    /* input validation */
    if (path->len == 0) {
        errno = EINVAL;
        return false;
    }
    if (path->len + SML_FS_PATH_EXTRA_LEN >= SML_FS_MAX_PATH_LEN) {
        errno = ENAMETOOLONG;
        return false;
    }

    SML_FS_Dir_init(dir, path);
    return SML_FS_Dir_open(dir);
}

void SML_FS_closeDir(SML_FS_Dir *dir)
{
    SML_FS_Dir_close(dir);
}

bool SML_FS_getNextFile(SML_FS_Dir *dir, SML_FS_File *file)
{
    if (!SML_FS_peekNextFile(dir, file)) {
        return false;
    }
    /* advance the dir stream - no next file is not a reason to return false */
    SML_FS_advanceFile(dir);
    return true;
}

void SML_FS_advanceFile(SML_FS_Dir *dir)
{
    if (!dir->hasNext) {
        return;
    }

#if defined(_MSC_VER)
    if (FindNextFile(dir->handle, &dir->findData) == 0) {
        dir->hasNext = false;
    }
#else
    dir->e = SML_FS_READDIR(dir->d);
    if (!dir->e) {
        dir->hasNext = false;
    }
#endif
}

bool SML_FS_peekNextFile(SML_FS_Dir *dir, SML_FS_File *file)
{
    const SML_FS_CHAR* fileName;
    unsigned int fileNameLen;
    unsigned int filePathLen;

#if defined(_MSC_VER)
    if (!dir->hasNext || !dir->handle) {
        return false;
    }
    fileName = dir->findData.cFileName;
    fileNameLen = SML_FS_STRLEN(dir->findData.cFileName);
    filePathLen = dir->path.len + fileNameLen;
    
#else
    if (!dir->d || !dir->hasNext) {
        return false;
    }
    file->statLoaded = false;
    fileName = dir->e->d_name;
    fileNameLen = SML_FS_STRLEN(dir->e->d_name);
    filePathLen = dir->path.len + fileNameLen;
#endif

    if (filePathLen + SML_FS_PATH_EXTRA_LEN + 1 >= SML_FS_MAX_PATH_LEN // TODO: to +1 or not to +1
        || fileNameLen >= SML_FS_MAX_NAME_LEN)
    {
        errno = ENAMETOOLONG;
        return false;
    }

    SML_FS_STRCPY(file->name, fileName);
    SML_FS_pathCopy(&file->path, &dir->path);

    /* concatenate fileName to directory path */
    SML_FS_pathCatStrN(&file->path, fileName, fileNameLen);

    /* get extension */
    if (file->name[0] == SML_FS_STR('.')) {
        /* it is a hidden file -> no extension, set to null character */
        file->extension = &file->name[fileNameLen];
    } else {
        SML_FS_CHAR *p = &file->name[fileNameLen];
        while (*p != SML_FS_STR('.')) {
            if (p == file->name) {
                /* no extension found - set to null character */
                file->extension = &file->name[fileNameLen];
                /* gotta use a little goto :) */
                goto next;
            }
            --p;
        }
        file->extension = p + 1;
    }

next:
    /* load file flags */
    SML_FS_fileFlags(dir, file);

    return true;
}


static bool SML_FS_fileFlags(SML_FS_Dir *dir, SML_FS_File *file)
{
#if defined(_MSC_VER)
    file->isDir = !!(dir->findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
    file->isReg = !!(dir->findData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) ||
                    ( 
                        !(dir->findData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) &&
                        !(dir->findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                        !(dir->findData.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) &&
#ifdef FILE_ATTRIBUTE_INTEGRITY_STREAM
                        !(dir->findData.dwFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM) &&
#endif
#ifdef FILE_ATTRIBUTE_NO_SCRUB_DATA
                        !(dir->findData.dwFileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA) &&
#endif
                        !(dir->findData.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE) &&
                        !(dir->findData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
                    );
    file->isSym = !!(dir->findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT);
#else
#if defined(_DIRENT_HAVE_D_TYPE) || (defined __APPLE__ && defined __MACH__)
    /* try to load flags from dirent to avoid calling stat */
    if (dir->e->d_type == DT_UNKNOWN) {
        if (!SML_FS_File_loadStat(file)) {
            return false;
        }
        file->isDir = S_ISDIR(file->stat.st_mode);
        file->isReg = S_ISREG(file->stat.st_mode);
        file->isSym = S_ISLNK(file->stat.st_mode);
    } else {
        file->isDir = dir->e->d_type == DT_DIR;
        file->isReg = dir->e->d_type == DT_REG;
        file->isSym = dir->e->d_type == DT_LNK;
    }
#else
    /* we have to assume that dirent does not have d_type field - always call stat */
    if (!SML_FS_File_loadStat(file)) {
        return false;
    }
    file->isDir = S_ISDIR(file->stat.st_mode);
    file->isReg = S_ISREG(file->stat.st_mode);
    // file->isSym = S_ISLNK(file->stat.st_mode); // TODO: check availability

#endif
#endif /* _MSC_VER */
    return true;
}

#if !defined(_MSC_VER)
static bool SML_FS_File_loadStat(SML_FS_File *me)
{
    if (me->statLoaded) {
        return true;
    }
#if defined(__MINGW32__)
    if (_tstat(me->path.buf, &me->stat) == -1) {
        return false;
    }
#elif defined _BSD_SOURCE || defined _DEFAULT_SOURCE	\
            || (defined _XOPEN_SOURCE && _XOPEN_SOURCE >= 500)	\
            || (defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) // TODO: check
    if (lstat(me->path.buf, &me->stat) == -1) {
        return false;
    }
#else
    if (stat(me->path.buf, &me->stat) == -1) {
        return false;
    }
#endif
    me->statLoaded = true;
    return true;
}
#endif

bool SML_FS_exists(SML_FS_Path *path)
{
#if defined(_MSC_VER)
    const DWORD attrib = GetFileAttributes(path->buf);
    if (attrib == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    return true;
#elif defined(__MINGW32__)
    return !_taccess(path->buf, F_OK);
#else
    return !access(path->buf, F_OK);
#endif
}