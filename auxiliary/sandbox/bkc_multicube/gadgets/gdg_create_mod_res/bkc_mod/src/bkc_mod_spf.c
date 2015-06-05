/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_mod_spf
 *  file name: bkc_mod_spf.c
 *  description: simplifed platform supporting of the bkc_mod_spf mod
 *****************************************************************************
 *  log:
 *  >>>
 *  version:
 *      v1.0.1-v1.0.0
 *  time:
 *      Monday, May 5, 2009 (090504) at 1:10:27 AM UTC/GMT +8 hours
 *  location:
 *      Beijing, China on Earth
 *      latitude: 39.55 North, longtitude: 116.23 East
 *      international country code: + 86 (China)
 *      area code: 10
 *  weather:
 *      clear, mild
 *
 *  pgr_id:sizhe(email:tancng#gmail.com)
 *
 *  description: created
 ****************************************************************************/
/*********************included files*****************************************/
#include <stdio.h>
#include <stdlib.h>
#include "bkc_mod_spf.h"

/*********************definition, types & constants *************************/
/*400 blocks*/
#define  BKC_MOD_SPF_MAX_BLOCK_NUM   400
/*16 M per block*/
#define  BKC_MOD_SPF_BLOCK_SIZE   (1024 * 1024 * 16)
/*********************variables**********************************************/
static unsigned int msize = 0;/*malloc size*/
static unsigned int fsize = 0;/*free size*/
static unsigned int hpsize = 0;/*heap size */
static unsigned int maxhpsize = 0; /*max heap size*/
/*for memory diagnose*/
static void *mem_pool[BKC_MOD_SPF_MAX_BLOCK_NUM] = {NULL};
/*********************prototype of local functions***************************/

/*********************implementation of open and local functions*************/
/*****************************************************************************
 *                  bkc_mod_spf_imalloc
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_mod_spf_imalloc(unsigned int size)
{
    unsigned char *ret_p = NULL;
    unsigned int *malloc_num_p = NULL;
    unsigned int *malloc_block_p = NULL;

    ret_p = bkc_mod_spf_stdmalloc(size + sizeof(unsigned int) * 2);
    malloc_num_p = (unsigned int *)ret_p ;
    *malloc_num_p = size;
    malloc_block_p = malloc_num_p + 1;
    *malloc_block_p = (unsigned int )ret_p ;

    msize += size;
    hpsize += size;

    if (hpsize > maxhpsize)
      maxhpsize = hpsize;

    ret_p = ret_p + sizeof(unsigned int)*2;

    BKC_MOD_SPF_MDPRT("(malloc size is %d) \n", *malloc_num_p );
    BKC_MOD_SPF_MDPRT("(malloc addr is 0x%x) \n", (int)malloc_block_p);

    return ret_p;
}

/*****************************************************************************
 *                  bkc_mod_spf_ifree
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_mod_spf_ifree(void *p)
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

        BKC_MOD_SPF_MDPRT("(free: size is %d) \n", *malloc_num_p );
        BKC_MOD_SPF_MDPRT("(free: addr is 0x%x)\n", (int)malloc_block_p);
        bkc_mod_spf_stdfree(ret_p);
    }

    return BKC_MOD_SPF_RSUC;
}

/*****************************************************************************
 *                  bkc_mod_spf_istrdup
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_mod_spf_istrdup(char *str_p)
{
    char *retstr_p = NULL;

    if (str_p != NULL) {
        unsigned int len = 0;
        len = BKC_MOD_SPF_STD_STRLEN(str_p);
        retstr_p = bkc_mod_spf_imalloc(len + 1);
        if (retstr_p != NULL) {
            BKC_MOD_SPF_STD_MEMSET(retstr_p, '\0', len + 1);
            BKC_MOD_SPF_STD_STRNCPY(retstr_p, str_p, len);
        }
    }
    return retstr_p;
}

/*****************************************************************************
 *                  bkc_mod_spf_watchmm
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_mod_spf_watchmm(void)
{
#ifdef BKC_MOD_SPF_RELEASE
    BKC_MOD_SPF_ABSPRT("empty message: not in debugging mode.\n"
        "this functionality can be activated by undefining \n"
        "BKC_MOD_SPF_RELEASE in bkc_mod_spf.h");
    return BKC_MOD_SPF_RSUC;
#else
    BKC_MOD_SPF_PRT("#####################\n");
    BKC_MOD_SPF_PRT("mem heap size is %u\n", hpsize);
    BKC_MOD_SPF_PRT("mem max use size is %u\n", maxhpsize );
    BKC_MOD_SPF_PRT("#####################\n");
    return BKC_MOD_SPF_RSUC;
#endif
}

/*****************************************************************************
 *                  bkc_mod_spf_resetmm
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_mod_spf_resetmm(void)
{
    msize = 0;
    fsize = 0;
    hpsize = 0;
    maxhpsize = 0;
    return BKC_MOD_SPF_RSUC;
}

/*****************************************************************************
 *                  bkc_mod_spf_memdgn
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     called to diagnose the current memory status
 ***************************** notes **************************************
 ****************************************************************************/
void bkc_mod_spf_memdgn(bkc_mod_spf_malloc_func_t malloc_func,
        bkc_mod_spf_free_func_t free_func)
{
    int loop = 0;
    int total = 0;/*total malloc size in bytes*/
    int block_size = 0;/*variable block size*/

    if (malloc_func == NULL || free_func == NULL) {
        BKC_MOD_SPF_ABSPRT("malloc or free func is null ,"
            "please have a check\n");
    } else {
        BKC_MOD_SPF_ABSPRT("=====>begin bkc_mod_spf_memdgn\n");
    }

    block_size = BKC_MOD_SPF_MAX_BLOCK_NUM;

    for (loop = 0; loop < BKC_MOD_SPF_MAX_BLOCK_NUM; loop++)
        mem_pool[loop] = NULL;

    loop = 0;
    while (loop < BKC_MOD_SPF_MAX_BLOCK_NUM) {
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

    BKC_MOD_SPF_ABSPRT("bkc_mod_spf_memdgn:total malloc size is hex[%x],"
        "decimal[%d], total block num is [%d],last block_size is [%d]\n",
        total, total, loop, block_size);

    /*free all mem*/
    for (loop = 0; loop < BKC_MOD_SPF_MAX_BLOCK_NUM ; loop++) {
        if(mem_pool[loop] != NULL)
            free_func(mem_pool[loop]);
    }

    BKC_MOD_SPF_ABSPRT("====>end bkc_mod_spf_memdgn\n");
    return;
}

/*****************************************************************************
 *                  bkc_mod_spf_stdmalloc
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     replace the standard malloc fun, and will stuff the blocks
 *     with magic num
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_mod_spf_stdmalloc(unsigned int size)
{
    unsigned char *ret_p = NULL;
    ret_p = BKC_MOD_SPF_STD_MALLOC(size);

    if (ret_p != NULL) {
        BKC_MOD_SPF_STD_MEMSET(ret_p, BKC_MOD_MAGIC_STUFF, size);
    } else {
        BKC_MOD_SPF_ABSPRT("malloc failed, please have a check\n");
    }
    return ret_p;
}

/*****************************************************************************
 *                  bkc_mod_spf_ifree
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     paired with the bkc_mod_spf_stdmalloc
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_mod_spf_stdfree(void *p)
{
    if (NULL !=  p) {
        BKC_MOD_SPF_STD_FREE(p);
        return BKC_MOD_SPF_RSUC;
    } else {
        BKC_MOD_SPF_ABSPRT("free NULL pointer, please have a check\n");
        return BKC_MOD_SPF_RERR;
    }
}
