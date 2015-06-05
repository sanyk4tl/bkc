/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_pthread_spf
 *  file name: bkc_pthread_spf.h
 *  description: simplifed platform supporting of the bkc_pthread_spf mod
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
#ifndef BKC_PTHREAD_SPF_H
#define BKC_PTHREAD_SPF_H
/*********************included files*****************************************/

/*********************definition, types & constants *************************/
/*define to open , undef or no define to close*/
/*releasing or debugging this module*/
/*#define BKC_PTHREAD_SPF_RELEASE*/

#if defined(BKC_PTHREAD_SPF_RELEASE)
/*trace file name and line when printf in this module*/
/*#define BKC_PTHREAD_SPF_FLPRT_OPEN*/
/*trace malloc*/
/*#define BKC_PTHREAD_SPF_MFPRT_OPEN*/
/*trace func return checking*/
#define BKC_PTHREAD_SPF_RETCKPRT_OPEN
/*trace debugging printf info in specific funcs*/
/*#define BKC_PTHREAD_SPF_DBUGPRT_OPEN*/
/*trace safe usage of pointers*/
/*#define BKC_PTHREAD_SPF_REFCHECK_OPEN*/
/*trace successfull return address in a func*/
/*#define BKC_PTHREAD_SPF_PRTSUCADDR_OPEN*/
#else
/*trace file name and line for general tracing printf in this module*/
#define BKC_PTHREAD_SPF_FLPRT_OPEN
/*trace malloc*/
#define BKC_PTHREAD_SPF_MFPRT_OPEN
/*trace func return checking*/
/*#define BKC_PTHREAD_SPF_RETCKPRT_OPEN*/
/*trace debugging printf info in specific funcs*/
#define BKC_PTHREAD_SPF_DBUGPRT_OPEN
/*trace safe usage of pointers*/
#define BKC_PTHREAD_SPF_REFCHECK_OPEN
/*trace successfull return address in a func*/
#define BKC_PTHREAD_SPF_PRTSUCADDR_OPEN
#endif

/*libc std func*/
#define BKC_PTHREAD_SPF_STD_PRINTF   printf
#define BKC_PTHREAD_SPF_STD_FPRINTF  fprintf
#define BKC_PTHREAD_SPF_STD_MEMSET   memset
#define BKC_PTHREAD_SPF_STD_MEMCPY   memcpy
#define BKC_PTHREAD_SPF_STD_STRNCPY  strncpy
#define BKC_PTHREAD_SPF_STD_STRNCMP  strncmp
#define BKC_PTHREAD_SPF_STD_STRCMP   strcmp
#define BKC_PTHREAD_SPF_STD_STRLEN   strlen
#define BKC_PTHREAD_SPF_STD_STRDUP   strdup
#define BKC_PTHREAD_SPF_STD_SPRINTF  sprintf
#define BKC_PTHREAD_SPF_STD_SNPRINTF snprintf
#define BKC_PTHREAD_SPF_STD_STRNCAT  strncat
#define BKC_PTHREAD_SPF_STD_MALLOC   malloc
#define BKC_PTHREAD_SPF_STD_FREE     free

/*libc std func used in bkc_pthread module*/
/*printf utility*/
#define BKC_PTHREAD_SPF_TRFL \
    do {\
        BKC_PTHREAD_SPF_STD_PRINTF("[file:%s][line%d]", __FILE__, __LINE__);\
        /*BKC_PTHREAD_SPF_STD_PRINTF("[func:%s][line%d]", __func__, __LINE__);*/\
    } while (0)

/*general printing*/
#if defined(BKC_PTHREAD_SPF_FLPRT_OPEN)
#define BKC_PTHREAD_SPF_PRT \
    BKC_PTHREAD_SPF_TRFL; \
    BKC_PTHREAD_SPF_STD_PRINTF
#elif defined(BKC_PTHREAD_SPF_PRT_OPEN)
#define BKC_PTHREAD_SPF_PRT  0 ? ((void)0): BKC_PTHREAD_SPF_STD_PRINTF
#else
#define BKC_PTHREAD_SPF_PRT  1 ? ((void)0): BKC_PTHREAD_SPF_STD_PRINTF
#endif

/*for debugging info in a func*/
#if defined(BKC_PTHREAD_SPF_DBUGPRT_OPEN)
#define BKC_PTHREAD_SPF_DBGP  \
    BKC_PTHREAD_SPF_TRFL; \
    BKC_PTHREAD_SPF_STD_PRINTF
#else
#define BKC_PTHREAD_SPF_DBGP  1 ? ((void)0): BKC_PTHREAD_SPF_STD_PRINTF
#endif

