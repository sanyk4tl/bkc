/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_mod
 *  file name: bkc_mod.h
 *  description: the basic mod facility supplied in bkc
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
#ifndef BKC_MOD_H
#define BKC_MOD_H

/*********************included files*****************************************/

/*********************definition, types & constants *************************/
/*ret value of funcs in bkc_mod*/
#define BKC_MOD_RSUC   (0)
#define BKC_MOD_RERR   (-1)

/*********************prototype of open functions****************************/
/*****************************************************************************
 *                  bkc_mod_mod_init
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_MOD_RSUC: sucessfull return
 *     BKC_MOD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     init the bkc_mod module. only after bkc_mod_mod_init(),
 *     the other functions in bkc_mod.h can be used
 *
 *     once the bkc_mod module is inited, there will be a global
 *     clock which records the current time in the format descripted in
 *     structure bkc_mod_t, and the global clock is set to be
 *     running(clocking) by default.
 *
 *     the global clock default value is set with the time span which is
 *     counted from  00:00:00 on January 1, 1970,
 *     Coordinated Universal Time (UTC), and the precision is only
 *     seconds currently, and more accurate precision such as million seconds
 *     and nano seconds will be added later
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_mod_mod_init(void);

/*****************************************************************************
 *                  bkc_mod_mod_uninit
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_MOD_RSUC: sucessfull return
 *     BKC_MOD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     uninit the bkc_mod module, thus, functions in bkc_mod.h
 *     can not be called after calling this func
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_mod_mod_uninit(void);

/*****************************************************************************
 *                  bkc_mod_watchmm
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_MOD_RSUC: sucessfull return
 *     BKC_MOD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_MOD_SPF_RELEASE is not
 *     defined in bkc_mod_spf.h) this func will always show
 *     empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_mod_watchmm(void);

#endif

