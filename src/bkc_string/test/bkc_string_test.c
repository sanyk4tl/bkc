/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_string
 *  file name: bkc_string_test.c
 *  description:  facilities for testing the bkc_string mod
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
/*********************included files*************************************/
#include <stdio.h>
#include <stdlib.h>
#include "bkc_string_tspf.h"
#include "bkc_string.h"
/*********************definition, types & constants ***********************/
/*#define BKC_STRING_T_WATCHM*/
/*********************variables******************************************/

/*********************prototype of local functions***********************/
void bkc_string_t_cd(void);
void bkc_string_t_appd(void);
void bkc_string_t_chglen(void);
void bkc_string_t_trim(void);
void bkc_string_t_cpy(void);
void bkc_string_t_dup(void);
void bkc_string_t_cmp(void);
void bkc_string_t_gettext(void);
void bkc_string_t_isempty(void);

/*********************implementation of open and local functions*********/
/*************************************************************************
 *                  bkc_string_t_cd
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create and destroy
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_string_t_cd(void)
{
    bkc_string_t *str_p = NULL;

    str_p = bkc_string_create("new");
    bkc_string_prtinfo(str_p);
    bkc_string_destroy(str_p);

#ifdef BKC_STRING_T_WATCHM
    bkc_string_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_string_t_appd
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test bkc_string_cat
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_string_t_appd(void)
{
    bkc_string_t *str_p = NULL;
    bkc_string_t *growstr_p = NULL;

    growstr_p = bkc_string_create("growing string");
    str_p = bkc_string_create(" first");

    bkc_string_appd(growstr_p, str_p, " second", -1);
    bkc_string_prtinfo(growstr_p);

    bkc_string_appd(growstr_p, NULL, " third", -1);
    bkc_string_prtinfo(growstr_p);

    bkc_string_appdchar(growstr_p, 'o');
    bkc_string_prtinfo(growstr_p);

    bkc_string_appdint(growstr_p, 1000);
    bkc_string_prtinfo(growstr_p);

    bkc_string_destroy(str_p);
    bkc_string_destroy(growstr_p);

#ifdef BKC_STRING_T_WATCHM
    bkc_string_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_string_t_chglen
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test bkc_string_chg and int bkc_string_getlen
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_string_t_chglen(void)
{
    bkc_string_t *str_p = NULL;

    str_p = bkc_string_create("first");
    bkc_string_prtinfo(str_p);

    bkc_string_chg(str_p, "second");
    bkc_string_prtinfo(str_p);

    bkc_string_chg(str_p, "third");
    bkc_string_prtinfo(str_p);

    bkc_string_chg(str_p, NULL);
    bkc_string_prtinfo(str_p);

    bkc_string_chg(str_p, "empty");
    bkc_string_prtinfo(str_p);
    printf("string len is %d\n", bkc_string_getlen(str_p));

    bkc_string_destroy(str_p);

#ifdef BKC_STRING_T_WATCHM
    bkc_string_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_string_t_trim
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test bkc_string_trim
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_string_t_trim(void)
{
    bkc_string_t *str_p = NULL;

    str_p = bkc_string_create("    abcdefghijklmnopqrstuvwxyz    ");
    bkc_string_prtinfo(str_p);

    printf("test BKC_STRING_TRIMLSPACE\n");
    bkc_string_trim(str_p, BKC_STRING_TRIMLSPACE, 0);
    bkc_string_prtinfo(str_p);

    printf("test BKC_STRING_TRIMRSPACE\n");
    bkc_string_trim(str_p, BKC_STRING_TRIMRSPACE, 0);
    bkc_string_prtinfo(str_p);

    printf("change string\n");
    bkc_string_chg(str_p, "    abcd    efgh    ijkl    mnop"
        "    qrst    uvw    xyz    ");
    bkc_string_prtinfo(str_p);

    printf("test BKC_STRING_TRIMLRSPACE\n");
    bkc_string_trim(str_p, BKC_STRING_TRIMLRSPACE, 0);
    bkc_string_prtinfo(str_p);

    printf("change string\n");
    bkc_string_chg(str_p, "    abcd    efgh    ijkl    mnop"
        "    qrst    uvw    xyz    ");
    bkc_string_prtinfo(str_p);

    printf("test BKC_STRING_TRIMCSPACE\n");
    bkc_string_trim(str_p, BKC_STRING_TRIMCSPACE, 0);
    bkc_string_prtinfo(str_p);

    printf("test BKC_STRING_TRIMLEN\n");
    bkc_string_trim(str_p, BKC_STRING_TRIMLEN, 13);
    bkc_string_prtinfo(str_p);

    bkc_string_destroy(str_p);

#ifdef BKC_STRING_T_WATCHM
    bkc_string_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_string_t_dup
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test bkc_string_dup
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_string_t_dup(void)
{
    bkc_string_t *sstr_p = NULL;
    bkc_string_t *dstr_p = NULL;

    sstr_p = bkc_string_create("    abcdefghijklmnopqrstuvwxyz    ");
    bkc_string_prtinfo(sstr_p);

    printf("test dup\n");
    dstr_p = bkc_string_dup(sstr_p);
    bkc_string_prtinfo(dstr_p);

    bkc_string_destroy(sstr_p);
    bkc_string_destroy(dstr_p);

#ifdef BKC_STRING_T_WATCHM
    bkc_string_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_string_t_gettext
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test bkc_string_t_gettext
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_string_t_gettext(void)
{
    bkc_string_t *sstr_p = NULL;
    char *retstr_p = NULL;
    
    sstr_p = bkc_string_create("abcdefghijklmnopqrstuvwxyz");
    bkc_string_prtinfo(sstr_p);

    bkc_string_gettext(sstr_p, &retstr_p, malloc);
    printf("returned string is %s\n", retstr_p == NULL ? "emtpy" :
        retstr_p);

    if (retstr_p != NULL)
        free(retstr_p);

    bkc_string_destroy(sstr_p);

#ifdef BKC_STRING_T_WATCHM
    bkc_string_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_string_cmp
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test bkc_string_cmp
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_string_t_cmp(void)
{
    bkc_string_t *sstr_p = NULL;
    bkc_string_t *dstr_p = NULL;
    int cmp = 0;
    
    dstr_p = bkc_string_create("abcdefghijklmnopqrstuvwxyz");
    bkc_string_prtinfo(dstr_p);

    sstr_p = bkc_string_create("1234567890");
    bkc_string_prtinfo(sstr_p);

    cmp = bkc_string_cmp(dstr_p, sstr_p);

    printf("cmp result is %s\n", cmp == 0 ? "same\n" :
        "not same\n");

    printf("change sstr_p\n");
    bkc_string_chg(dstr_p, "1234567890");
    bkc_string_prtinfo(dstr_p);
    cmp = bkc_string_cmp(dstr_p, sstr_p);

    printf("after changing cmp result is %s\n", cmp == 0 ? "same\n" :
        "not same\n");

    bkc_string_destroy(sstr_p);
    bkc_string_destroy(dstr_p);

#ifdef BKC_STRING_T_WATCHM
    bkc_string_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_string_t_isempty
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test bkc_string_isempty
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_string_t_isempty(void)
{
    bkc_string_t *str_p = NULL;

    int isemtpy = 0;
    
    str_p = bkc_string_create("abcdefghijklmnopqrstuvwxyz");
    bkc_string_prtinfo(str_p);

    isemtpy = bkc_string_isempty(str_p);

    printf("judge result is %s\n", isemtpy == 1 ? "empty\n" :
        "not empty\n");

    printf("change str_p\n");
    bkc_string_chg(str_p, NULL);
    bkc_string_prtinfo(str_p);
    isemtpy = bkc_string_isempty(str_p);

    printf("after changing judge result is %s\n", isemtpy == 1 ? "empty\n" :
        "not empty\n");

    bkc_string_destroy(str_p);

#ifdef BKC_STRING_T_WATCHM
    bkc_string_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_string_t_cpy
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test bkc_string_cpy
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_string_t_cpy(void)
{
    bkc_string_t *sstr_p = NULL;
    bkc_string_t *dstr_p = NULL;

    sstr_p = bkc_string_create("    abcdefghijklmnopqrstuvwxyz    ");
    bkc_string_prtinfo(sstr_p);

    dstr_p = bkc_string_create(NULL);
    bkc_string_prtinfo(dstr_p);


    printf("test cpy\n");
    bkc_string_cpy(dstr_p, sstr_p);
    bkc_string_prtinfo(dstr_p);

    bkc_string_destroy(sstr_p);
    bkc_string_destroy(dstr_p);

#ifdef BKC_STRING_T_WATCHM
    bkc_string_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  main
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 *****************************  Notes  *****************************
 ************************************************************************/
int main(int arc, char *argv[])
{
#if 0
    bkc_string_t_cd();
#endif

#if 0
    bkc_string_t_appd();
#endif

#if 0
    bkc_string_t_chglen(); 
#endif

#if 0
    bkc_string_t_trim();
#endif

#if 0
    bkc_string_t_cpy();
#endif

#if 0
    bkc_string_t_dup();
#endif

#if 0
    bkc_string_t_gettext();
#endif

#if 0
    bkc_string_t_cmp();
#endif

#if 1
    bkc_string_t_isempty();
#endif
    return 0;
}
