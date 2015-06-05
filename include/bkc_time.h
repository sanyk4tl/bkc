/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_time
 *  file name: bkc_time.h
 *  description: the basic string facility supplied in bkc
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
#ifndef BKC_TIME_H
#define BKC_TIME_H

/*********************included files*****************************************/

/*********************definition, types & constants *************************/
/*ret value of funcs in bkc_time*/
#define BKC_TIME_RSUC   (0)
#define BKC_TIME_RERR   (-1)

/*types forward declaration in bkc_time module*/
typedef struct bkc_time_tag bkc_time_t;
/*for the time description from the user view*/
struct bkc_time_t {
    int year;
    int mon;
    int hour;
    int min;
    int sec;
    /*million-seconds*/
    int million_sec;
    /*nano-second*/
    int nano_sec;
};

/*********************prototype of open functions****************************/
/*****************************************************************************
 *                  bkc_time_mod_init
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_TIME_RSUC: sucessfull return
 *     BKC_TIME_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     init the bkc_time module. only after bkc_time_mod_init(),
 *     the other functions in bkc_time.h can be used
 *
 *     once the bkc_time module is inited, there will be a global
 *     clock which records the current time in the format descripted in
 *     structure bkc_time_t, and the global clock is set to be
 *     running(clocking) by default.
 *
 *     the global clock default value is set with the time span which is
 *     counted from  00:00:00 on January 1, 1970,
 *     Coordinated Universal Time (UTC), and the precision is only
 *     seconds currently, and more accurate precision such as million seconds
 *     and nano seconds will be added later
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_time_mod_init(void);

/*****************************************************************************
 *                  bkc_time_mod_uninit
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_TIME_RSUC: sucessfull return
 *     BKC_TIME_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     uninit the bkc_time module, thus, functions in bkc_time.h
 *     can not be called after calling this func
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_time_mod_uninit(void);

/*****************************************************************************
 *                  bkc_time_get_ctime
 *****************************************************************************
 * parameters:
 *     time_p:
 *         points to the structure of bkc_time_t which will be used for
 *         returning the current clock time
 * return value:
 *     BKC_TIME_RSUC: sucessfull return
 *     BKC_TIME_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used for getting the current clock time.
 *     and time span will be counted from  00:00:00 on January 1, 1970,
 *     Coordinated Universal Time (UTC).
 *     the precision is only seconds currently, and more accurate
 *     precision such as million seconds and nano seconds will be added later
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_time_get_ctime(bkc_time_t *time_p);

/*****************************************************************************
 *                  bkc_time_set_ctime
 *****************************************************************************
 * parameters:
 *     time_p:
 *         points to the structure of bkc_time_t which will be used for
 *         setting the current clock time
 * return value:
 *     BKC_TIME_RSUC: sucessfull return
 *     BKC_TIME_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used for setting the current clock time.
 *     and the time span will be counted from  00:00:00 on January 1, 1970,
 *     Coordinated Universal Time (UTC).
 *     the precision is only seconds currently, and more accurate
 *     precision such as million seconds and nano seconds will be added later
 *
 *     this func can only be called when the global clock is stopped by
 *     calling bkc_time_stop_ctime(), or else, an error msg will be poped up,
 *     and the return value will be BKC_TIME_RERR
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_time_set_ctime(bkc_time_t *time_p);

/*****************************************************************************
 *                  bkc_time_stop_ctime
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_TIME_RSUC: sucessfull return
 *     BKC_TIME_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to stop the global clock supplied in this module.
 *     we can stop a stopped clock, and the clock will still be stopped,
 *     and more, a warning will be poped in that case
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_time_stop_ctime(void);

/*****************************************************************************
 *                  bkc_time_start_ctime
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_TIME_RSUC: sucessfull return
 *     BKC_TIME_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to continue the global clock which has been
 *     stopped by bkc_time_stop_ctime.
 *     we can continue a running clock(which means not stopped by
 *     bkc_time_stop_ctime), and a warning will be poped up in that case.
 *
 *     in fact, the bkc_time_mod_init() will set the global clock to be
 *     running innerly, and the user need not to call bkc_time_start_ctime()
 *     manually after calling bkc_time_mod_init().
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_time_start_ctime(void);

/*****************************************************************************
 *                  bkc_time_sync_ctime
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_TIME_RSUC: sucessfull return
 *     BKC_TIME_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to synchronize the global clock which the real current
 *     UTC time. this is usefull when we have called bkc_time_stop_ctime,
 *     bkc_time_start_ctime sometimes, and want to recover the global clock
 *     to be the real current UTC time
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_time_sync_ctime(void);

/*****************************************************************************
 *                  bkc_time_definit_tv
 *****************************************************************************
 * parameters:
 *     time_p:
 *       points to the structure of bkc_time_t which need to be
 *       inited defaultly
 * return value:
 *     BKC_TIME_RSUC: sucessfull return
 *     BKC_TIME_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     all the members in time_p will be set to 0.
 *     before each structure of bkc_time_t can be used, this func
 *     needs to be called to init that structure
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_time_definit_tv(bkc_time_t *time_p);

/*****************************************************************************
 *                  bkc_time_watchmm
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_TIME_RSUC: sucessfull return
 *     BKC_TIME_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_TIME_SPF_RELEASE is not
 *     defined in bkc_time_spf.h) this func will always show
 *     empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_time_watchmm(void);

#endif

