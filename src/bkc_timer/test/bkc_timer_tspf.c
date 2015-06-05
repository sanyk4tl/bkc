/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_timer
 *  file name: bkc_timer_tspf.c
 *  description: supporting utilites for the bkc_timer_test mod
 *****************************************************************************
 *  log:
 *  >>>
 *  version:
 *      v1.0.1-v1.0.0
 *  time:
 *      Thursday, April 16, 2009 (090416) at 12:45:07 AM UTC/GMT +8 hours
 *  location:
 *      ChongQing, China on Earth
 *      latitude: 29.3 North, longtitude: 106.35 East
 *      international country code: + 86 (China)
 *      area code: 23
 *  weather:
 *      clear, mild
 *  pgr_id:sizhe(email:tancng#gmail.com)
 *
 *  description: created
 ****************************************************************************/
/*********************included files*****************************************/
#include <stdio.h>
#include <stdlib.h>
#include "bkc_timer_tspf.h"
/*********************definition, types & constants *************************/
/*1000k blocks*/
#define  BKC_TIMER_TSPF_MAX_BLOCK_NUM   (1024000)
/*1024Gb per block*/
#define  BKC_TIMER_TSPF_BLOCK_SIZE   (1024 * 1024 * 1024 * 1024)
/*********************variables**********************************************/
/*for memory diagnose*/
static void *mem_pool[BKC_TIMER_TSPF_MAX_BLOCK_NUM] = {NULL};
/*********************prototype of local functions***************************/

/*********************implementation of open and local functions*************/

/*****************************************************************************
 *                  bkc_timer_tspf_memdgn
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     called to diagnose the current memory status
 *****************************  Notes  *****************************
 ****************************************************************************/
void bkc_timer_tspf_memdgn(bkc_timer_tspf_malloc_func_t malloc_func,
        bkc_timer_tspf_free_func_t free_func)
{
    int loop = 0;
    /*total malloc size in bytes*/
    long long  total = 0;
    /*variable block size*/
    int block_size = 0;

    if (malloc_func == NULL || free_func == NULL) {
        printf("malloc or free func is null , please have a check\n");
    } else {
        printf("=============>begin bkc_timer_tspf_memdgn\n");
    }

    block_size = BKC_TIMER_TSPF_MAX_BLOCK_NUM;

    for (loop = 0; loop < BKC_TIMER_TSPF_MAX_BLOCK_NUM; loop++)
        mem_pool[loop] = NULL;

    loop = 0;
    while (loop < BKC_TIMER_TSPF_MAX_BLOCK_NUM) {
        mem_pool[loop] = malloc_func(block_size);

        if (mem_pool[loop] != NULL) {
            total += block_size;
            loop++;
        } else {
            block_size /= 2;
            if (block_size < 1)
                break;
        }
    }

    printf("BKC_MemDiagnose:total malloc size is [%lld], "
        "total block num is [%d],last block_size is [%d]\n",
        total, loop, block_size);

    /*free all mem*/
    for (loop = 0; loop < BKC_TIMER_TSPF_MAX_BLOCK_NUM ; loop++) {
        if(mem_pool[loop] != NULL)
            free_func(mem_pool[loop]);
    }

    printf("=============>end bkc_timer_tspf_memdgn\n");
    return;
}

