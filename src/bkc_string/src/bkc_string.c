/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_string
 *  file name: bkc_string.c
 *  description: the string facility supplied in bkc
 *************************************************************************
 *  log:
 *  >>>
 *  version:
 *      v1.0.1-v1.0.0
 *  time:
 *      Monday, May 18, 2009 at 12:35:01 AM AM UTC/GMT +8 hours
 *  location:
 *      Beijing, China on Earth
 *      latitude: 39.55 North, longtitude: 116.23 East
 *      international country code: + 86 (China)
 *      area code: 10
 *  weather:
 *      clear, mild
 *      temperature: 82 F (comfort level is 81 F)
 *      wind: 1 mph from 0 North
 *
 *  pgr_id: sizhe(email:tancng#gmail.com)
 *
 *  description: created
 ************************************************************************/
/*********************included files*************************************/
#include <stdlib.h>
#include <stdio.h>
#include "bkc_string.h"
#include "bkc_string_spf.h"

/*********************definition, types & constants ***********************/
#define BKC_STRING_INTSTR_LEN    20
#define BKC_STRING_CHARSTR_LEN    8
/*used for expanding a store len of the string*/
#define BKC_STRING_EXP_AUX_LEN   50

/*bkc_string*/
struct bkc_string_tag_t {
    char *txtarr_p;
    /*lenth of the string*/
    unsigned int len;
    /*the memory in bytes occupied by this string*/
    unsigned int storelen;
};

/*trim mode of a string*/
typedef enum bkc_string_expmode_e {
    BKC_STRING_EXP_EMPTY,
    /*expand the string with auxliary store*/
    BKC_STRING_EXP_AUX,
    /*expand the string without auxliary store*/
    BKC_STRING_EXP_NOAUX
} bkc_string_expmode_m;

/*********************variables******************************************/

/*********************prototype of local functions***********************/
static int bkc_string_expstore(bkc_string_t *str_p, unsigned int newlen,
        bkc_string_expmode_m mode);
static char *bkc_string_dupint(int count);
static int bkc_string_trcont(bkc_string_t *str_p, unsigned int newlen);
static int bkc_string_trimleft(bkc_string_t *str_p);
static int bkc_string_trimright(bkc_string_t *str_p);
static int bkc_string_trimlr(bkc_string_t *str_p);
static int bkc_string_trimispace(bkc_string_t *str_p);
static int bkc_string_watchmm(void);

/*********************implementation of open and local functions*********/
/*************************************************************************
 *                  bkc_string_create
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
bkc_string_t *bkc_string_create(char *content_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);
    bkc_string_t *str_p = NULL;

    BKC_FLOWCONTROL_TRY {
        str_p = BKC_MALLOC(sizeof(bkc_string_t));
        BKC_FC_CKTHROW(str_p != NULL, 1, BKC_STRING_RET_ERR);

        /*init the string member defautly*/
        BKC_RF(str_p)->len = 0;
        BKC_RF(str_p)->storelen = 0;
        BKC_RF(str_p)->txtarr_p = NULL;

        if (content_p != NULL) {
            BKC_RF(str_p)->txtarr_p = BKC_STRDUP(content_p);
            BKC_FC_CKTHROW(BKC_RF(str_p)->txtarr_p != NULL,
                2, BKC_STRING_RET_ERR);
            BKC_RF(str_p)->len = BKC_STRLEN(BKC_RF(str_p)->txtarr_p);
            /*'\0' must be calculated*/
            BKC_RF(str_p)->storelen = BKC_RF(str_p)->len + 1;
        }
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            str_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_SFREE(str_p);
            str_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_CKPRET(str_p != NULL);
    BKC_RETURN(str_p);
}

