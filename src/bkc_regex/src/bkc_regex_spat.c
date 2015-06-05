/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_regex
 *  file name: bkc_regex_spat.c
 *  description: the search pattern component in bkc_regex module
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
#include "bkc_regex_spat.h"
#include "bkc_regex_spat_def.h"
#include "bkc_regex_spf.h"
#include <stdlib.h>

/*********************definition, types & constants ***********************/

/*********************variables******************************************/

/*********************prototype of local functions***********************/

/*********************implementation of open and local functions*********/
/**************************************************************************
 *                  bkc_regex_spat_create
 **************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes ***********************************
 *************************************************************************/
bkc_regex_spat_t *bkc_regex_spat_create(char *pat_str_p)
{
    BKC_DITT(BKC_REGEX_RSUC, 0);
    bkc_regex_spat_t *spat_p = NULL;

    BKC_FLOWCONTROL_TRY {
        spat_p = BKC_MALLOC(sizeof(bkc_regex_spat_t));
        BKC_FC_CT(spat_p != NULL, 1, BKC_REGEX_RERR, NULL);

        BKC_CKPRT(pat_str_p != NULL);
        BKC_RF(spat_p)->pat_str_p = bkc_string_create(pat_str_p);
        BKC_RF(spat_p)->mat_ret_p = bkc_string_create(NULL);

        BKC_FC_CT(BKC_RF(spat_p)->pat_str_p != NULL &&
            BKC_RF(spat_p)->mat_ret_p != NULL, 2, BKC_REGEX_RERR, NULL);

        BKC_RF(spat_p)->cur_pos = 0;
        BKC_RF(spat_p)->is_pat_matched = 0;
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            spat_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_SFREE(spat_p);
            spat_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_CKPRET(spat_p != NULL);
    BKC_RETURN(spat_p);
}

/**************************************************************************
 *                  bkc_regex_spat_destroy
 **************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_spat_destroy(bkc_regex_spat_t *spat_p)
{
    BKC_DITT(BKC_REGEX_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(spat_p != NULL, 1, BKC_REGEX_RERR, NULL);

        if (spat_p->pat_str_p != NULL) {
            BKC_CR = bkc_string_destroy(spat_p->pat_str_p);
            BKC_FC_CKTHROW(BKC_CR == BKC_STRING_RET_SUC, 1,
                BKC_REGEX_RERR, "destroy error");
        }
        if (spat_p->mat_ret_p != NULL) {
            BKC_CR = bkc_string_destroy(spat_p->mat_ret_p);
            BKC_FC_CKTHROW(BKC_CR == BKC_STRING_RET_SUC, 1,
                BKC_REGEX_RERR, "destroy error");
        }

        BKC_SFREE(spat_p);
    }

    BKC_RETURN(BKC_FNRET);
}

/**************************************************************************
 *                  bkc_regex_spat_gmret
 **************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_spat_gmret(bkc_regex_spat_t *spat_p, bkc_string_t *rstr_p)
{
    BKC_DITT(BKC_REGEX_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(spat_p != NULL && rstr_p != NULL, 1,
            BKC_REGEX_RERR, NULL);

        /*pointer checking*/
        BKC_FC_CT(BKC_RF(spat_p)->mat_ret_p != NULL, 1,
            BKC_REGEX_RERR, NULL);

        BKC_CR = bkc_string_cpy(rstr_p, spat_p->mat_ret_p);
        BKC_FC_CKTHROW(BKC_CR == BKC_STRING_RET_SUC, 1,
            BKC_REGEX_RERR, "cpy error");
    }

    BKC_RETURN(BKC_FNRET);
}

/**************************************************************************
 *                  bkc_regex_spat_definit
 **************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_spat_definit(bkc_regex_spat_t *spat_p)
{
    BKC_DITT(BKC_REGEX_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(spat_p != NULL, 1, BKC_REGEX_RERR, NULL);

        BKC_FC_CT(BKC_RF(spat_p)->mat_ret_p != NULL, 1,
            BKC_REGEX_RERR, "pt ck err\n");

        /*clear the match-result*/
        BKC_CR = bkc_string_chg(spat_p->mat_ret_p, NULL);
        BKC_FC_CT(BKC_CR == BKC_STRING_RET_SUC, 1,
            BKC_REGEX_RERR, "chg err\n");

        spat_p->cur_pos = 0;
        spat_p->is_pat_matched = 0;
    }

    BKC_RETURN(BKC_FNRET);
}
