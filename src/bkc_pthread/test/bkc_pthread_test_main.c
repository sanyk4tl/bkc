/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_pthread
 *  file name: bkc_pthread_test.c
 *  description:  facilities for testing the bkc_pthread mod
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
#include <string.h>
#include "bkc_pthread_tspf.h"
#include "bkc_pthread.h"
/*********************definition, types & constants *************************/
#define BKC_PTHREAD_T_WATCHM
#define BKC_PTHREAD_RC(bret, rv) \
    do {\
        if (!(bret) ) {\
            printf("[err]:file %s,line %d\n", __FILE__, __LINE__);\
            return rv;\
        }\
    } while (0)

/*********************variables**********************************************/

/*********************prototype of local functions***************************/
int bkc_pthread_t_modiu(void);
void *bkc_pthread_t_prtfunc(void *arg_p);
int bkc_pthread_t_cpdp(void);
int bkc_pthread_t_cc(void);
int bkc_pthread_t_susres(void);
int bkc_pthread_t_gsattr(void);
int bkc_pthread_t_exit(void);
void *bkc_pthread_t_autoexit(void *arg_p);
int bkc_pthread_t_join(void);
int bkc_pthread_t_prtinfo(void);
void *bkc_pthread_t_autoexita(void *arg_p);
int bkc_pthread_t_auto_pdeamon(void);
int bkc_pthread_t_mutithr(void);
int bkc_pthread_t_sinthr(void);
int bkc_pthread_t_doublethr(void);

