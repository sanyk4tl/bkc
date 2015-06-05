/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_store
 *  file name: bkc_store.c
 *  description: the bkc_store facility supplied in bkc
 *****************************************************************************
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
 ****************************************************************************/
/*********************included files*****************************************/
#include <stdlib.h>
#include <stdio.h>
#include "bkc_store.h"
#include "bkc_store_spf.h"

/*********************definition, types & constants *************************/

/*********************variables**********************************************/

/*********************prototype of local functions***************************/

/*********************implementation of open and local functions*************/

/*****************************************************************************
 *                  bkc_store_watchmm
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_STORE_RSUC: sucessfull return
 *     BKC_STORE_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func can show the memory infomation about this module
 *     is in debugging mode in which BKC_STORE_SPF_RELEASE is not
 *     defined in bkc_store_spf.h. if BKC_STORE_SPF_RELEASE is defined
 *     in bkc_store_spf.h, this func will always show empty messages
 *
 *     this function is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module. in this case
 *     this func can be temporarily export to bkc_store.h for outer
 *     refering
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_store_watchmm(void)
{
    BKC_DITT(BKC_STORE_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_CR = bkc_store_spf_watchmm();
        BKC_FC_CT(BKC_CR == BKC_STORE_RSUC, 1, BKC_STORE_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}
