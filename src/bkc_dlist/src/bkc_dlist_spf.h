/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_dlist
 *  file name: bkc_dlist_spf.h
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
#ifndef BKC_DLIST_SPF_H
#define BKC_DLIST_SPF_H
/*********************included files*************************************/

/*********************definition, types & constants ***********************/
/*define to open , undef or no define to close*/
/*releasing or debugging this module*/
#define BKC_DLIST_SPF_RELEASE

#if defined(BKC_DLIST_SPF_RELEASE)
/*trace file name and line when printf in this module*/
/*#define BKC_DLIST_SPF_FLPRT_OPEN*/
/*trace malloc*/
/*#define BKC_DLIST_SPF_MFPRT_OPEN*/
/*trace func return checking*/
#define BKC_DLIST_SPF_RETCKPRT_OPEN
/*trace debugging printf info in specific funcs*/
/*#define BKC_DLIST_SPF_DBUGPRT_OPEN*/
#else
/*trace file name and line when printf in this module*/
#define BKC_DLIST_SPF_FLPRT_OPEN
/*trace malloc*/
#define BKC_DLIST_SPF_MFPRT_OPEN
/*trace func return checking*/
/*#define BKC_DLIST_SPF_RETCKPRT_OPEN*/
/*trace debugging printf info in specific funcs*/
#define BKC_DLIST_SPF_DBUGPRT_OPEN
#endif

/*libc std func*/
#define BKC_DLIST_SPF_STD_PRINTF printf
#define BKC_DLIST_SPF_STD_MEMSET memset
#define BKC_DLIST_SPF_STD_MEMCPY memcpy
#define BKC_DLIST_SPF_STD_STRNCPY strncpy
#define BKC_DLIST_SPF_STD_STRNCMP strncmp
#define BKC_DLIST_SPF_STD_STRLEN strlen
#define BKC_DLIST_SPF_STD_SPRINTF sprintf
#define BKC_DLIST_SPF_STD_SNPRINTF snprintf
#define BKC_DLIST_SPF_STD_STRNCAT   strncat
#define BKC_DLIST_SPF_STD_MALLOC malloc
#define BKC_DLIST_SPF_STD_FREE   free

/*libc std func used in bkc_dlist module*/
/*printf utility*/
#define BKC_DLIST_SPF_TRFL \
    do {\
        BKC_DLIST_SPF_STD_PRINTF("[file:%s][line%d]", __FILE__, __LINE__);\
        /*BKC_DLIST_SPF_STD_PRINTF("[line%d]", __LINE__);\*/\
    } while (0);

/*general printing*/
#if defined(BKC_DLIST_SPF_FLPRT_OPEN)
#define BKC_DLIST_SPF_PRT \
    BKC_DLIST_SPF_TRFL \
    BKC_DLIST_SPF_STD_PRINTF
#elif defined(BKC_DLIST_SPF_PRT_OPEN)
#define BKC_DLIST_SPF_PRT  0 ? ((void)0): BKC_DLIST_SPF_STD_PRINTF
#else
#define BKC_DLIST_SPF_PRT  1 ? ((void)0): BKC_DLIST_SPF_STD_PRINTF
#endif

/*for debugging info in a func*/
#if defined(BKC_DLIST_SPF_DBUGPRT_OPEN)
#define BKC_DLIST_SPF_DBGP  \
    BKC_DLIST_SPF_TRFL \
    BKC_DLIST_SPF_STD_PRINTF
#else
#define BKC_DLIST_SPF_DBGP  0 ? ((void)0): BKC_DLIST_SPF_STD_PRINTF
#endif

#define BKC_DLIST_SPF_NP  1 ? ((void)0): BKC_DLIST_SPF_STD_PRINTF

/*for absolutely printing in a func*/
#define BKC_DLIST_SPF_ABSPRT BKC_DLIST_SPF_STD_PRINTF

