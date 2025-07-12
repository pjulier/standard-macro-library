#include <stdio.h>

#include "SML/sml.h"
#include "templates.h"


int main(void)
{
    /*
     * SML_DVec_xxx
     */
    SML_DVec_uint vec;
    SML_DVec_uint_init(&vec);

    for (int i = 0; i < 5; ++i) {
        SML_DVec_uint_push_back(&vec, i + 1);
    }

    SML_DVec_uint_set(&vec, 1, 42);
    SML_DVec_uint_set(&vec, 3, 21);
    
    for (unsigned int i = 0; i < 5; ++i) {
        printf("vec[%u]: %u\n", i, SML_DVec_uint_get(&vec, i));
    }
    SML_DVec_uint_destroy(&vec);

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
}