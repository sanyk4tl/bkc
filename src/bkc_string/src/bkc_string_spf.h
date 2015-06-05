/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_string_spf
 *  file name: bkc_string_spf.h
 *  description: simplifed platform supporting of the bkc_string_spf mod
 *************************************************************************
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
 ************************************************************************/
#ifndef BKC_STRING_SPF_H
#define BKC_STRING_SPF_H
/*********************included files*************************************/

/*********************definition, types & constants ***********************/
/*define to open , undef or no define to close*/
/*releasing or debugging this module*/
#define BKC_STRING_SPF_RELEASE

#if defined(BKC_STRING_SPF_RELEASE)
/*trace file name and line when printf in this module*/
/*#define BKC_STRING_SPF_FLPRT_OPEN*/
/*trace malloc*/
/*#define BKC_STRING_SPF_MFPRT_OPEN*/
/*trace func return checking*/
#define BKC_STRING_SPF_RETCKPRT_OPEN
/*trace debugging printf info in specific funcs*/
/*#define BKC_STRING_SPF_DBUGPRT_OPEN*/
/*trace safe usage of pointers*/
/*#define BKC_STRING_SPF_REFCHECK_OPEN*/
#else
/*trace file name and line for general tracing printf in this module*/
#define BKC_STRING_SPF_FLPRT_OPEN
/*trace malloc*/
#define BKC_STRING_SPF_MFPRT_OPEN
/*trace func return checking*/
/*#define BKC_STRING_SPF_RETCKPRT_OPEN*/
/*trace debugging printf info in specific funcs*/
#define BKC_STRING_SPF_DBUGPRT_OPEN
/*trace safe usage of pointers*/
#define BKC_STRING_SPF_REFCHECK_OPEN
#endif

/*libc std func*/
#define BKC_STRING_SPF_STD_PRINTF   printf
#define BKC_STRING_SPF_STD_MEMSET   memset
#define BKC_STRING_SPF_STD_MEMCPY   memcpy
#define BKC_STRING_SPF_STD_STRNCPY  strncpy
#define BKC_STRING_SPF_STD_STRNCMP  strncmp
#define BKC_STRING_SPF_STD_STRCMP   strcmp
#define BKC_STRING_SPF_STD_STRLEN   strlen
#define BKC_STRING_SPF_STD_STRDUP   strdup
#define BKC_STRING_SPF_STD_SPRINTF  sprintf
#define BKC_STRING_SPF_STD_SNPRINTF snprintf
#define BKC_STRING_SPF_STD_STRNCAT  strncat
#define BKC_STRING_SPF_STD_MALLOC   malloc
#define BKC_STRING_SPF_STD_FREE     free

/*libc std func used in bkc_string module*/
/*printf utility*/
#define BKC_STRING_SPF_TRFL \
    do {\
        BKC_STRING_SPF_STD_PRINTF("[file:%s][line%d]", __FILE__, __LINE__);\
    } while (0)

/*general printing*/
#if defined(BKC_STRING_SPF_FLPRT_OPEN)
#define BKC_STRING_SPF_PRT \
    BKC_STRING_SPF_TRFL; \
    BKC_STRING_SPF_STD_PRINTF
#elif defined(BKC_STRING_SPF_PRT_OPEN)
#define BKC_STRING_SPF_PRT  0 ? ((void)0): BKC_STRING_SPF_STD_PRINTF
#else
#define BKC_STRING_SPF_PRT  1 ? ((void)0): BKC_STRING_SPF_STD_PRINTF
#endif

/*for debugging info in a func*/
#if defined(BKC_STRING_SPF_DBUGPRT_OPEN)
#define BKC_STRING_SPF_DBGP  \
    BKC_STRING_SPF_TRFL; \
    BKC_STRING_SPF_STD_PRINTF
#else
#define BKC_STRING_SPF_DBGP  0 ? ((void)0): BKC_STRING_SPF_STD_PRINTF
#endif

/*for absolutely printing in a func*/
#define BKC_STRING_SPF_ABSPRT BKC_STRING_SPF_STD_PRINTF

/*for err-printing in a func*/
#define BKC_STRING_SPF_ERR \
    BKC_STRING_SPF_TRFL; \
    BKC_STRING_SPF_STD_PRINTF("[error]");\
    BKC_STRING_SPF_STD_PRINTF

