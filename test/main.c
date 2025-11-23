#include <stdio.h>
#include <limits.h>
#include <stddef.h>

#include "SML/sml.h"

/*
 * A dummy type for testing
 */
typedef struct Point {
    float x;
    float y;
} Point;

/**
 * Implement dynamic vector of unsigned int
*/
#ifndef SML_DVec_uint_IMPL
#define SML_DVec_uint_IMPL
#define SML_DVEC_T unsigned int
#define SML_DVEC_ID uint
#include "SML/sml_dyn_vector.h"
#undef SML_DVEC_ID
#undef SML_DVEC_T
#endif /* SML_DVec_uint_IMPL */ 

/**
 * Implement hash map of type unsigned int
*/
#ifndef SML_EHashMap_uint_IMPL
#define SML_EHashMap_uint_IMPL
#define SML_EHASH_T unsigned int
#define SML_EHASH_KEYT char *
#define SML_EHASH_ID uint
#define SML_EHASH_ISKEYSTRING true
#define SML_EHASH_ISKEYCSTRING true
#include "SML/sml_extendible_hash.h"
#undef SML_EHASH_ISKEYCSTRING
#undef SML_EHASH_ISKEYSTRING
#undef SML_EHASH_ID
#undef SML_EHASH_KEYT
#undef SML_EHASH_T
#endif /* SML_EHashMap_uint_IMPL */

/**
 * Implement circular buffer of unsigned int and capacity of 8
*/
#ifndef SML_CircBuf_uint_IMPL
#define SML_CircBuf_uint_IMPL
#define SML_CIRCBUF_T unsigned int
#define SML_CIRCBUF_CAPACITY 8
#define SML_CIRCBUF_ID uint
#include "SML/sml_circular_buffer.h"
#undef SML_CIRCBUF_ID
#undef SML_CIRCBUF_CAPACITY
#undef SML_CIRCBUF_T
#endif /* SML_CircBuf_uint_IMPL */ 

/**
 * Implement a dynamic queue of unsigned int
*/
#ifndef SML_DQueue_uint_IMPL
#define SML_DQueue_uint_IMPL
#define SML_DQUEUE_T unsigned int
#define SML_DQUEUE_ID uint
#include "SML/sml_dyn_queue.h"
#undef SML_DQUEUE_ID
#undef SML_DQUEUE_T
#endif /* SML_DQueue_uint_IMPL */ 

/**
 * Implement a dynamic stack of points
*/
#ifndef SML_DStack_point_IMPL
#define SML_DStack_point_IMPL
#define SML_DSTACK_T Point
#define SML_DSTACK_ID point
#include "SML/sml_dyn_stack.h"
#undef SML_DSTACK_ID
#undef SML_DSTACK_T
#endif /* SML_DStack_point_IMPL */ 

static int sort_compare_fn(const void *a, const void *b, void *sort_arr)
{
    int *arr = (int*)sort_arr;
    const int idxA = *(int *)a;
    const int idxB = *(int *)b;
    return (arr[idxA] > arr[idxB]) - (arr[idxA] < arr[idxB]);
}

static void console_write(const char *msg, unsigned int level)
{
    printf("Log level: %u, Message: %s\n", level, msg);
}