#define BKC_PTHREAD_SPF_NP  1 ? ((void)0): BKC_PTHREAD_SPF_STD_PRINTF

/*for absolutely printing in a func*/
#define BKC_PTHREAD_SPF_ABSPRT BKC_PTHREAD_SPF_STD_PRINTF

/*for err-printing in a func*/
#define BKC_PTHREAD_SPF_ERR \
    BKC_PTHREAD_SPF_TRFL; \
    BKC_PTHREAD_SPF_STD_PRINTF("[error]");\
    BKC_PTHREAD_SPF_STD_PRINTF

/*for warn-printing in a func*/
#define BKC_PTHREAD_SPF_WARN \
    BKC_PTHREAD_SPF_TRFL;\
    BKC_PTHREAD_SPF_STD_PRINTF("[warn]");\
    BKC_PTHREAD_SPF_STD_PRINTF

#define BKC_PTHREAD_SPF_MEMSET    BKC_PTHREAD_SPF_STD_MEMSET
#define BKC_PTHREAD_SPF_MEMCPY    BKC_PTHREAD_SPF_STD_MEMCPY
#define BKC_PTHREAD_SPF_STRNCPY   BKC_PTHREAD_SPF_STD_STRNCPY
#define BKC_PTHREAD_SPF_STRNCMP   BKC_PTHREAD_SPF_STD_STRNCMP
#define BKC_PTHREAD_SPF_STRCMP    BKC_PTHREAD_SPF_STD_STRCMP
#define BKC_PTHREAD_SPF_STRLEN    BKC_PTHREAD_SPF_STD_STRLEN
#define BKC_PTHREAD_SPF_SPRINTF   BKC_PTHREAD_SPF_STD_SPRINTF
#define BKC_PTHREAD_SPF_SNPRINTF  BKC_PTHREAD_SPF_STD_SNPRINTF
#define BKC_PTHREAD_SPF_STRNCAT   BKC_PTHREAD_SPF_STD_STRNCAT
#define BKC_PTHREAD_SPF_FPRINTF   BKC_PTHREAD_SPF_STD_FPRINTF

#if defined(BKC_PTHREAD_SPF_MFPRT_OPEN)
#define BKC_PTHREAD_SPF_MALLOC(param)  \
    bkc_pthread_spf_imalloc(param);\
    BKC_PTHREAD_SPF_TRFL;\
    BKC_PTHREAD_SPF_STD_PRINTF("(malloc position info)");\
    BKC_PTHREAD_SPF_STD_PRINTF("\n");

#define BKC_PTHREAD_SPF_MDPRT \
    BKC_PTHREAD_SPF_TRFL; \
    BKC_PTHREAD_SPF_STD_PRINTF

#define BKC_PTHREAD_SPF_FREE(param)  \
    bkc_pthread_spf_ifree(param);\
    BKC_PTHREAD_SPF_TRFL;\
    BKC_PTHREAD_SPF_STD_PRINTF("(free position info)");\
    BKC_PTHREAD_SPF_STD_PRINTF("\n");

#define BKC_PTHREAD_SPF_SFREE(param)  \
    do {\
        if ((param)) {\
            BKC_PTHREAD_SPF_FREE(param);\
            (param) = NULL;\
        }\
    } while(0);

#define BKC_PTHREAD_SPF_STRDUP(param)  \
    bkc_pthread_spf_istrdup(param);\
    BKC_PTHREAD_SPF_TRFL;\
    BKC_PTHREAD_SPF_STD_PRINTF("(dup position info)");\
    BKC_PTHREAD_SPF_STD_PRINTF("\n");

#else
#define BKC_PTHREAD_SPF_MALLOC(param)  bkc_pthread_spf_stdmalloc(param)
#define BKC_PTHREAD_SPF_FREE(param)  bkc_pthread_spf_stdfree(param)
#define BKC_PTHREAD_SPF_SFREE(param)  \
    do {\
        if ((param)) {\
            BKC_PTHREAD_SPF_FREE(param);\
            (param) = NULL;\
        }\
    } while(0);

#define BKC_PTHREAD_SPF_MDPRT  1 ? ((void)0): BKC_PTHREAD_SPF_STD_PRINTF

#define BKC_PTHREAD_SPF_STRDUP(param)  BKC_PTHREAD_SPF_STD_STRDUP(param)

#endif