/*for warn-printing in a func*/
#define BKC_STRING_SPF_WARN \
    BKC_STRING_SPF_TRFL;\
    BKC_STRING_SPF_STD_PRINTF("[warn]");\
    BKC_STRING_SPF_STD_PRINTF

#define BKC_STRING_SPF_MEMSET    BKC_STRING_SPF_STD_MEMSET
#define BKC_STRING_SPF_MEMCPY    BKC_STRING_SPF_STD_MEMCPY
#define BKC_STRING_SPF_STRNCPY   BKC_STRING_SPF_STD_STRNCPY
#define BKC_STRING_SPF_STRNCMP   BKC_STRING_SPF_STD_STRNCMP
#define BKC_STRING_SPF_STRCMP    BKC_STRING_SPF_STD_STRCMP
#define BKC_STRING_SPF_STRLEN    BKC_STRING_SPF_STD_STRLEN
#define BKC_STRING_SPF_SPRINTF   BKC_STRING_SPF_STD_SPRINTF
#define BKC_STRING_SPF_SNPRINTF  BKC_STRING_SPF_STD_SNPRINTF
#define BKC_STRING_SPF_STRNCAT   BKC_STRING_SPF_STD_STRNCAT

#if defined(BKC_STRING_SPF_MFPRT_OPEN)
#define BKC_STRING_SPF_MALLOC(param)  \
    bkc_string_spf_imalloc(param);\
    BKC_STRING_SPF_TRFL;\
    BKC_STRING_SPF_STD_PRINTF("(malloc position info)");\
    BKC_STRING_SPF_STD_PRINTF("\n");

#define BKC_STRING_SPF_MDPRT \
    BKC_STRING_SPF_TRFL; \
    BKC_STRING_SPF_STD_PRINTF

#define BKC_STRING_SPF_FREE(param)  \
    bkc_string_spf_ifree(param);\
    BKC_STRING_SPF_TRFL;\
    BKC_STRING_SPF_STD_PRINTF("(free position info)");\
    BKC_STRING_SPF_STD_PRINTF("\n");

#define BKC_STRING_SPF_SFREE(param)  \
    do {\
        if ((param)) {\
            BKC_STRING_SPF_FREE(param);\
            (param) = NULL;\
        }\
    } while(0);

#define BKC_STRING_SPF_STRDUP(param)  \
    bkc_string_spf_istrdup(param);\
    BKC_STRING_SPF_TRFL;\
    BKC_STRING_SPF_STD_PRINTF("(dup position info)");\
    BKC_STRING_SPF_STD_PRINTF("\n");

#else
#define BKC_STRING_SPF_MALLOC(param)  BKC_STRING_SPF_STD_MALLOC(param)
#define BKC_STRING_SPF_FREE(param)  BKC_STRING_SPF_STD_FREE(param)
#define BKC_STRING_SPF_SFREE(param)  \
    do {\
        if ((param)) {\
            BKC_STRING_SPF_FREE(param);\
            (param) = NULL;\
        }\
    } while(0);

#define BKC_STRING_SPF_MDPRT  1 ? ((void)0): BKC_STRING_SPF_STD_PRINTF

#define BKC_STRING_SPF_STRDUP(param)  BKC_STRING_SPF_STD_STRDUP(param)

#endif