/*for err-printing in a func*/
#define BKC_DLIST_SPF_ERR \
    BKC_DLIST_SPF_TRFL \
    BKC_DLIST_SPF_STD_PRINTF("[error]");\
    BKC_DLIST_SPF_STD_PRINTF

/*for warn-printing in a func*/
#define BKC_DLIST_SPF_WARN \
    BKC_DLIST_SPF_TRFL \
    BKC_DLIST_SPF_STD_PRINTF("[warn]");\
    BKC_DLIST_SPF_STD_PRINTF

#define BKC_DLIST_SPF_MEMSET    BKC_DLIST_SPF_STD_MEMSET
#define BKC_DLIST_SPF_MEMCPY    BKC_DLIST_SPF_STD_MEMCPY
#define BKC_DLIST_SPF_STRNCPY   BKC_DLIST_SPF_STD_STRNCPY
#define BKC_DLIST_SPF_STRNCMP   BKC_DLIST_SPF_STD_STRNCMP
#define BKC_DLIST_SPF_STRLEN    BKC_DLIST_SPF_STD_STRLEN
#define BKC_DLIST_SPF_SPRINTF   BKC_DLIST_SPF_STD_SPRINTF
#define BKC_DLIST_SPF_SNPRINTF  BKC_DLIST_SPF_STD_SNPRINTF
#define BKC_DLIST_SPF_STRNCAT   BKC_DLIST_SPF_STD_STRNCAT

#if defined(BKC_DLIST_SPF_MFPRT_OPEN)
#define BKC_DLIST_SPF_MALLOC(param)  \
    bkc_dlist_spf_imalloc(param);\
    BKC_DLIST_SPF_TRFL\
    BKC_DLIST_SPF_STD_PRINTF("(malloc position info)");\
    BKC_DLIST_SPF_STD_PRINTF("\n");

#define BKC_DLIST_SPF_FREE(param)  \
    bkc_dlist_spf_ifree(param);\
    BKC_DLIST_SPF_TRFL\
    BKC_DLIST_SPF_STD_PRINTF("(free position info)");\
    BKC_DLIST_SPF_STD_PRINTF("\n");

#define BKC_DLIST_SPF_SFREE(param)  \
    do {\
        if ((param)) {\
            BKC_DLIST_SPF_FREE(param);\
            (param) = NULL;\
        }\
    } while(0);
#else
#define BKC_DLIST_SPF_MALLOC(param)  BKC_DLIST_SPF_STD_MALLOC(param)
#define BKC_DLIST_SPF_FREE(param)  BKC_DLIST_SPF_STD_FREE(param)
#define BKC_DLIST_SPF_SFREE(param)  \
    do {\
        if ((param)) {\
            BKC_DLIST_SPF_FREE(param);\
            (param) = NULL;\
        }\
    } while(0);
#endif

/*ASTRET and ASTNRET can be deactivated when released*/
#define BKC_DLIST_SPF_ASTRET(cond, ret) \
    do {\
        if (!(cond)) {\
            BKC_DLIST_SPF_ERR("assert failed, please have a check\n");\
            return (ret);\
        }\
    } while(0)

#define BKC_DLIST_SPF_ASTNRET(cond) \
    do {\
        if (!(cond)) {\
            BKC_DLIST_SPF_ERR("assert without retvalue failed, "\
                "please have a check\n");\
            return;\
        }\
    } while(0)