/*ASTRET can be deactivated when released*/
#if 1
#define BKC_PTHREAD_SPF_ASTRET(cond, errnum, retval) \
    if (!(cond)) {\
         BKC_PTHREAD_SPF_TRFL;\
         BKC_PTHREAD_SPF_STD_PRINTF("assert failed:" #cond " is not satisfied\n");\
        (err_rlyflag) = (-1);\
        (terrno) = (errnum);\
        (fnret) = (retval);\
        break;\
    }
#else
#define BKC_PTHREAD_SPF_ASTRET(cond, ret) \
    do {\
        if (!(cond)) {\
            BKC_PTHREAD_SPF_ERR("assert failed, please have a check\n");\
            return (ret);\
        }\
    } while(0)

#define BKC_PTHREAD_SPF_ASTNRET(cond) \
    do {\
        if (!(cond)) {\
            BKC_PTHREAD_SPF_ERR("assert without retvalue failed, "\
                "please have a check\n");\
            return;\
        }\
    } while(0)
#endif

/*utilites for printing*/
#define BKC_PTHREAD_SPF_CKPRT(cond) \
    do {\
        if (!(cond)) {\
            BKC_PTHREAD_SPF_STD_PRINTF("[warn]please have a "\
                "check," #cond "is not satisfied\n");\
        }\
    } while(0)

/*specially for checking return value in a function,
  it is used when we only want the ret-checking for a special
  func without other CKPRT in the same function*/
#if defined(BKC_PTHREAD_SPF_RETCKPRT_OPEN)
#define BKC_PTHREAD_SPF_CKPRET(cond) \
    do {\
        if (!(cond)) {\
            BKC_PTHREAD_SPF_TRFL;\
            BKC_PTHREAD_SPF_STD_PRINTF("[error][func return],please "\
                "have a check," #cond "is not satisfied\n");\
        }\
    } while(0)
#else
#define BKC_PTHREAD_SPF_CKPRET(cond) \
    do {\
        if (!(cond)) {\
        }\
    } while(0)
#endif

/*utilities for array initialization*/
#define BKC_PTHREAD_SPF_SETARR(array, loop, scanstart, scanlen, defval) \
    do {\
        if ( ( (array) != NULL)) {\
            for ( (loop) = (scanstart); (loop) < \
                ( (scanlen) + (scanstart)); (loop)++) {\
                (array)[(loop)] = (defval);\
            }\
        }\
    } while(0)

/*normal ret value of funcs in dlist_spf*/
#define BKC_PTHREAD_SPF_RSUC  (0)
#define BKC_PTHREAD_SPF_RERR  (-1)

/*compiler*/
#define BKC_PTHREAD_COMPILER_GCC

/*magic value for not initialized memory, just be helpfull
    to trace down when debugging*/
#if defined(BKC_PTHREAD_SPF_RELEASE)
#define BKC_PTHREAD_MAGIC_STUFF 0x33
#else
#define BKC_PTHREAD_MAGIC_STUFF 0x00
#endif

/*utilities for declaring a user defined structure*/
#if defined(BKC_PTHREAD_COMPILER_GCC)
#define BKC_PTHREAD_SPF_DCL(name, type)    \
    type name;\
    BKC_PTHREAD_SPF_MEMSET(&(name), BKC_PTHREAD_MAGIC_STUFF, sizeof(type));
#else
#define BKC_PTHREAD_SPF_DCL(name, type)    \
    type name;
#endif

/*utilities for try catch style*/
/*check and throw error*/
/*declaration and initilation for try and throw styles*/
#define BKC_PTHREAD_SPF_DITT(infnret, interrno)  \
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

#define BKC_PTHREAD_SPF_CKRET ckret
#define BKC_PTHREAD_SPF_FNRET fnret

/*failed abnormity*/
#define BKC_PTHREAD_SPF_FABN (NULL)

/*check condition,  set error relay flag, and throw error*/
#define BKC_PTHREAD_SPF_CKTHROW(cond, errnum, retval) \
    if (!(cond)) {\
         BKC_PTHREAD_SPF_TRFL;\
         BKC_PTHREAD_SPF_STD_PRINTF("[error]" #cond " is not satisfied\n");\
        (err_rlyflag) = (-1);\
        (terrno) = (errnum);\
        (fnret) = (retval);\
        break;\
    }

/*check condition, print reason, set error relay flag, and throw error*/
#define BKC_PTHREAD_SPF_CKSTHROW(cond, errnum, retval, reason_str) \
    if (!(cond)) {\
        BKC_PTHREAD_SPF_TRFL;\
        BKC_PTHREAD_SPF_STD_PRINTF("[error]" #cond " is not satisfied\n");\
        if (!(reason_str)) {\
            BKC_PTHREAD_SPF_STD_PRINTF(reason_str);\
        }\
        (err_rlyflag) = (-1);\
        (terrno) = (errnum);\
        (fnret) = (retval);\
        break;\
    }

/*directly set error relay flag, and throw error*/
#define BKC_PTHREAD_SPF_ERRTHROW(reason_str, errnum, retval) \
    BKC_PTHREAD_SPF_TRFL;\
    if (!(reason_str)) {\
        BKC_PTHREAD_SPF_STD_PRINTF(reason_str);\
    }\
    (err_rlyflag) = (-1);\
    (terrno) = (errnum);\
    (fnret) = (retval);\
    break;

/*directly set successful return value*/
#if defined(BKC_PTHREAD_SPF_PRTSUCADDR_OPEN)
#define BKC_PTHREAD_SPF_SUCTHROW(retval) \
    BKC_PTHREAD_SPF_TRFL;\
    (err_rlyflag) = (-1);\
    (fnret) = (retval);\
    break;
#else
#define BKC_PTHREAD_SPF_SUCTHROW(retval) \
    (err_rlyflag) = (-1);\
    (fnret) = (retval);\
    break;
#endif

/*directly set successful ret value and post-processing
  point*/
#if defined(BKC_PTHREAD_SPF_PRTSUCADDR_OPEN)
#define BKC_PTHREAD_SPF_SUCTHROWS(retval, catchpoint) \
    BKC_PTHREAD_SPF_TRFL;\
    (err_rlyflag) = (-1);\
    (terrno) = (catchpoint);\
    (fnret) = (retval);\
    break;
#else
#define BKC_PTHREAD_SPF_SUCTHROWS(retval, catchpoint) \
    (err_rlyflag) = (-1);\
    (terrno) = (catchpoint);\
    (fnret) = (retval);\
    break;
#endif

/*relay the throwed error*/
#define BKC_PTHREAD_SPF_RLYTHROW() \
    if ((err_rlyflag) != (0)) {\
        break;\
    }

/*return*/
#define BKC_PTHREAD_SPF_RETURN(ret) \
    do {\
        BKC_PTHREAD_SPF_CKPRET((fnret) == (sucfnret));\
        return ret;\
    } while (0)

/*null-return*/
#define BKC_PTHREAD_SPF_NRETURN \
    do {\
        BKC_PTHREAD_SPF_CKPRET((fnret) == (sucfnret));\
        return ;\
    } while (0)

#define BKC_PTHREAD_SPF_TRY  for (try_flag = 0; try_flag == 0; try_flag = -1)
#define BKC_PTHREAD_SPF_BREAK  break
#define BKC_PTHREAD_SPF_CATCH  switch (terrno)
#define BKC_PTHREAD_SPF_CASE(param)  case (param):
#define BKC_PTHREAD_SPF_DEFAULT      default :

/*get a valid string, usefull for printf */
#define BKC_PTHREAD_SPF_GETVSTR(param)  ( (param) == NULL || \
    (*(param) == '\0') ? "EMPTY" : (param))

/*short names for the above utilites used in bkc_pthread*/
/*for reference of pointer*/
#if defined(BKC_PTHREAD_SPF_REFCHECK_OPEN)
#define BKC_RF(pointer) ( ( (pointer) == NULL ? \
    (BKC_PTHREAD_SPF_STD_PRINTF("" #pointer " is null\n" \
    "[func beginning line] is %d\n"\
    "[file] %s\n", fnbegline, fnfile_p), (pointer)) \
    : (pointer)))
#else
#define BKC_RF(pointer) (pointer)
#endif
#define BKC_MSTUFF    BKC_PTHREAD_MAGIC_STUFF

#define BKC_MEMSET    BKC_PTHREAD_SPF_MEMSET
#define BKC_MEMCPY    BKC_PTHREAD_SPF_MEMCPY
#define BKC_STRNCPY   BKC_PTHREAD_SPF_STRNCPY
#define BKC_STRNCMP   BKC_PTHREAD_SPF_STRNCMP
#define BKC_STRCMP    BKC_PTHREAD_SPF_STRCMP
#define BKC_STRLEN    BKC_PTHREAD_SPF_STRLEN
#define BKC_SPRINTF   BKC_PTHREAD_SPF_SPRINTF
#define BKC_SNPRINTF  BKC_PTHREAD_SPF_SNPRINTF
#define BKC_STRNCAT   BKC_PTHREAD_SPF_STRNCAT
#define BKC_STRDUP    BKC_PTHREAD_SPF_STRDUP
#define BKC_FPRINTF   BKC_PTHREAD_SPF_FPRINTF

#define BKC_PRT       BKC_PTHREAD_SPF_PRT
#define BKC_DBGP      BKC_PTHREAD_SPF_DBGP
#define BKC_ABSPRT    BKC_PTHREAD_SPF_ABSPRT
#define BKC_ERR       BKC_PTHREAD_SPF_ERR
#define BKC_WARN      BKC_PTHREAD_SPF_WARN
#define BKC_NP        BKC_PTHREAD_SPF_NP

#define BKC_MALLOC    BKC_PTHREAD_SPF_MALLOC
#define BKC_MDPRT     BKC_PTHREAD_SPF_MDPRT
#define BKC_FREE      BKC_PTHREAD_SPF_FREE
#define BKC_SFREE     BKC_PTHREAD_SPF_SFREE

#define BKC_ASTRET    BKC_PTHREAD_SPF_ASTRET
#define BKC_CKPRT     BKC_PTHREAD_SPF_CKPRT
/*for prt return check*/
#define BKC_CKPRET    BKC_PTHREAD_SPF_CKPRET

#define BKC_DITT      BKC_PTHREAD_SPF_DITT
#define BKC_RETURN    BKC_PTHREAD_SPF_RETURN
#define BKC_NRETURN    BKC_PTHREAD_SPF_NRETURN

/*flow control utilities*/
#define BKC_FLOWCONTROL_TRY       BKC_PTHREAD_SPF_TRY
#define BKC_FLOWCONTROL_CATCH     BKC_PTHREAD_SPF_CATCH
#define BKC_FC_BREAK              BKC_PTHREAD_SPF_BREAK
#define BKC_FC_CASE               BKC_PTHREAD_SPF_CASE
#define BKC_FC_DEFAULT            BKC_PTHREAD_SPF_DEFAULT
#define BKC_FC_CT                 BKC_PTHREAD_SPF_CKSTHROW
#define BKC_FC_ERRTHR             BKC_PTHREAD_SPF_ERRTHROW
#define BKC_FC_SUCTHR             BKC_PTHREAD_SPF_SUCTHROW
#define BKC_FC_SUCTHRS            BKC_PTHREAD_SPF_SUCTHROWS
#define BKC_FC_RT                 BKC_PTHREAD_SPF_RLYTHROW
/*for pointer checking*/
#define BKC_FC_PCK                BKC_FC_CT
/*for calling and checking*/
#define BKC_CCT(func_calling, ret_checking)   \
    do {\
        func_calling;\
        ret_checking;\
    }

#define BKC_CR        BKC_PTHREAD_SPF_CKRET
#define BKC_FNRET     BKC_PTHREAD_SPF_FNRET
#define BKC_FABN      BKC_PTHREAD_SPF_FABN

#define BKC_SETARR   BKC_PTHREAD_SPF_SETARR
#define BKC_GETVSTR   BKC_PTHREAD_SPF_GETVSTR

/*for memory diagnose*/
typedef void *(*bkc_pthread_spf_malloc_func_t)(unsigned int);
typedef void (*bkc_pthread_spf_free_func_t)(void *mem_p);
/*********************prototype of open functions****************************/
/*****************************************************************************
 *                  bkc_pthread_spf_resetmm
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     reset the memory record status, can be called before each
 *     diagnose
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_spf_resetmm(void);

/*****************************************************************************
 *                  bkc_pthread_spf_imalloc
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     pseudo malloc
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_pthread_spf_imalloc(unsigned int size);

/*****************************************************************************
 *                  bkc_pthread_spf_ifree
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     pseudo free
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_spf_ifree(void *p);

/*****************************************************************************
 *                  bkc_pthread_spf_istrdup
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     pseudo strdup
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_pthread_spf_istrdup(char *str_p);

/*****************************************************************************
 *                  bkc_pthread_spf_watchmm
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     called to watch the current memory status
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_spf_watchmm(void);

/*****************************************************************************
 *                  bkc_pthread_spf_memdgn
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     called to diagnose the current memory status
 ***************************** notes **************************************
 ****************************************************************************/
void bkc_pthread_spf_memdgn(bkc_pthread_spf_malloc_func_t malloc_func,
        bkc_pthread_spf_free_func_t free_func);

/*****************************************************************************
 *                  bkc_pthread_spf_stdmalloc
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     use to replace the standard malloc, and will stuff the magic num
 *     for each malloc blocks
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_pthread_spf_stdmalloc(unsigned int size);


/*****************************************************************************
 *                  bkc_pthread_spf_stdfree
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     paired with the bkc_pthread_spf_stdmalloc
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_spf_stdfree(void *p);

#endif