/*ASTRET can be deactivated when released*/
#if 1
#define BKC_STRING_SPF_ASTRET(cond, errnum, retval) \
    if (!(cond)) {\
         BKC_STRING_SPF_TRFL;\
         BKC_STRING_SPF_STD_PRINTF("assert failed:" #cond " is not satisfied\n");\
        (err_rlyflag) = (-1);\
        (terrno) = (errnum);\
        (fnret) = (retval);\
        break;\
    }
#else
#define BKC_STRING_SPF_ASTRET(cond, ret) \
    do {\
        if (!(cond)) {\
            BKC_STRING_SPF_ERR("assert failed, please have a check\n");\
            return (ret);\
        }\
    } while(0)

#define BKC_STRING_SPF_ASTNRET(cond) \
    do {\
        if (!(cond)) {\
            BKC_STRING_SPF_ERR("assert without retvalue failed, "\
                "please have a check\n");\
            return;\
        }\
    } while(0)
#endif

/*utilites for printing*/
#define BKC_STRING_SPF_CKPRT(cond) \
    do {\
        if (!(cond)) {\
            BKC_STRING_SPF_STD_PRINTF("[warn]please have a "\
                "check," #cond "is not satisfied\n");\
        }\
    } while(0)

/*specially for checking return value in a function, 
  it is used when we only want the ret-checking for a special 
  func without other CKPRT in the same function*/
#if defined(BKC_STRING_SPF_RETCKPRT_OPEN) 
#define BKC_STRING_SPF_CKPRET(cond) \
    do {\
        if (!(cond)) {\
            BKC_STRING_SPF_TRFL;\
            BKC_STRING_SPF_STD_PRINTF("[error][func return],please "\
                "have a check," #cond "is not satisfied\n");\
        }\
    } while(0)
#else
#define BKC_STRING_SPF_CKPRET(cond) \
    do {\
        if (!(cond)) {\
        }\
    } while(0)
#endif

/*utilities for array initialization*/
#define BKC_STRING_SPF_SETARR(array, loop, scanstart, scanlen, defval) \
    do {\
        if ( ( (array) != NULL)) {\
            for ( (loop) = (scanstart); (loop) < \
                ( (scanlen) + (scanstart)); (loop)++) {\
                (array)[(loop)] = (defval);\
            }\
        }\
    } while(0)

/*normal ret value of funcs in dlist_spf*/
#define BKC_STRING_SPF_RSUC  (0)
#define BKC_STRING_SPF_RERR  (-1)

/*utilites for try catch style*/
/*check and throw error*/
/*declaration and initilation for try and throw styles*/
#define BKC_STRING_SPF_DITT(infnret, interrno)  \
    /*successfull running status for this func*/ \
    int fnbegline = __LINE__; \
    char *fnfile_p = __FILE__; \
    int sucfnret = infnret; \
    /*real ret for this func*/ \
    int fnret = infnret; \
    /*ret for checking funcs*/ \
    int ckret = 0; \
    /*thrown err*/ \
    int terrno = interrno; \
    /*error relay flag*/ \
    int err_rlyflag = 0; \
    /*control the try area*/ \
    int try_flag = 0

#define BKC_STRING_SPF_CKRET ckret
#define BKC_STRING_SPF_FNRET fnret

/*failed abnormity*/
#define BKC_STRING_SPF_FABN (NULL)

/*set error relay flag, and throw error*/
#define BKC_STRING_SPF_CKTHROW(cond, errnum, retval) \
    if (!(cond)) {\
         BKC_STRING_SPF_TRFL;\
         BKC_STRING_SPF_STD_PRINTF("" #cond " is not satisfied\n");\
        (err_rlyflag) = (-1);\
        (terrno) = (errnum);\
        (fnret) = (retval);\
        break;\
    }

/*set error return*/
#define BKC_STRING_SPF_CKSRET(cond, retval) \
    if (!(cond)) {\
         BKC_STRING_SPF_TRFL;\
         BKC_STRING_SPF_STD_PRINTF("" #cond " is not satisfied\n");\
        (fnret) = (retval);\
        break;\
    }

/*relay the throwed error*/
#define BKC_STRING_SPF_RLYTHROW() \
    if ((err_rlyflag) != (0)) {\
        break;\
    }

/*return*/
#define BKC_STRING_SPF_RETURN(ret) \
    BKC_STRING_SPF_CKPRET((fnret) == (sucfnret));\
    return ret;

#define BKC_STRING_SPF_TRY  for (try_flag = 0; try_flag == 0; try_flag = -1)
#define BKC_STRING_SPF_BREAK  break
#define BKC_STRING_SPF_CATCH  switch (terrno)
#define BKC_STRING_SPF_CASE(param)  case (param):
#define BKC_STRING_SPF_DEFAULT      default :

/*get a valid string, usefull for printf */
#define BKC_STRING_SPF_GETVSTR(param)  ( (param) == NULL || \
    (*(param) == '\0') ? "EMPTY" : (param))

/*short names for the above utilites used in bkc_string*/
/*for reference of pointer*/
#if defined(BKC_STRING_SPF_REFCHECK_OPEN)
#define BKC_RF(pointer) ( ( (pointer) == NULL ? \
    (BKC_STRING_SPF_STD_PRINTF("" #pointer " is null\n" \
    "[func beginning line] is %d\n"\
    "[file] %s\n", fnbegline, fnfile_p), (pointer)) \
    : (pointer)))
#else
#define BKC_RF(pointer) (pointer)
#endif

#define BKC_MEMSET    BKC_STRING_SPF_MEMSET
#define BKC_MEMCPY    BKC_STRING_SPF_MEMCPY
#define BKC_STRNCPY   BKC_STRING_SPF_STRNCPY
#define BKC_STRNCMP   BKC_STRING_SPF_STRNCMP
#define BKC_STRCMP    BKC_STRING_SPF_STRCMP
#define BKC_STRLEN    BKC_STRING_SPF_STRLEN
#define BKC_SPRINTF   BKC_STRING_SPF_SPRINTF
#define BKC_SNPRINTF  BKC_STRING_SPF_SNPRINTF
#define BKC_STRNCAT   BKC_STRING_SPF_STRNCAT
#define BKC_STRDUP    BKC_STRING_SPF_STRDUP

#define BKC_PRT       BKC_STRING_SPF_PRT
#define BKC_DBGP      BKC_STRING_SPF_DBGP
#define BKC_ABSPRT    BKC_STRING_SPF_ABSPRT
#define BKC_ERR       BKC_STRING_SPF_ERR
#define BKC_WARN      BKC_STRING_SPF_WARN

#define BKC_MALLOC    BKC_STRING_SPF_MALLOC
#define BKC_MDPRT     BKC_STRING_SPF_MDPRT
#define BKC_FREE      BKC_STRING_SPF_FREE
#define BKC_SFREE     BKC_STRING_SPF_SFREE

#define BKC_ASTRET    BKC_STRING_SPF_ASTRET
#define BKC_CKPRT     BKC_STRING_SPF_CKPRT
#define BKC_CKPRET    BKC_STRING_SPF_CKPRET

#define BKC_DITT      BKC_STRING_SPF_DITT
#define BKC_CKSRET    BKC_STRING_SPF_CKSRET
#define BKC_RETURN    BKC_STRING_SPF_RETURN

/*flow control utilities*/
#define BKC_FLOWCONTROL_TRY       BKC_STRING_SPF_TRY
#define BKC_FLOWCONTROL_CATCH     BKC_STRING_SPF_CATCH
#define BKC_FC_BREAK     BKC_STRING_SPF_BREAK
#define BKC_FC_CASE      BKC_STRING_SPF_CASE
#define BKC_FC_DEFAULT   BKC_STRING_SPF_DEFAULT
#define BKC_FC_CKTHROW   BKC_STRING_SPF_CKTHROW
#define BKC_FC_RLYTHROW  BKC_STRING_SPF_RLYTHROW

#define BKC_CKRET     BKC_STRING_SPF_CKRET
#define BKC_FNRET     BKC_STRING_SPF_FNRET
#define BKC_FABN      BKC_STRING_SPF_FABN

#define BKC_SETARR   BKC_STRING_SPF_SETARR

#define BKC_GETVSTR   BKC_STRING_SPF_GETVSTR

/*for memory diagnose*/
typedef void *(*bkc_string_spf_malloc_func_t)(unsigned int);
typedef	void (*bkc_string_spf_free_func_t)(void *mem_p);
/*********************prototype of open functions************************/
/*************************************************************************
 *                  bkc_string_spf_resetmm
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     reset the memory record status, can be called before each
 *     diagnose
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_spf_resetmm(void);

/*************************************************************************
 *                  bkc_string_spf_imalloc
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     pseudo malloc
 *****************************  Notes  *****************************
 ************************************************************************/
void *bkc_string_spf_imalloc(unsigned int size);

/*************************************************************************
 *                  bkc_string_spf_ifree
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     pseudo free
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_spf_ifree(void *p);

/*************************************************************************
 *                  bkc_string_spf_watchmm
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     called to watch the current memory status
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_spf_watchmm(void);

/*************************************************************************
 *                  bkc_string_spf_memdgn
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     called to diagnose the current memory status
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_string_spf_memdgn(bkc_string_spf_malloc_func_t malloc_func, 
        bkc_string_spf_free_func_t free_func);
#endif

