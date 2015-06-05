/**********************************************************************
 *  copyleft(c)
/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_dlist
 *  file name: bkc_dlist_spf.c
 *  description: simplifed platform supporting of the bkc_dlist mod
 *************************************************************************
 *  log:
 *  >>>
 *  version: 
 *      v1.0.0-v1.0.0
 *  time:
 *      Thursday, April 16, 2009 (090416) at 12:45:07 AM UTC/GMT +8 hours
 *  location:
 *      ChongQing, China on Earth
 *      latitude: 29.3 North, longtitude: 106.35 East 
 *      international country code: + 86 (China) 
 *      area code: 23
 *  weather:
 *      clear, mild
 *      temperature: 70 F (comfort level is 70 F)
 *      wind: 3 mph from 190 south by west
 *
 *  pgr_id:sizhe(email:tancng#gmail.com)
 *
 *  description: created
 ************************************************************************/
/*********************included files*************************************/
#include <stdio.h>
#include <stdlib.h>
#include "bkc_dlist_spf.h"

/*********************definition, types & constants ***********************/
/*400 blocks*/
#define  BKC_DLIST_SPF_MAX_BLOCK_NUM   400
/*16 M per block*/
#define  BKC_DLIST_SPF_BLOCK_SIZE   (1024 * 1024 * 16)
/*********************variables******************************************/
static unsigned int msize = 0;/*malloc size*/
static unsigned int fsize = 0;/*free size*/
static unsigned int hpsize = 0;/*heap size */
static unsigned int maxhpsize = 0; /*max heap size*/
/*for memory diagnose*/
static void *mem_pool[BKC_DLIST_SPF_MAX_BLOCK_NUM] = {NULL};
/*********************prototype of local functions***********************/

/*********************implementation of open and local functions*********/
/*************************************************************************
 *                  bkc_dlist_spf_imalloc
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 *****************************  Notes  *****************************
 ************************************************************************/
void *bkc_dlist_spf_imalloc(unsigned int size)
{
    unsigned char *ret_p = NULL;
    unsigned int *malloc_num_p = NULL;
    unsigned int *malloc_block_p = NULL;

    ret_p = BKC_DLIST_SPF_STD_MALLOC(size + sizeof(unsigned int) * 2);
    malloc_num_p = (unsigned int *)ret_p ;
    *malloc_num_p = size;
    malloc_block_p = malloc_num_p + 1;
    *malloc_block_p = (unsigned int )ret_p ;

    msize += size;
    hpsize += size;

    if (hpsize > maxhpsize)
      maxhpsize = hpsize;

    ret_p = ret_p + sizeof(unsigned int)*2;

    BKC_DLIST_SPF_PRT("(malloc size is %x) \n", *malloc_num_p );
    BKC_DLIST_SPF_PRT("(malloc addr is %x) \n", (int)malloc_block_p);

    return ret_p;
}
 
/*************************************************************************
 *                  bkc_dlist_spf_ifree
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_spf_ifree(void *p)
{
    unsigned char *ret_p = NULL;
    unsigned int *malloc_num_p = NULL;
    unsigned int *malloc_block_p = NULL;

    if (NULL !=  p) {
        ret_p = p;
        ret_p = ret_p - sizeof(unsigned int)*2;
        malloc_num_p = (unsigned int *)ret_p ;
        malloc_block_p = malloc_num_p + 1;
        fsize += *malloc_num_p;
        hpsize -= *malloc_num_p;
        
        BKC_DLIST_SPF_PRT("(free: size is %x) \n", *malloc_num_p );
        BKC_DLIST_SPF_PRT("(free: addr is %x)\n", (int)malloc_block_p);
        BKC_DLIST_SPF_STD_FREE(ret_p);
    }

    return BKC_DLIST_SPF_RSUC;
}

/*************************************************************************
 *                  bkc_dlist_spf_watchmm
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_spf_watchmm(void)
{
#ifdef BKC_DLIST_SPF_RELEASE
    BKC_DLIST_SPF_ABSPRT("empty message: not in debugging mode.\n"
        "this functionality can be activated by undefining \n"
        "BKC_DLIST_SPF_RELEASE in bkc_dlist_spf.h");
    return BKC_DLIST_SPF_RSUC;
#else
    BKC_DLIST_SPF_PRT("#####################\n");
    BKC_DLIST_SPF_PRT("mem heap size is %u\n", hpsize);
    BKC_DLIST_SPF_PRT("mem max use size is %u\n", maxhpsize );
    BKC_DLIST_SPF_PRT("#####################\n");
    return BKC_DLIST_SPF_RSUC;
#endif
}

/*************************************************************************
 *                  bkc_dlist_spf_resetmm
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_spf_resetmm(void)
{
    msize = 0;
    fsize = 0;
    hpsize = 0;
    maxhpsize = 0;
    return BKC_DLIST_SPF_RSUC;
}

/*************************************************************************
 *                  bkc_dlist_spf_memdgn
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     called to diagnose the current memory status
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_spf_memdgn(bkc_dlist_spf_malloc_func_t malloc_func, 
        bkc_dlist_spf_free_func_t free_func)
{
    int loop = 0;
    int total = 0;/*total malloc size in bytes*/
    int block_size = 0;/*variable block size*/

    if (malloc_func == NULL || free_func == NULL) {
        BKC_DLIST_SPF_ABSPRT("malloc or free func is null ,"
            "please have a check\n");
    } else {
        BKC_DLIST_SPF_ABSPRT("=====>begin bkc_dlist_spf_memdgn\n");
    }

    block_size = BKC_DLIST_SPF_MAX_BLOCK_NUM;

    for (loop = 0; loop < BKC_DLIST_SPF_MAX_BLOCK_NUM; loop++)
        mem_pool[loop] = NULL;

    loop = 0;
    while (loop < BKC_DLIST_SPF_MAX_BLOCK_NUM) {
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
    
    BKC_DLIST_SPF_ABSPRT("bkc_dlist_spf_memdgn:total malloc size is hex[%x],"
        "decimal[%d], total block num is [%d],last block_size is [%d]\n", 
        total, total, loop, block_size);

    /*free all mem*/
    for (loop = 0; loop < BKC_DLIST_SPF_MAX_BLOCK_NUM ; loop++) {
        if(mem_pool[loop] != NULL)
            free_func(mem_pool[loop]);
    }

    BKC_DLIST_SPF_ABSPRT("====>end bkc_dlist_spf_memdgn\n");
    return;
}