/*********************implementation of open and local functions*************/
/*****************************************************************************
 *                  bkc_pthread_t_modiu
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test module init and unint
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_modiu(void)
{
    int ret = BKC_PTHREAD_RERR;
    ret = bkc_pthread_mod_init(10, NULL, -1);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    getchar();
    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  bkc_pthread_t_prtfunc
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     the print handle
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_pthread_t_prtfunc(void *arg_p)
{
    int count = 0;

    while (count < 20) {
        printf("i am in the loop\n");
        count++;
    }
}

/*****************************************************************************
 *                  bkc_pthread_t_prtfuncloop
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     the print handle
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_pthread_t_prtfuncloop(void *arg_p)
{
    printf("\n");
    bkc_pthread_t *pthr_p = NULL;
    int count = 0;

    while (1) {
        count++;
        pthr_p = bkc_pthread_self();
        printf("count = %d, ", count);
        printf("the pthread itself is %p \n", pthr_p);
        sleep(1);
    }
}

/*****************************************************************************
 *                  bkc_pthread_t_prtfuncloop
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     the print handle
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_pthread_t_autoexit(void *arg_p)
{
    printf("\n");
    bkc_pthread_t *pthr_p = NULL;
    int count = 0;

    while (1) {
        printf("test main: i am in the loop \n");
        count++;
        pthr_p = bkc_pthread_self();
        printf("test main: the pthread itself is %p \n", pthr_p);
        printf("count = %d", count);
        sleep(1);
        if (count >= 5) {
             bkc_pthread_exit(NULL);   
        }
    }
}

/*****************************************************************************
 *                  bkc_pthread_t_autoexita
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     the print handle
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_pthread_t_autoexita(void *arg_p)
{
    printf("\n");
    bkc_pthread_t *pthr_p = NULL;
    int count = 0;

    while (1) {
        count++;
        printf("count = %d\n", count);
        sleep(1);
    #if 1
        if (count >= 5) {
             bkc_pthread_exit(NULL);   
        }
    #endif
    }
}

/*****************************************************************************
 *                  bkc_pthread_t_cpdp
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create pthread, destroy pthread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_cpdp(void)
{
    int ret = BKC_PTHREAD_RERR;
    bkc_pthread_t *pthr_p = NULL;
    bkc_pthread_attr_t pattr;

    ret = bkc_pthread_mod_init(10, NULL, -1);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    pthr_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfunc, NULL);
    BKC_PTHREAD_RC(pthr_p != NULL, -1);

#if 1
    ret = bkc_pthread_join(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

#if 1
    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  bkc_pthread_t_cc
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create pthread, cancel pthread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_cc(void)
{
    int ret = BKC_PTHREAD_RERR;
    bkc_pthread_t *pthr_p = NULL;
    bkc_pthread_attr_t pattr;

    ret = bkc_pthread_mod_init(10, NULL, -1);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    pthr_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfuncloop, NULL);
    BKC_PTHREAD_RC(pthr_p != NULL, -1);

    printf("pls enter a key to cancel this pthread\n");
    getchar();

    ret = bkc_pthread_cancel(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    ret = bkc_pthread_join(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#if 1
    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  bkc_pthread_t_susres
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test suspend pthread, resume pthread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_susres(void)
{
    int ret = BKC_PTHREAD_RERR;
    bkc_pthread_t *pthr_p = NULL;
    bkc_pthread_attr_t pattr;

    ret = bkc_pthread_mod_init(10, NULL, -1);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    pthr_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfuncloop, NULL);
    BKC_PTHREAD_RC(pthr_p != NULL, -1);

#if 1
    printf("pls enter a key to suspend  pthread\n");
    getchar();
    ret = bkc_pthread_suspend(pthr_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

#if 1
    printf("pls enter a key to resume  pthread\n");
    getchar();
    ret = bkc_pthread_resume(pthr_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

    printf("pls enter a key to cancel  pthread\n");
    getchar();
    ret = bkc_pthread_cancel(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    ret = bkc_pthread_join(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  bkc_pthread_t_gsattr
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test getting and setting thread attr 
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_gsattr(void)
{
    int ret = BKC_PTHREAD_RERR;
    bkc_pthread_t *pthr_p = NULL;
    bkc_pthread_attr_t pattr;

    ret = bkc_pthread_mod_init(10, NULL, -1);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    pthr_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfuncloop, NULL);
    BKC_PTHREAD_RC(pthr_p != NULL, -1);

    printf("pls enter a key to get attr of  pthread\n");
    getchar();

    ret = bkc_pthread_getsetattr(pthr_p, &pattr,
        BKC_PTHREAD_SELATTR_PRIORITY, BKC_PTHREAD_GSATTR_MODE_GET);
    printf("bkc_pthread_getsetattr:get:priority is %d\n", pattr.priority);
    printf("pls enter a key to continue\n");


    pattr.priority = 1;
    ret = bkc_pthread_getsetattr(pthr_p, &pattr,
        BKC_PTHREAD_SELATTR_PRIORITY, BKC_PTHREAD_GSATTR_MODE_SET);
#if 1
    ret = bkc_pthread_getsetattr(pthr_p, &pattr,
        BKC_PTHREAD_SELATTR_PRIORITY, BKC_PTHREAD_GSATTR_MODE_GET);
    printf("bkc_pthread_getsetattr:get:priority is %d\n", pattr.priority);
    printf("pls enter a key to continue\n");

    ret = bkc_pthread_getsetattr(pthr_p, &pattr,
        BKC_PTHREAD_SELATTR_DETACHS, BKC_PTHREAD_GSATTR_MODE_GET);
    printf("bkc_pthread_getsetattr:get:detachs is %d\n", pattr.is_detached);
    printf("pls enter a key to continue\n");

    pattr.is_detached = 1;
    ret = bkc_pthread_getsetattr(pthr_p, &pattr,
        BKC_PTHREAD_SELATTR_PRIORITY, BKC_PTHREAD_GSATTR_MODE_SET);
        
    ret = bkc_pthread_getsetattr(pthr_p, &pattr,
    BKC_PTHREAD_SELATTR_PRIORITY, BKC_PTHREAD_GSATTR_MODE_GET);
    printf("bkc_pthread_getsetattr:get:detachs is %d\n", pattr.is_detached);
    printf("pls enter a key to continue\n");
#endif

    printf("pls enter a key to cancel  pthread\n");
    getchar();
    ret = bkc_pthread_cancel(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    ret = bkc_pthread_join(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  bkc_pthread_t_exit
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test exit pthread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_exit(void)
{
    int ret = BKC_PTHREAD_RERR;
    bkc_pthread_t *pthr_p = NULL;
    bkc_pthread_attr_t pattr;

    ret = bkc_pthread_mod_init(10, NULL, -1);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    pthr_p = bkc_pthread_create(&pattr, bkc_pthread_t_autoexit, NULL);
    BKC_PTHREAD_RC(pthr_p != NULL, -1);

    printf("wait the sub thread to exit...\n"
        "pls enter a key to continue\n");
    getchar();

    ret = bkc_pthread_cancel(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    ret = bkc_pthread_join(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  bkc_pthread_t_join
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test join pthread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_join(void)
{
    int ret = BKC_PTHREAD_RERR;
    bkc_pthread_t *pthr_p = NULL;
    bkc_pthread_attr_t pattr;

    ret = bkc_pthread_mod_init(10, NULL, -1);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    pthr_p = bkc_pthread_create(&pattr, bkc_pthread_t_autoexit, NULL);
    BKC_PTHREAD_RC(pthr_p != NULL, -1);

    printf("join 1 time\n");
    ret = bkc_pthread_join(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    printf("join success\n");

#if 0
    printf("join 2 time, and here will get error, this is correct\n");
    ret = bkc_pthread_join(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  bkc_pthread_t_pdeamon
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test printinfo and daemon operation pthread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_prtinfo(void)
{
    int ret = BKC_PTHREAD_RERR;
    bkc_pthread_t *pthr_p = NULL;
    bkc_pthread_attr_t pattr;

    ret = bkc_pthread_mod_init(10, "./bkc_pthread_log.txt", 5);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    memcpy(pattr.create_info.creator_name, "auto exit func", BKC_PTHREAD_NAME_MAX -1 );
    memcpy(pattr.create_info.cr_file_name, __FILE__, BKC_PTHREAD_NAME_MAX - 1);
    memcpy(pattr.create_info.cr_func_name, "bkc_pthread_t_autoexita", BKC_PTHREAD_NAME_MAX - 1);
    pattr.create_info.cr_line = __LINE__;
    pthr_p = bkc_pthread_create(&pattr, bkc_pthread_t_autoexita, NULL);
    BKC_PTHREAD_RC(pthr_p != NULL, -1);

#if 0
    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM, pthr_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE, pthr_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("2press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM, pthr_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE, pthr_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
 
#else

    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_TERM, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_FILE, pthr_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("2press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_TERM, pthr_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_FILE, pthr_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#endif

    printf("press a key to join the thread\n");
    getchar();
    ret = bkc_pthread_join(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    printf("join success\n");

    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  bkc_pthread_t_auto_pdeamon
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test printinfo and auto daemon operation pthread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_auto_pdeamon(void)
{
    int ret = BKC_PTHREAD_RERR;
    bkc_pthread_t *pthr_p = NULL;
    bkc_pthread_attr_t pattr;

    ret = bkc_pthread_mod_init(10, "./bkc_pthread_log.txt", 1);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    memcpy(pattr.create_info.creator_name, "auto exit func", BKC_PTHREAD_NAME_MAX -1 );
    memcpy(pattr.create_info.cr_file_name, __FILE__, BKC_PTHREAD_NAME_MAX - 1);
    memcpy(pattr.create_info.cr_func_name, "bkc_pthread_t_autoexita", BKC_PTHREAD_NAME_MAX - 1);
    pattr.create_info.cr_line = __LINE__;
    pthr_p = bkc_pthread_create(&pattr, bkc_pthread_t_autoexita, NULL);
    BKC_PTHREAD_RC(pthr_p != NULL, -1);

#if 0
    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM_DAEMON, pthr_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

#if 0
    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE_DAEMON, pthr_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

#if 0
    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_TERM_DAEMON, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

#if 1
    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_FILE_DAEMON, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

    printf("press a key to join the thread\n");
    getchar();
    ret = bkc_pthread_join(pthr_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    printf("join success\n");

    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  bkc_pthread_t_doublethr
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test two thread with suspend and resume
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_doublethr(void)
{
    int ret = BKC_PTHREAD_RERR;
    bkc_pthread_t *pthra_p = NULL;
    bkc_pthread_t *pthrb_p = NULL;
    
    bkc_pthread_attr_t pattr;

    ret = bkc_pthread_mod_init(10, "./bkc_pthread_log.txt", 2);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    memcpy(pattr.create_info.creator_name, "prtfuncloop func1", BKC_PTHREAD_NAME_MAX -1 );
    memcpy(pattr.create_info.cr_file_name, __FILE__, BKC_PTHREAD_NAME_MAX - 1);
    memcpy(pattr.create_info.cr_func_name, "bkc_pthread_t_prtfuncloop", BKC_PTHREAD_NAME_MAX - 1);
    pattr.create_info.cr_line = __LINE__;
    pthra_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfuncloop, NULL);
    BKC_PTHREAD_RC(pthra_p != NULL, -1);
    
    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    memcpy(pattr.create_info.creator_name, "prtfuncloop func2", BKC_PTHREAD_NAME_MAX -1 );
    memcpy(pattr.create_info.cr_file_name, __FILE__, BKC_PTHREAD_NAME_MAX - 1);
    memcpy(pattr.create_info.cr_func_name, "bkc_pthread_t_prtfuncloop", BKC_PTHREAD_NAME_MAX - 1);
    pattr.create_info.cr_line = __LINE__;
    pthrb_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfuncloop, NULL);
    BKC_PTHREAD_RC(pthra_p != NULL, -1);
    
#if 0
    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_TERM_DAEMON, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

#if 1
    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_FILE_DAEMON, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

    printf("press a key to pause thread a\n");
    getchar();
    ret = bkc_pthread_suspend(pthra_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to pause thread b\n");
    getchar();
    ret = bkc_pthread_suspend(pthrb_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to resume thread a\n");
    getchar();
    ret = bkc_pthread_resume(pthra_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to resume thread b\n");
    getchar();
    ret = bkc_pthread_resume(pthrb_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to suspend daemon thread \n");
    getchar();
    ret = bkc_pthread_operate_pdeamon(BKC_PTHREAD_DAEMON_OP_MODE_SUSPEND);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to resume daemon thread \n");
    getchar();
    ret = bkc_pthread_operate_pdeamon(BKC_PTHREAD_DAEMON_OP_MODE_RESUME);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to cancel thread a\n");
    getchar();
    ret = bkc_pthread_cancel(pthra_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to cancel thread b\n");
    getchar();
    ret = bkc_pthread_cancel(pthrb_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to destroy daemon thread \n");
    getchar();
    ret = bkc_pthread_operate_pdeamon(BKC_PTHREAD_DAEMON_OP_MODE_DESTROY);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to join the thread\n");
    getchar();
    ret = bkc_pthread_join(pthra_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    printf("joina success\n");

    printf("press a key to join the thread\n");
    getchar();
    ret = bkc_pthread_join(pthrb_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    printf("joinb success\n");

    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  bkc_pthread_t_sinthr
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test single thread suspend and resume
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_sinthr(void)
{
    int ret = BKC_PTHREAD_RERR;
    bkc_pthread_t *pthra_p = NULL;

    bkc_pthread_attr_t pattr;

    ret = bkc_pthread_mod_init(10, "./bkc_pthread_log.txt", 2);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    memcpy(pattr.create_info.creator_name, "prtfuncloop func1", BKC_PTHREAD_NAME_MAX -1 );
    memcpy(pattr.create_info.cr_file_name, __FILE__, BKC_PTHREAD_NAME_MAX - 1);
    memcpy(pattr.create_info.cr_func_name, "bkc_pthread_t_prtfuncloop", BKC_PTHREAD_NAME_MAX - 1);
    pattr.create_info.cr_line = __LINE__;
    pthra_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfuncloop, NULL);
    BKC_PTHREAD_RC(pthra_p != NULL, -1);
    
    
#if 0
    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_TERM_DAEMON, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

#if 0
    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_FILE_DAEMON, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

    printf("press a key to pause thread a\n");
    getchar();
    ret = bkc_pthread_suspend(pthra_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to resume thread a\n");
    getchar();
    ret = bkc_pthread_resume(pthra_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to cancel thread a\n");
    getchar();
    ret = bkc_pthread_cancel(pthra_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to join the thread\n");
    getchar();
    ret = bkc_pthread_join(pthra_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    printf("joina success\n");

    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  bkc_pthread_t_mutithr
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create multithread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_t_mutithr(void)
{
    int ret = BKC_PTHREAD_RERR;
    bkc_pthread_t *pthra_p = NULL;
    bkc_pthread_t *pthrb_p = NULL;
    bkc_pthread_t *pthrc_p = NULL;
    bkc_pthread_t *pthrd_p = NULL;
    bkc_pthread_t *pthre_p = NULL;
    
    bkc_pthread_attr_t pattr;

    ret = bkc_pthread_mod_init(10, "./bkc_pthread_log.txt", 2);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    bkc_pthread_attr_definit(&pattr);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    memcpy(pattr.create_info.creator_name, "prtfuncloop func1", BKC_PTHREAD_NAME_MAX -1 );
    memcpy(pattr.create_info.cr_file_name, __FILE__, BKC_PTHREAD_NAME_MAX - 1);
    memcpy(pattr.create_info.cr_func_name, "bkc_pthread_t_prtfuncloop", BKC_PTHREAD_NAME_MAX - 1);
    pattr.create_info.cr_line = __LINE__;
    pthra_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfuncloop, NULL);
    BKC_PTHREAD_RC(pthra_p != NULL, -1);
    
    memcpy(pattr.create_info.creator_name, "prtfuncloop func2", BKC_PTHREAD_NAME_MAX -1 );
    memcpy(pattr.create_info.cr_file_name, __FILE__, BKC_PTHREAD_NAME_MAX - 1);
    memcpy(pattr.create_info.cr_func_name, "bkc_pthread_t_prtfuncloop", BKC_PTHREAD_NAME_MAX - 1);
    pattr.create_info.cr_line = __LINE__;
    pthrb_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfuncloop, NULL);
    BKC_PTHREAD_RC(pthrb_p != NULL, -1);
    
    memcpy(pattr.create_info.creator_name, "prtfuncloop func3", BKC_PTHREAD_NAME_MAX -1 );
    memcpy(pattr.create_info.cr_file_name, __FILE__, BKC_PTHREAD_NAME_MAX - 1);
    memcpy(pattr.create_info.cr_func_name, "bkc_pthread_t_prtfuncloop", BKC_PTHREAD_NAME_MAX - 1);
    pattr.create_info.cr_line = __LINE__;
    pthrc_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfuncloop, NULL);
    BKC_PTHREAD_RC(pthrc_p != NULL, -1);
    
    memcpy(pattr.create_info.creator_name, "prtfuncloop func4", BKC_PTHREAD_NAME_MAX -1 );
    memcpy(pattr.create_info.cr_file_name, __FILE__, BKC_PTHREAD_NAME_MAX - 1);
    memcpy(pattr.create_info.cr_func_name, "bkc_pthread_t_prtfuncloop", BKC_PTHREAD_NAME_MAX - 1);
    pattr.create_info.cr_line = __LINE__;
    pthrd_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfuncloop, NULL);
    BKC_PTHREAD_RC(pthrd_p != NULL, -1);
    
    memcpy(pattr.create_info.creator_name, "prtfuncloop func5", BKC_PTHREAD_NAME_MAX -1 );
    memcpy(pattr.create_info.cr_file_name, __FILE__, BKC_PTHREAD_NAME_MAX - 1);
    memcpy(pattr.create_info.cr_func_name, "bkc_pthread_t_prtfuncloop", BKC_PTHREAD_NAME_MAX - 1);
    pattr.create_info.cr_line = __LINE__;
    pthre_p = bkc_pthread_create(&pattr, bkc_pthread_t_prtfuncloop, NULL);
    BKC_PTHREAD_RC(pthre_p != NULL, -1);
    
#if 0
    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_TERM_DAEMON, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

#if 1
    printf("1 press a key to print the info of the thread\n");
    getchar();
    ret = bkc_pthread_prt_thrinfo(
        BKC_PTHREAD_PRTINFO_MODE_ALL_FILE_DAEMON, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
#endif

    printf("press a key to pause thread a\n");
    getchar();
    ret = bkc_pthread_suspend(pthra_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to pause thread c\n");
    getchar();
    ret = bkc_pthread_suspend(pthrc_p);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to cancel thread a\n");
    getchar();
    ret = bkc_pthread_cancel(pthra_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

    printf("press a key to cancel thread b\n");
    getchar();
    ret = bkc_pthread_cancel(pthrb_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    printf("press a key to cancel thread c\n");
    getchar();
    ret = bkc_pthread_cancel(pthrc_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    printf("press a key to cancel thread d\n");
    getchar();
    ret = bkc_pthread_cancel(pthrd_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    printf("press a key to cancel thread e\n");
    getchar();
    ret = bkc_pthread_cancel(pthre_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    
    printf("press a key to join the thread\n");
    getchar();
    ret = bkc_pthread_join(pthra_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    printf("joina success\n");

    ret = bkc_pthread_join(pthrb_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    printf("joinb success\n");

    ret = bkc_pthread_join(pthrc_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    printf("joinc success\n");

    ret = bkc_pthread_join(pthrd_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    printf("joind success\n");

    ret = bkc_pthread_join(pthre_p, NULL);
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);
    printf("joine success\n");

    ret = bkc_pthread_mod_uninit();
    BKC_PTHREAD_RC(ret == BKC_PTHREAD_RSUC, -1);

#ifdef BKC_PTHREAD_T_WATCHM
    bkc_pthread_watchmm();
#endif
    return 0;
}

/*****************************************************************************
 *                  main
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 ***************************** notes **************************************
 ****************************************************************************/
int main(int arc, char *argv[])
{
    int ret = -1;

    /*step 1*/
#if 0
    ret = bkc_pthread_t_modiu();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif
    /*step 2*/
#if 0
    ret = bkc_pthread_t_cpdp();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif

#if 0
    /*step 3*/
    ret = bkc_pthread_t_cc();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif

#if 0
    /*step 4*/
    ret = bkc_pthread_t_susres();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif

#if 0
    /*step 5*/
    ret = bkc_pthread_t_gsattr();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif

#if 1
    /*step 6*/
    ret = bkc_pthread_t_exit();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif

#if 0
    /*step 7*/
    ret = bkc_pthread_t_join();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif

#if 0
    /*step 8*/
    ret = bkc_pthread_t_prtinfo();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif

#if 0
    /*step 8*/
    ret = bkc_pthread_t_auto_pdeamon();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif

#if 0
    /*step 9*/
    ret = bkc_pthread_t_mutithr();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif

#if 0
    /*step 10*/
    ret = bkc_pthread_t_sinthr();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif

#if 0
    /*step 11*/
    ret = bkc_pthread_t_doublethr();
    BKC_PTHREAD_RC(ret == 0, -1);
#endif

    return 0;
}