/*************************************************************************
 *                  bkc_string_destroy
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_destroy(bkc_string_t *str_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CKTHROW(str_p != NULL, 1, BKC_STRING_RET_ERR);

        /*uninit the string member*/
        if (BKC_RF(str_p)->txtarr_p != NULL) {
            BKC_SFREE(BKC_RF(str_p)->txtarr_p);
        }
        /*free the string*/
        BKC_SFREE(str_p);
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_prtinfo
 *************************************************************************
 * parameters:
 *     str_p: points to the string
 * return value:
 *     BKC_STRING_RET_SUC: successfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func can show the detailed infomation about this string
 *
 *     its usefull when take a white-box testing in this
 *     module for showing string info. in this case this func can be
 *     temporarily export to bkc_string.h for outer refering
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_prtinfo(bkc_string_t *str_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        if (str_p != NULL) {
            BKC_PRT("\n");
            BKC_PRT("####################################\n");
            BKC_PRT("str_p->txtarr_p is %s\n",
                BKC_GETVSTR(BKC_RF(str_p)->txtarr_p));
            BKC_PRT("str_p->len is %d\n", BKC_RF(str_p)->len);
            BKC_PRT("str_p->storelen is %d\n", BKC_RF(str_p)->storelen);
            BKC_PRT("####################################\n");
            BKC_PRT("\n");
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_dupint
 *************************************************************************
 * parameters:
 *     count: the int value which need to be changed to str
 * return value:
 *     Not NULL: points to the successfully created string
 *     NULL: abnormal return
 * description:
 *     translate the format of count from int into char*
 *****************************  Notes  *****************************
 ************************************************************************/
char *bkc_string_dupint(int count)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);
    char *retstr_p = NULL;

    BKC_FLOWCONTROL_TRY {
        char strarr[BKC_STRING_INTSTR_LEN + 1] = {0};
        BKC_MEMSET(strarr, '\0', BKC_STRING_INTSTR_LEN + 1);
        BKC_SNPRINTF(strarr, BKC_STRING_INTSTR_LEN, "%d", count);
        retstr_p = BKC_STRDUP(strarr);
        BKC_FC_CKTHROW(retstr_p != NULL, 1, BKC_STRING_RET_ERR);
    }

    BKC_CKPRET(retstr_p != NULL);
    BKC_RETURN(retstr_p);
}

/*************************************************************************
 *                  bkc_string_appd
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_appd(bkc_string_t *dstr_p, bkc_string_t *sstr_p,
        char *stext_p, char ch)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);
    int tlen = 0;

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CKTHROW(dstr_p != NULL, 1, BKC_STRING_RET_ERR);
        tlen = BKC_RF(dstr_p)->len;

        if (sstr_p != NULL && BKC_RF(sstr_p)->len > 0) {
            tlen += BKC_RF(sstr_p)->len;
        }
        if (stext_p != NULL) {
            tlen += BKC_STRLEN(stext_p);
        }
        if (ch != -1) {
            tlen += sizeof(ch);
        }

        BKC_CKRET = bkc_string_expstore(dstr_p, tlen + 1,
            BKC_STRING_EXP_NOAUX);
        BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
            BKC_STRING_RET_ERR);

        if (sstr_p != NULL && BKC_RF(sstr_p)->len > 0) {
            BKC_STRNCAT(BKC_RF(dstr_p)->txtarr_p,
                BKC_RF(sstr_p)->txtarr_p, BKC_RF(sstr_p)->len);
        }
        if (stext_p != NULL) {
            BKC_STRNCAT(BKC_RF(dstr_p)->txtarr_p,
                stext_p, BKC_STRLEN(stext_p));
        }
        if (ch != -1) {
            BKC_RF(dstr_p)->txtarr_p[tlen - 1] = ch;
        }

        BKC_RF(dstr_p)->len = tlen;
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_appdchar
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_appdchar(bkc_string_t *dstr_p, char ch)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CKTHROW(dstr_p != NULL && ch != -1, 1,
            BKC_STRING_RET_ERR);

        BKC_CKRET = bkc_string_appd(dstr_p, NULL, NULL, ch);
        BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC,
            1, BKC_STRING_RET_ERR);
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_insnchar
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_insnchar(bkc_string_t *str_p, int pos, char ch)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        int loop = -1;
        int len = -1;
        BKC_FC_CKTHROW(str_p != NULL && ch != -1, 1,
            BKC_STRING_RET_ERR);
        BKC_FC_CKTHROW(pos >= 0 && pos < str_p->len, 1,
            BKC_STRING_RET_ERR);

        len = str_p->len;
        BKC_CKRET = bkc_string_expstore(str_p, len + 1,
            BKC_STRING_EXP_NOAUX);
        BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
            BKC_STRING_RET_ERR);

        for (loop = len; loop > pos; loop--) {
            str_p->txtarr_p[loop] = str_p->txtarr_p[loop - 1];
        }
        BKC_FC_RLYTHROW();

        str_p->txtarr_p[pos] = ch;
        /*end the string definitely*/
        str_p->txtarr_p[len + 1] = '0';
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_getnchar
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  notes  *****************************
 ************************************************************************/
