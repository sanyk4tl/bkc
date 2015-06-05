/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_regex
 *  file name: bkc_regex_igs.c
 *  description: the ignition string component in bkc_regex module
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
#include "bkc_regex_igs.h"
#include "bkc_regex_igs_def.h"
#include "bkc_regex_spf.h"
#include <stdlib.h>
/*********************definition, types & constants ***********************/

/*********************variables******************************************/

/*********************prototype of local functions***********************/

/*********************implementation of open and local functions*********/
/**************************************************************************
 *                  bkc_regex_igs_create
 **************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes ***********************************
 *************************************************************************/
bkc_regex_igs_t *bkc_regex_igs_create(char *str_p)
{
    BKC_DITT(BKC_REGEX_RSUC, 0);
    bkc_regex_igs_t *igs_p = NULL;

    BKC_FLOWCONTROL_TRY {
        igs_p = BKC_MALLOC(sizeof(bkc_regex_igs_t));
        BKC_FC_CT(igs_p != NULL, 1, BKC_REGEX_RERR, NULL);

        BKC_CKPRT(str_p != NULL);
        igs_p->str_p = bkc_string_create(str_p);
        BKC_FC_CT(igs_p->str_p != NULL, 2, BKC_REGEX_RERR, NULL);

        igs_p->start_pos = 0;
        igs_p->cur_pos = 0;
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            igs_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_SFREE(igs_p);
            igs_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_CKPRET(igs_p != NULL);
    BKC_RETURN(igs_p);
}

/**************************************************************************
 *                  bkc_regex_igs_destroy
 **************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_igs_destroy(bkc_regex_igs_t *igs_p)
{
    BKC_DITT(BKC_REGEX_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(igs_p != NULL, 1, BKC_REGEX_RERR, NULL);

        if (igs_p->str_p != NULL) {
            BKC_CR = bkc_string_destroy(igs_p->str_p);
            BKC_FC_CT(BKC_CR == BKC_STRING_RET_SUC, 1,
                BKC_REGEX_RERR, "destroy error");
        }

        BKC_SFREE(igs_p);
    }

    BKC_RETURN(BKC_FNRET);
}