int main(void)
{
    /*
     * SML_DVec
     */
    SML_DVec_uint vec;
    SML_DVec_uint_init(&vec);

    for (int i = 0; i < 5; ++i) {
        SML_DVec_uint_push_back(&vec, i + 1);
    }

    SML_DVec_uint_set(&vec, 1, 42);
    SML_DVec_uint_set(&vec, 3, 21);
    printf("Capacity of vec: %lu\n", SML_DVec_uint_capacity(&vec));

    SML_DVec_uint_reserve(&vec, 10);
    printf("Capacity of vec after reserve(): %lu\n", SML_DVec_uint_capacity(&vec));

    SML_DVec_uint_shrink_to_fit(&vec);
    printf("Capacity of vec after shrink_to_fit(): %lu\n", SML_DVec_uint_capacity(&vec));
    
    for (const unsigned int *p = SML_DVec_uint_cbegin(&vec); p != SML_DVec_uint_cend(&vec); ++p) {
        printf("vec[%i]: %u\n", (int)(ptrdiff_t)(p - SML_DVec_uint_cbegin(&vec)), *p);
    }

    SML_DVec_uint_destroy(&vec);
    
    /*
     * SML_EHashMap
     */
    SML_EHashMap_uint hashMap;
    SML_EHashMap_uint_init(&hashMap, NULL, NULL);

    SML_EHashMap_uint_insert(&hashMap, "one", 1);
    SML_EHashMap_uint_insert(&hashMap, "two", 2);
    SML_EHashMap_uint_insert(&hashMap, "three", 3);

    unsigned int one, two, three;
    SML_EHashMap_uint_get(&hashMap, "one", &one);
    SML_EHashMap_uint_get(&hashMap, "two", &two);
    SML_EHashMap_uint_get(&hashMap, "three", &three);
    printf("one: %u, two: %u, three %u\n", one, two, three);

    SML_EHashMap_uint_erase(&hashMap, "two");
    SML_EHashMap_uint_insert(&hashMap, "two", 4);
    SML_EHashMap_uint_get(&hashMap, "two", &two);
    printf("one: %u, two (modified): %u, three %u\n", one, two, three);

    SML_EHashMap_uint_insert(&hashMap, "two", 2);

    SML_EHashMap_uint_insert(&hashMap, "four", 4);
    SML_EHashMap_uint_insert(&hashMap, "five", 5);
    SML_EHashMap_uint_insert(&hashMap, "six", 6);

    /* iterate over all items */
    for (SML_EHashMapIterator_uint it = SML_EHashMap_uint_firstIt(&hashMap); !SML_EHashMap_uint_isEndIt(&hashMap, &it); SML_EHashMap_uint_nextIt(&hashMap, &it)) {
        printf("key: %s, value: %u\n", it.item->key, it.item->data);
    }

    unsigned int *two_p = SML_EHashMap_uint_get_p(&hashMap, "two");
    printf("two by pointer: %u\n", *two_p);

    SML_EHashMap_uint_clear(&hashMap);

    SML_EHashMapIterator_uint it = SML_EHashMap_uint_firstIt(&hashMap);
    printf("Iterator begin of empty map is %sequal to iterator end\n", SML_EHashMap_uint_isEndIt(&hashMap, &it) ? "" : "NOT");

    SML_EHashMap_uint_destroy(&hashMap);

    /*
     * Fixed size SML_CircBuf
     */
    SML_CircBuf_uint circBuf;
    SML_CircBuf_uint_init(&circBuf);
    SML_CircBuf_uint_push(&circBuf, 1);
    SML_CircBuf_uint_push(&circBuf, 2);
    SML_CircBuf_uint_push(&circBuf, 3);

    while (!SML_CircBuf_uint_empty(&circBuf)) {
        printf("From circBuf: %u\n", SML_CircBuf_uint_front(&circBuf));
        SML_CircBuf_uint_pop(&circBuf);
    }

    /*
     * SML_DQueue
     */
    SML_DQueue_uint queue;
    SML_DQueue_uint_initWithCapacity(&queue, 1);
    SML_DQueue_uint_push(&queue, 1);
    SML_DQueue_uint_push(&queue, 2);
    SML_DQueue_uint_push(&queue, 3);
    SML_DQueue_uint_push(&queue, 4);
    SML_DQueue_uint_push(&queue, 5);
    SML_DQueue_uint_push(&queue, 6);
    SML_DQueue_uint_push(&queue, 7);
    printf("Size of queue: %u\n", (unsigned int)SML_DQueue_uint_size(&queue));

    while (!SML_DQueue_uint_empty(&queue)) {
        printf("From queue: %u\n", SML_DQueue_uint_front(&queue));
        SML_DQueue_uint_pop(&queue);
    }
    SML_DQueue_uint_destroy(&queue);

    /*
     * SML_DStack
     */
    SML_DStack_point stack;
    SML_DStack_point_init(&stack);
    SML_DStack_point_push(&stack, (Point){1,1});
    SML_DStack_point_push(&stack, (Point){1,2});
    SML_DStack_point_push(&stack, (Point){1,3});
    SML_DStack_point_push(&stack, (Point){1,4});
    SML_DStack_point_push(&stack, (Point){1,5});
    SML_DStack_point_push(&stack, (Point){1,6});
    SML_DStack_point_push(&stack, (Point){1,7});
    SML_DStack_point_push(&stack, (Point){1,8});
    printf("Size of stack: %u\n", (unsigned int)SML_DStack_point_size(&stack));

    while (!SML_DStack_point_empty(&stack)) {
        Point temp = SML_DStack_point_back(&stack);
        printf("From stack: x = %0.1f, y = %.1f\n", temp.x, temp.y);
        SML_DStack_point_pop(&stack);
    }

    SML_DStack_point_destroy(&stack);

    /*
     * SML_fill
     */
    int fill_arr[] = {999, 999, 999, 999, 999, 999, 999, 999, 999, 999};

    SML_fill_int(fill_arr, 42, SML_ARRCOUNT(fill_arr));
    for (unsigned int i = 0; i < SML_ARRCOUNT(fill_arr); ++i) {
        printf("fill_arr[%u]: %i\n", i, fill_arr[i]);
    }

    int replace_arr[] = {1, 2, 3, 4, 5};
    SML_fill_void(fill_arr, replace_arr, SML_ARRCOUNT(fill_arr) / SML_ARRCOUNT(replace_arr),  sizeof(replace_arr));
    for (unsigned int i = 0; i < SML_ARRCOUNT(fill_arr); ++i) {
        printf("fill_arr[%u]: %i\n", i, fill_arr[i]);
    }

    /*
     * SML_quicksort_r
     */
    int sort_arr[] = {5, 2, 3, 4, 1, 7, 8, 6};
    int idc_arr[] = {0, 1, 2, 3, 4, 5, 6, 7};
    SML_quicksort_r(idc_arr, SML_ARRCOUNT(idc_arr), sizeof(idc_arr[0]), sort_compare_fn, sort_arr);
    for (unsigned int i = 0; i < SML_ARRCOUNT(sort_arr); ++i) {
        printf("idc_arr[%u]: %i, sort_arr[idc_arr[%u]]: %i\n", i, idc_arr[i], i, sort_arr[idc_arr[i]]);
    }

    /*
     * SML_string
     */
    char numbers[] = " -1234.0 999";
    const char *from = numbers;
    const char *to;

    int num1 = SML_strtoi(from, &to);
    if (from == to) {
        return 1;
    }
    from = to;
    int num2 = SML_strtoi(from, &to);
    if (from == to) {
        return 1;
    }
    printf("num1: %i, num2: %i\n", num1, num2);

    char str_buf[64];
    char *p = str_buf;
    p = SML_strpcpy(p, "This string ");
    p = SML_strpcpy(p, "has ");
    p = SML_strpcpy(p, "3 parts");
    printf("str_buf: %s\n", str_buf);

    char buf[10];
    size_t n;
    n = SML_strscpy(buf, "1234", 10);
    printf("SML_strscpy (%struncation detected) -- n: %i, buf: %s\n", n == 10 ? "" : "no ", (int)n, buf);
    n = SML_strscpy(buf, "123456789A", 10);
    printf("SML_strscpy (%struncation detected) -- n: %i, buf: %s\n", n == 10 ? "" : "no ", (int)n, buf);
    
    char *str_dup = SML_strdup(str_buf);
    printf("str_cpy: %s\n", str_dup);
    free(str_dup);

    char strbin32[33];
    SML_uint32tobin(UINT32_MAX - 1, strbin32, 32);
    printf("strbin32: %s\n", strbin32);

    char strbin64[65];
    SML_uint64tobin(UINT64_MAX - 1, strbin64, 64);
    printf("strbin64: %s\n", strbin64);

    char stritoa[8 * sizeof(int) + 2];
    SML_itoa(stritoa, SML_ARRCOUNT(stritoa), INT_MIN, 10);
    printf("stritoa: %s\n", stritoa);

    /*
     * SML_Logger
     */
    int i;
    /* set the lowest log level */
    SML_Logger_setLogLevelConsole(SML_LOG_LVL_TRACE);

    /* overwrite default console write function and log some stuff */
    SML_Logger_setConsoleWriteFn(console_write);
    i = 0;
    LOGFATAL("This is a %s message... i = %i", "fatal", i++);
    LOGERROR("This is a %s message... i = %i", "error", i++);
    LOGWARN ("This is a %s message... i = %i", "warn ", i++);
    LOGINFO ("This is a %s message... i = %i", "info ", i++);
    LOGDEBUG("This is a %s message... i = %i", "debug", i++);
    LOGTRACE("This is a %s message... i = %i", "trace", i++);

    /* set back to the default write function */
    SML_Logger_setConsoleWriteFn(NULL);
    i = 0;
    LOGFATAL("This is a %s message... i = %i", "fatal", i++);
    LOGERROR("This is a %s message... i = %i", "error", i++);
    LOGWARN ("This is a %s message... i = %i", "warn ", i++);
    LOGINFO ("This is a %s message... i = %i", "info ", i++);
    LOGDEBUG("This is a %s message... i = %i", "debug", i++);
    LOGTRACE("This is a %s message... i = %i", "trace", i++);

    /*
     * SML_FS
     */
    SML_FS_Path path;
    SML_FS_Dir dir;
    SML_FS_File file;

#if defined(WIN32)
    /* get the current working directory */
    SML_FS_getCwd(&path);
    _tprintf(TEXT("CWD: %s\n"), path.buf);

    /* list all files in working directory */
    _tprintf(TEXT("Entries in: %s\n"), path.buf);
    if (SML_FS_openDir(&dir, &path)) {
        while (SML_FS_getNextFile(&dir, &file)) {
            if (file.extension[0] == SML_FS_STR('\0')) {
                _tprintf(TEXT("\t[%s] %s\n"), file.isDir ? TEXT("D") : TEXT("F"), file.name);
            } else {
                _tprintf(TEXT("\t[%s] %s, ext: %s\n"), file.isDir ? TEXT("D") : TEXT("F"), file.name, file.extension);
            }
        }
    }
#else
    /* get the current working directory */
    SML_FS_getCwd(&path);
    printf("CWD: %s\n", path.buf);

    /* list all files in working directory */
    printf("Entries in: %s\n", path.buf);
    if (SML_FS_openDir(&dir, &path)) {
        while (SML_FS_getNextFile(&dir, &file)) {
            if (file.extension[0] == '\0') {
                printf("\t[%s] %s\n", file.isDir ? "D" : "F", file.name);
            } else {
                printf("\t[%s] %s, ext: %s\n", file.isDir ? "D" : "F", file.name, file.extension);
            }
        }
    }
#endif

    return 0;
}