char bkc_string_getnchar(bkc_string_t *dstr_p, int pos)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    char ch = -1;

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CKTHROW(dstr_p != NULL, 1,
            BKC_STRING_RET_ERR);
        BKC_FC_CKTHROW(pos >= 0 && pos < dstr_p->len, 1,
            BKC_STRING_RET_ERR);
        ch = dstr_p->txtarr_p[pos];
    }

    BKC_CKPRET(ch >= 0);
    BKC_RETURN(ch);
}

/*************************************************************************
 *                  bkc_string_appdint
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_appdint(bkc_string_t *dstr_p, int num)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);
    int tlen = 0;
    char *intstr_p = NULL;

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CKTHROW(dstr_p != NULL, 1, BKC_STRING_RET_ERR);
        tlen = BKC_RF(dstr_p)->len;

        intstr_p = bkc_string_dupint(num);
        BKC_FC_CKTHROW(intstr_p != NULL, 1, BKC_STRING_RET_ERR);
        tlen += BKC_STRLEN(intstr_p);

        BKC_CKRET = bkc_string_expstore(dstr_p, tlen + 1,
            BKC_STRING_EXP_NOAUX);
        BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 2,
            BKC_STRING_RET_ERR);

        BKC_STRNCAT(BKC_RF(dstr_p)->txtarr_p,
            intstr_p, BKC_STRLEN(intstr_p));
        BKC_SFREE(intstr_p);

        BKC_RF(dstr_p)->len = tlen;
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_SFREE(intstr_p);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_chg
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_chg(bkc_string_t *str_p, char *txt_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);
    int itxtlen = 0;

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CKTHROW(str_p != NULL, 1, BKC_STRING_RET_ERR);

        if (txt_p != NULL) {
            itxtlen = BKC_STRLEN(txt_p);
            BKC_CKRET = bkc_string_expstore(str_p, itxtlen + 1,
                BKC_STRING_EXP_NOAUX);
            BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
                BKC_STRING_RET_ERR);
            BKC_MEMSET(BKC_RF(str_p)->txtarr_p, '\0',
                BKC_RF(str_p)->storelen);
            BKC_STRNCPY(BKC_RF(str_p)->txtarr_p, txt_p, itxtlen);
            BKC_RF(str_p)->len = itxtlen;
        } else {
            BKC_MEMSET(BKC_RF(str_p)->txtarr_p, '\0',
                BKC_RF(str_p)->storelen);
            BKC_RF(str_p)->len = 0;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_trim
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_trim(bkc_string_t *str_p, bkc_string_trimmode_m mode,
        unsigned int newlen)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CKTHROW(str_p != NULL, 1, BKC_STRING_RET_ERR);

        switch (mode) {
        case BKC_STRING_TRIMLSPACE:
            BKC_CKRET = bkc_string_trimleft(str_p);
            BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
                BKC_STRING_RET_ERR);
            break;
        case BKC_STRING_TRIMRSPACE:
            BKC_CKRET = bkc_string_trimright(str_p);
            BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
                BKC_STRING_RET_ERR);
            break;
        case BKC_STRING_TRIMLRSPACE:
            BKC_CKRET = bkc_string_trimlr(str_p);
            BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
                BKC_STRING_RET_ERR);
            break;
        case BKC_STRING_TRIMCSPACE:
            BKC_CKRET = bkc_string_trimispace(str_p);
            BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
                BKC_STRING_RET_ERR);
            break;
        case BKC_STRING_TRIMLEN:
            BKC_CKRET = bkc_string_trcont(str_p, newlen);
            BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
                BKC_STRING_RET_ERR);
            break;
        default:
            BKC_ERR("no supported mode, please have a check\n");
            break;
        }
        BKC_FC_RLYTHROW();
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_trimleft
 *************************************************************************
 * parameters:
 *     str_p: points to the string which need to be trimmed
 * return value:
 *     BKC_STRING_RET_SUC: sucessfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     trim the left spaces in the str_p
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_trimleft(bkc_string_t *str_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        /*length of beginning white spaces*/
        unsigned int wslen = 0;
        unsigned int len = 0;
        BKC_FC_CKTHROW(str_p != NULL, 1, BKC_STRING_RET_ERR);

        len = BKC_RF(str_p)->len;
        for (wslen = 0; wslen < len; wslen++) {
            if (BKC_RF(str_p)->txtarr_p[wslen] != ' ') {
                break;
            }
        }
        BKC_FC_RLYTHROW();

        /*there really exists some white-spaces on the left end*/
        if (wslen != 0) {
            unsigned int trims = 0;
            unsigned int loop = 0;
            for (trims = 0, loop = wslen; loop < len; loop++, trims++) {
                BKC_RF(str_p)->txtarr_p[trims] =
                    BKC_RF(str_p)->txtarr_p[loop];
            }
            BKC_FC_RLYTHROW();

            /*erase the tail characters*/
            for (; trims < len; trims++) {
                BKC_RF(str_p)->txtarr_p[trims] = '\0';
            }
            BKC_FC_RLYTHROW();

            BKC_ASTRET(wslen <= BKC_RF(str_p)->len,
                1, BKC_STRING_RET_ERR);
            BKC_RF(str_p)->len -= wslen;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_trimlr
 *************************************************************************
 * parameters:
 *     str_p: points to the string which need to be trimmed
 * return value:
 *     BKC_STRING_RET_SUC: sucessfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     trim the left and right spaces in the str_p
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_trimlr(bkc_string_t *str_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_CKRET = bkc_string_trimleft(str_p);
        BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
            BKC_STRING_RET_ERR);
        BKC_CKRET = bkc_string_trimright(str_p);
        BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
            BKC_STRING_RET_ERR);
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_trimispace
 *************************************************************************
 * parameters:
 *     str_p: points to the string which need to be trimmed
 * return value:
 *     BKC_STRING_RET_SUC: sucessfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     trim the intermediate spaces in the str_p.
 *     the head and tail whitespaces are also trimmed
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_trimispace(bkc_string_t *str_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        /*position of the found char*/
        unsigned int fpos = 0;
        /*position to which the found char will be set*/
        unsigned int spos = 0;
        /*counter of white spaces*/
        unsigned int wscount = 0;
        unsigned int len = 0;
        BKC_FC_CKTHROW(str_p != NULL, 1, BKC_STRING_RET_ERR);

        len = BKC_RF(str_p)->len;
        for (fpos = 0, spos = 0; fpos < len; fpos++) {
            if (BKC_RF(str_p)->txtarr_p[fpos] != ' ') {
                BKC_ASTRET(wscount <= BKC_RF(str_p)->len, 1,
                    BKC_STRING_RET_ERR);
                /*add the whitespace*/
                if (wscount != 0 && spos != 0) {
                    BKC_RF(str_p)->txtarr_p[spos] = ' ';
                    spos++;
                    BKC_RF(str_p)->len -= (wscount - 1);
                    wscount = 0;
                } else if (wscount != 0) {
                    BKC_RF(str_p)->len -= wscount;
                    wscount = 0;
                }

                /*add the non-whitespace char*/
                if (fpos != spos) {
                    BKC_RF(str_p)->txtarr_p[spos] =
                        BKC_RF(str_p)->txtarr_p[fpos];
                    spos++;
                } else {
                    spos++;
                }

            } else {
                wscount++;
            }
        }

        /*erase the tail characters*/
        for (fpos = BKC_RF(str_p)->len; fpos < len; fpos++) {
            BKC_RF(str_p)->txtarr_p[fpos] = '\0';
        }
        BKC_FC_RLYTHROW();
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_trimright
 *************************************************************************
 * parameters:
 *     str_p: points to the string which need to be trimmed
 * return value:
 *     BKC_STRING_RET_SUC: sucessfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     trim the right spaces in the str_p
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_trimright(bkc_string_t *str_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        /*length of beginning white spaces*/
        unsigned int wslen = 0;
        unsigned int len = 0;
        BKC_FC_CKTHROW(str_p != NULL, 1, BKC_STRING_RET_ERR);

        len = BKC_RF(str_p)->len;
        for (wslen = 0; wslen < len; wslen++) {
            if (BKC_RF(str_p)->txtarr_p[len - wslen - 1] != ' ') {
                break;
            }
        }
        BKC_FC_RLYTHROW();

        /*there really exists some white-spaces on the right end*/
        if (wslen != 0) {
            unsigned int trims = 0;
            /*this is a must condition when loop != 0*/
            BKC_ASTRET(len >= wslen, 1, BKC_STRING_RET_ERR);
            /*erase the right white-space characters*/
            for (trims = len - wslen ; trims < len; trims++) {
                BKC_RF(str_p)->txtarr_p[trims] = '\0';
            }
            BKC_FC_RLYTHROW();

            BKC_ASTRET(wslen <= BKC_RF(str_p)->len,
                1, BKC_STRING_RET_ERR);
            BKC_RF(str_p)->len -= wslen;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_dup
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
bkc_string_t *bkc_string_dup(bkc_string_t *str_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);
    bkc_string_t *nstr_p = NULL;

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CKTHROW(str_p != NULL, 1, BKC_STRING_RET_ERR);
        nstr_p = bkc_string_create(NULL);
        BKC_FC_CKTHROW(nstr_p != NULL, 1, BKC_STRING_RET_ERR);

        BKC_CKRET = bkc_string_cpy(nstr_p, str_p);
        BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 2,
            BKC_STRING_RET_ERR);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            nstr_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_CKRET = bkc_string_destroy(nstr_p);
            BKC_STRING_SPF_CKSRET(BKC_CKRET == BKC_STRING_RET_SUC,
                BKC_STRING_RET_ERR);
            nstr_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_CKPRET(nstr_p != NULL);
    BKC_RETURN(nstr_p);
}

/*************************************************************************
 *                  bkc_string_dupspec
 *************************************************************************
 * parameters:
 *     str_p:
 *         points to the string which will be duplicated
 *     spos:
 *         the start position in str_p for the string that will
 *         be duplicated and returned
 *     len_p:
 *         this is a value-result argument first be inputed as the the len
 *         of the string which will be duplicated in sstr_p, and will be
 *         finally stuffed with length thar really been duplicated
 *
 *         if first initialed with -1, then the real length covers the
 *         range of [spos, bkc_string_getlen(str_p)-1]
 * return value:
 *     Not NULL: points to the sucessfully duplicated string
 *     NULL: abnormal return
 * description:
 *     create a new string of which the conten designated by
 *     the start points of spos refered to the content in str_p
 *     and  the length of len.
 *     str_p can not be NULL. if str_p == NULL, return value
 *     will be NULL and will get an error msg
 *****************************  notes  *****************************
 ************************************************************************/
bkc_string_t *bkc_string_dupspec(bkc_string_t *str_p, int spos, int *len_p)
{

}

/*************************************************************************
 *                  bkc_string_cpy
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_cpy(bkc_string_t *dstr_p, bkc_string_t *sstr_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);
    bkc_string_t *nstr_p = NULL;

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CKTHROW(dstr_p != NULL && sstr_p != NULL,
            1, BKC_STRING_RET_ERR);
        /*clear the dstr_p*/
        BKC_CKRET = bkc_string_trcont(dstr_p, 0);
        BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
            BKC_STRING_RET_ERR);
        /*expand the dstr_p*/
        BKC_CKRET = bkc_string_expstore(dstr_p, BKC_RF(sstr_p)->storelen,
            BKC_STRING_EXP_NOAUX);
        BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC, 1,
            BKC_STRING_RET_ERR);

        /*must condition checking points*/
        BKC_ASTRET(BKC_RF(dstr_p)->storelen >= BKC_RF(sstr_p)->storelen,
            1, BKC_STRING_RET_ERR);

        BKC_STRNCPY(BKC_RF(dstr_p)->txtarr_p, BKC_RF(sstr_p)->txtarr_p,
            BKC_RF(sstr_p)->len);
        BKC_RF(dstr_p)->len = BKC_RF(sstr_p)->len;
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_gettext
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_gettext(bkc_string_t *str_p, char **rstr_pp,
            bkc_string_gtextmfunc_t mfunc)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        char *rtext_p = NULL;

        BKC_FC_CKTHROW(str_p != NULL && rstr_pp != NULL
            && mfunc != NULL, 1, BKC_STRING_RET_ERR);

        if (BKC_RF(str_p)->txtarr_p != NULL && BKC_RF(str_p)->len != 0) {
            rtext_p = mfunc(BKC_RF(str_p)->len + 1);
            BKC_FC_CKTHROW(rtext_p != NULL , 1, BKC_STRING_RET_ERR);
            BKC_MEMSET(rtext_p, '\0', BKC_RF(str_p)->len + 1);
            BKC_STRNCPY(rtext_p, BKC_RF(str_p)->txtarr_p,
                BKC_RF(str_p)->len);
        }

        *(BKC_RF(rstr_pp)) = rtext_p;
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_cmp
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_cmp(bkc_string_t *dstr_p, bkc_string_t *sstr_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    int cmp = -1;
    BKC_FLOWCONTROL_TRY {
        if (dstr_p != NULL && sstr_p != NULL) {
            cmp = BKC_STRCMP(BKC_RF(dstr_p)->txtarr_p,
                BKC_RF(sstr_p)->txtarr_p);
        } else if (dstr_p == NULL && sstr_p == NULL) {
            cmp = 0;
        } else {
            cmp = -1;
        }
    }

    BKC_RETURN(cmp);
}

