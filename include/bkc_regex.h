/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_regex
 *  file name: bkc_regex.h
 *  description: the basic string facility supplied in bkc
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
#ifndef BKC_REGEX_H
#define BKC_REGEX_H

/*********************included files*************************************/
#include "bkc_regex_def.h"
#include "bkc_regex_spat.h"
#include "bkc_regex_igs.h"
#include "bkc_regex_mac.h"

/*********************definition, types & constants ***********************/
/*types forward declaration in bkc_regex module*/

/*********************prototype of open functions************************/
/*************************************************************************
 *                  bkc_regex_watchmm
 *************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_REGEX_RSUC: sucessfull return
 *     BKC_REGEX_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_REGEX_SPF_RELEASE is not
 *     defined in bkc_regex_spf.h) this func will always show empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_regex_watchmm(void);

#endif