/*utilites for printing*/
#define BKC_DLIST_SPF_CKPRT(cond) \
    do {\
        if (!(cond)) {\
            BKC_DLIST_SPF_STD_PRINTF("please have a check," #cond "is not satisfied\n");\
        }\
    } while(0)

/*specially for checking return value in a function,
  it is used when we only want the ret-checking for a special func without
  other CKPRT in the same function*/
#if defined(BKC_DLIST_SPF_RETCKPRT_OPEN)
#define BKC_DLIST_SPF_CKPRET(cond) \
    do {\
        if (!(cond)) {\
            BKC_DLIST_SPF_TRFL\
            BKC_DLIST_SPF_STD_PRINTF("[func return error],please "\
                "have a check," #cond "is not satisfied\n");\
        }\
    } while(0)
#else
#define BKC_DLIST_SPF_CKPRET(cond) \
    do {\
        if (!(cond)) {\
        }\
    } while(0)
#endif

/*utilities for array initialization*/
#define BKC_DLIST_SPF_INITARR(array, loop, scanstart, scanlen, defval) \
    do {\
        if ( ( (array) != NULL)) {\
            for ( (loop) = (scanstart); (loop) < (scanlen); (loop)++) {\
                (array)[(loop)] = (defval);\
            }\
        }\
    } while(0)

/*normal ret value of funcs in dlist_spf*/
#define BKC_DLIST_SPF_RSUC  (0)
#define BKC_DLIST_SPF_RERR  (-1)

/*utilites for try catch style*/
/*check and throw error*/
/*declaration and initilation for try and throw styles*/
#define BKC_DLIST_SPF_DITT(infnret, interrno)  \
    int fnret = infnret; /*real ret for this func*/ \
    int ckret = 0; /*ret for checking funcs*/ \
    int terrno = interrno; /*throw err*/ \
    int err_rlyflag = 0; /*error relay flag*/ \
    int try_flag = 0 /*control the try area*/

#define BKC_DLIST_SPF_CKRET ckret
#define BKC_DLIST_SPF_FNRET fnret

/*failed abnormity*/
#define BKC_DLIST_SPF_FABN (NULL)

/*set error relay flag, and throw error*/
#define BKC_DLIST_SPF_CKTHROW(cond, errnum, retval) \
    if (!(cond)) {\
         BKC_DLIST_SPF_TRFL \
         BKC_DLIST_SPF_STD_PRINTF("" #cond " is not satisfied\n");\
        (err_rlyflag) = (-1);\
        (terrno) = (errnum);\
        (fnret) = (retval);\
        break;\
    }

/*set error return*/
#define BKC_DLIST_SPF_CKSRET(cond, retval) \
    if (!(cond)) {\
         BKC_DLIST_SPF_TRFL \
         BKC_DLIST_SPF_STD_PRINTF("" #cond " is not satisfied\n");\
        (fnret) = (retval);\
        break;\
    }

/*relay the throwed error*/
#define BKC_DLIST_SPF_RLYTHROW() \
    if ((err_rlyflag) != (0)) {\
        break;\
    }

#define BKC_DLIST_SPF_TRY  for (try_flag = 0; try_flag == 0; try_flag = -1)
#define BKC_DLIST_SPF_BREAK  break
#define BKC_DLIST_SPF_CATCH  switch (terrno)
#define BKC_DLIST_SPF_CASE(param)  case (param):
#define BKC_DLIST_SPF_DEFAULT      default :

/*for memory diagnose*/
typedef void *(*bkc_dlist_spf_malloc_func_t)(unsigned int);
typedef	void (*bkc_dlist_spf_free_func_t)(void *mem_p);
/*********************prototype of open functions************************/
/*************************************************************************
 *                  bkc_dlist_spf_resetmm
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     reset the memory record status, can be called before each
 *     diagnose
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_spf_resetmm(void);

/*************************************************************************
 *                  bkc_dlist_spf_imalloc
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     pseudo malloc
 *****************************  Notes  *****************************
 ************************************************************************/
void *bkc_dlist_spf_imalloc(unsigned int size);

/*************************************************************************
 *                  bkc_dlist_spf_ifree
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     pseudo free
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_spf_ifree(void *p);

/*************************************************************************
 *                  bkc_dlist_spf_watchmm
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     called to watch the current memory status
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_spf_watchmm(void);

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
        bkc_dlist_spf_free_func_t free_func);
#endif