/*************************************************************************
 *                  bkc_string_isempty
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_isempty(bkc_string_t *str_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    int ret = 0;
    BKC_FLOWCONTROL_TRY {
        if (str_p == NULL
            || (BKC_RF(str_p)->len == 0
                && BKC_RF(str_p)->txtarr_p[0] == '\0')) {
            ret = 1;
        }
    }

    BKC_RETURN(ret);
}

/*************************************************************************
 *                  bkc_string_getlen
 *************************************************************************
 * parameters:
 *     str_p:
 *       points to the string which need to get its lenth
 * return value:
 *     (large than or equal 0):the length of the string count in
 *          characters(ascii)
 *     (other): abnormal return
 * description:
 *     get the string length
 *     test in bkc_string/bkc_string_test.c:
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_getlen(bkc_string_t *str_p)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);
    int slen = 0;

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CKTHROW(str_p != NULL, 1, BKC_STRING_RET_ERR);
        slen = BKC_RF(str_p)->len;
        BKC_ASTRET(slen >= 0 && slen < BKC_RF(str_p)->storelen,
            1, BKC_STRING_RET_ERR);
    }

    BKC_RETURN(slen);
}

/*************************************************************************
 *                  bkc_string_trcont
 *************************************************************************
 * parameters:
 *     str_p: points to the string which need to be truncated
 *     newlen: the designated length of the text content
 * return value:
 *     BKC_STRING_RET_SUC: sucessfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     truncate have two impact:
 *     if newlen is large than the lenth of the content text in the
 *     string, then nothing  will be applied to the string
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_trcont(bkc_string_t *str_p, unsigned int newlen)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        int loop = 0;
        int scanlen = 0;
        BKC_FC_CKTHROW(str_p != NULL, 1, BKC_STRING_RET_ERR);

        if (BKC_RF(str_p)->len > newlen) {
            scanlen = BKC_RF(str_p)->len - newlen;
            BKC_SETARR(BKC_RF(str_p)->txtarr_p, loop, newlen,
                scanlen, '\0');
            BKC_ASTRET(scanlen > 0 && (loop == BKC_RF(str_p)->len),
                1, BKC_STRING_RET_ERR);
            BKC_RF(str_p)->len = newlen;
        } else {
            BKC_PRT("BKC_RF(str_p)->len <= newlen,"
                "and need not to truncate its content\n");
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_expstore
 *************************************************************************
 * parameters:
 *     str_p: points to the string which need to be expanded
 *         its memory store
 *     newlen: the designated length of the text content
 * return value:
 *     BKC_STRING_RET_SUC: sucessfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     expand the storelen of the str_p, and after calling this func,
 *     the storelen will be never be less than newlen.
 *     the original content of the string will be left unchanged.
 *     if newlen is less than the string store len, nothing will
 *     be applied to the string
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_expstore(bkc_string_t *str_p, unsigned int newlen,
        bkc_string_expmode_m mode)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        char *oritxt_p = NULL;
        int storelen = 0;
        BKC_FC_CKTHROW(str_p != NULL, 1, BKC_STRING_RET_ERR);

        if (BKC_RF(str_p)->storelen < newlen) {

            if (mode == BKC_STRING_EXP_AUX)
                storelen = newlen + BKC_STRING_EXP_AUX_LEN;
            else if (mode == BKC_STRING_EXP_NOAUX)
                storelen = newlen;
            else {
                BKC_PRT("not supported mode in bkc_string_expstore"
                    "please have a check\n");
            }

            if (BKC_RF(str_p)->txtarr_p != NULL) {
                oritxt_p = BKC_STRDUP(BKC_RF(str_p)->txtarr_p);
                BKC_FC_CKTHROW(oritxt_p != NULL, 1, BKC_STRING_RET_ERR);

                BKC_SFREE(BKC_RF(str_p)->txtarr_p);
                BKC_RF(str_p)->txtarr_p = BKC_MALLOC(storelen);
                BKC_MEMSET(BKC_RF(str_p)->txtarr_p, '\0', storelen);
                BKC_STRNCPY(BKC_RF(str_p)->txtarr_p, oritxt_p, storelen - 1);

                /*text len is unchanged, just update the storelen*/
                BKC_RF(str_p)->storelen = storelen;
                BKC_SFREE(oritxt_p);
            } else {
                BKC_RF(str_p)->txtarr_p = BKC_MALLOC(storelen);
                BKC_MEMSET(BKC_RF(str_p)->txtarr_p, '\0', storelen);
                BKC_RF(str_p)->storelen = storelen;
            }
        } else {
            BKC_PRT("BKC_RF(str_p)->storelen >= newlen,"
                "and need not to expand\n");
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_string_watchmm
 *************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_STRING_RET_SUC: sucessfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func can show the memory infomation about this module
 *     is in debugging mode in which BKC_STRING_SPF_RELEASE is not
 *     defined in bkc_string_spf.h. if BKC_STRING_SPF_RELEASE is defined
 *     in bkc_string_spf.h, this func will always show empty messages
 *
 *     this function is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module. in this case
 *     this func can be temporarily export to bkc_string.h for outer
 *     refering
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_string_watchmm(void)
{
    BKC_DITT(BKC_STRING_RET_SUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_CKRET = bkc_string_spf_watchmm();
        BKC_FC_CKTHROW(BKC_CKRET == BKC_STRING_RET_SUC,
            1, BKC_STRING_RET_ERR);
    }

    BKC_RETURN(BKC_FNRET);
}
