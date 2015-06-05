/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_regex
 *  file name: bkc_regex_mac.c
 *  description: the search machine component in bkc_regex module
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
#include "bkc_regex_mac.h"
#include "bkc_regex_mac_def.h"
#include "bkc_regex_spf.h"
#include <stdlib.h>

/*********************definition, types & constants *********************/

/*********************variables*******************************************/

/*********************prototype of local functions************************/
static int bkc_regex_mac_startsch(bkc_regex_mac_t *mac_p);

/*********************implementation of open and local functions**********/
/**************************************************************************
 *                  bkc_regex_mac_create
 **************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes ***********************************
 *************************************************************************/
bkc_regex_mac_t *bkc_regex_mac_create(bkc_regex_spat_t *spat_p, char *igs_p)
{
    BKC_DITT(BKC_REGEX_RSUC, 0);
    bkc_regex_mac_t *mac_p = NULL;

    BKC_FLOWCONTROL_TRY {
        mac_p = BKC_MALLOC(sizeof(bkc_regex_mac_t));
        BKC_FC_CT(mac_p != NULL, 1, BKC_REGEX_RERR, NULL);

        /*check and get a appropriate warning*/
        BKC_CKPRT(spat_p != NULL && igs_p != NULL);
        mac_p->spat_p = spat_p;
        mac_p->igs_p = igs_p;
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            spat_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_CKPRET(mac_p != NULL);
    BKC_RETURN(mac_p);
}

/**************************************************************************
 *                  bkc_regex_mac_destroy
 **************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_mac_destroy(bkc_regex_mac_t *mac_p)
{
    BKC_DITT(BKC_REGEX_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(mac_p != NULL, 1, BKC_REGEX_RERR, NULL);

        /*the mac_p->spat_p and mac_p->igs_p are left intact*/
        BKC_SFREE(mac_p);
    }

    BKC_RETURN(BKC_FNRET);
}

/**************************************************************************
 *                  bkc_regex_mac_cmd
 **************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_mac_cmd(bkc_regex_mac_t *mac_p, bkc_regex_mac_cmd_set_m cmd,
        void *args_p)
{
    BKC_DITT(BKC_REGEX_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(mac_p != NULL &&
            (cmd > BKC_REGEX_MAC_CMD_EMPTY && cmd < BKC_REGEX_MAC_CMD_LIMIT),
            1, BKC_REGEX_RERR, NULL);

        switch (cmd) {
        case BKC_REGEX_MAC_CMD_CHGSPAT:
            BKC_CKPRT(args_p != NULL);
            mac_p->spat_p = args_p;
            break;
        case BKC_REGEX_MAC_CMD_CHGIGS:
            BKC_CKPRT(args_p != NULL);
            mac_p->igs_p = args_p;
            break;
        case BKC_REGEX_MAC_CMD_STARTSCH:
            BKC_CR = bkc_regex_mac_startsch(mac_p);
            BKC_FC_CT(BKC_CR == BKC_STRING_RET_SUC, 1,
                BKC_REGEX_RERR, "destroy error");
            break;
        default:
            BKC_FC_ERRTHR("err cmd\n", 1, BKC_REGEX_RERR);
            break;
        }
        BKC_FC_RT();
    }

    BKC_RETURN(BKC_FNRET);
}

/**************************************************************************
 *                  bkc_regex_mac_startsch
 **************************************************************************
 * parameters:
 *     mac_p:
 *       points to the search-machine that need to be destroyed
 * return value:
 *     BKC_REGEX_RSUC: successfull return
 *     BKC_REGEX_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     start the serach-machine which created by bkc_regex_mac_create
 *
 *     mac_p can not be NULL. if mac_p == NULL, return value will be
 *     BKC_REGEX_RERR, and a error msg will be prompted
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_mac_startsch(bkc_regex_mac_t *mac_p)
{
    BKC_DITT(BKC_REGEX_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int limit = 0;
        int loop = 0;
        char sch = -1;
        char dch = -1;
        int dpos = -1;
        int patlen = 0;

        BKC_FC_CT(mac_p != NULL, 1, BKC_REGEX_RERR, NULL);

        BKC_FC_CT(BKC_RF(BKC_RF(mac_p)->spat_p)->pat_str_p != NULL
            && BKC_RF(BKC_RF(mac_p)->igs_p)->str_p != NULL,
            1, BKC_REGEX_RERR, "pt ck err\n");

        limit = bkc_string_getlen(mac_p->igs_p->str_p);
        patlen = bkc_string_getlen(mac_p->spat_p->pat_str_p);
        BKC_FC_CT(limit >= 0 && patlen >= 0, 1,
            BKC_REGEX_RERR, "limit ck err\n");

        /*def init the search pattern*/
        BKC_CR = bkc_regex_spat_definit(mac_p->spat_p);
        BKC_FC_CT(BKC_CR == BKC_REGEX_RSUC, 1, BKC_REGEX_RERR, NULL);

        for (loop = mac_p->igs_p->start_pos, dpos = mac_p->spat_p->cur_pos;
            loop < limit && (mac_p->spat_p->is_pat_matched) == 0; loop++) {

            sch = bkc_string_getnchar(mac_p->igs_p->str_p, loop);
            dch = bkc_string_getnchar(mac_p->spat_p->pat_str_p, dpos);
            BKC_FC_CT(sch >= 0 && dch >= 0, 1, BKC_REGEX_RERR, NULL);

            if (sch == dch) {
                /*matched*/
                BKC_CR = bkc_string_appdchar(
                    mac_p->spat_p->mat_ret_p, sch);
                BKC_FC_CT(BKC_CR == BKC_STRING_RET_SUC, 1, BKC_REGEX_RERR,
                    "appd err\n");
                dpos++;
            } else {
                /*not matched, retreat the dpos*/
                BKC_CR = bkc_regex_spat_definit(mac_p->spat_p);
                BKC_FC_CT(BKC_CR == BKC_REGEX_RSUC, 1, BKC_REGEX_RERR,
                    NULL);
                dpos = mac_p->spat_p->cur_pos;
            }

            if (dpos >= patlen) {
                mac_p->spat_p->is_pat_matched = 1;
                mac_p->spat_p->cur_pos = dpos;
                break;
            }
        }
        BKC_FC_RT();
    }

    BKC_RETURN(BKC_FNRET);
}
