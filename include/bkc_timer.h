/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_timer
 *  file name: bkc_timer.h
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
#ifndef BKC_TIMER_H
#define BKC_TIMER_H

/*********************included files*****************************************/
#include "bkc_time.h"

/*********************definition, types & constants *************************/
/*ret value of funcs in bkc_timer*/
#define BKC_TIMER_RSUC       (0)
#define BKC_TIMER_RERR       (-1)

#define BKC_TIMER_NAME_LEN   (30)

/*types forward declaration in bkc_timer module*/
typedef struct bkc_timer_tag_t bkc_timer_t;
typedef struct bkc_timer_setting_tag bkc_timer_setting_t;

/*timer expiration function */
typedef int (*bkc_timer_expired_func_t)(void *param_p);

/*timer expiration mode*/
typedef enum bkc_timer_exp_mode_e {
    BKC_TIMER_EXP_MODE_EMPTY,
    BKC_TIMER_EXP_MODE_ONCE,
    BKC_TIMER_EXP_MODE_REPEAT
} bkc_timer_exp_mode_m;

/*timer setting*/
struct bkc_timer_setting_tag {
    bkc_time_t *exp_time_p;
    bkc_timer_expired_func_t exp_func;
    void *exp_func_param_p;
    bkc_timer_exp_mode_m exp_mode;
    char timer_name[BKC_TIMER_NAME_LEN];
};

/*********************prototype of open functions****************************/
/*****************************************************************************
 *                  bkc_timer_mod_init
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_TIMER_RSUC: sucessfull return
 *     BKC_TIMER_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     init the bkc_time module. only after bkc_timer_mod_init(),
 *     the other functions in bkc_timer.h can be used
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_timer_mod_init(void);

/*****************************************************************************
 *                  bkc_timer_mod_uninit
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_TIMER_RSUC: sucessfull return
 *     BKC_TIMER_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     uninit the bkc_timer module, thus, functions in bkc_timer.h
 *     can not be called after calling this func
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_timer_mod_uninit(void);

/*****************************************************************************
 *                  bkc_timer_definit_sett
 *****************************************************************************
 * parameters:
 *     timer_setting_p:
 *       points to the structure of timer settings which needs to be
 *       inited defaultly
 * return value:
 *     BKC_TIMER_RSUC: sucessfull return
 *     BKC_TIMER_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func will defaultly init a structure of bkc_timer_setting_t.
 *
 *     timer_setting_p->exp_time_p, timer_setting_p->exp_func,
 *     timer_setting_p->exp_func_param_p will be set to NULL, and
 *     timer_setting_p->exp_mode will be set to
 *     BKC_TIMER_EXP_MODE_EMPTY,timer_setting_p->timer_name will be
 *     set to empty string
 *
 *     before each structure of bkc_timer_setting_t can be used, this func
 *     needs to be called to init that structure
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_timer_definit_sett(bkc_timer_setting_t *timer_setting_p);

/*****************************************************************************
 *                  bkc_timer_creat
 *****************************************************************************
 * parameters:
 *     timer_setting_p:
 *       points to the structure of timer settings for the timer which will
 *       be created.
 * return value:
 *     Not NULL: points to the sucessfully created timer
 *     NULL: abnormal return
 * description:
 *     this func will create a instance of bkc_timer_t
 *****************************  notes  ************************************
 ****************************************************************************/
bkc_timer_t *bkc_timer_creat(bkc_timer_setting_t *timer_setting_p);

/*****************************************************************************
 *                  bkc_timer_destroy
 *****************************************************************************
 * parameters:
 *     timer_p:
 *       points to the timer which need to be destroyed
 * return value:
 *     BKC_TIMER_RSUC: sucessfull return
 *     BKC_TIMER_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func will destroy a instance of bkc_timer_t designated by
 *     timer_p
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_timer_destroy(bkc_timer_t *timer_p);

/*****************************************************************************
 *                  bkc_timer_set
 *****************************************************************************
 * parameters:
 *     timer_p:
 *       points to the timer of which the settings need to be set
 *       according to the timer_setting_p
 *     timer_setting_p
 *       points to the timer settings wich will be set to the timer_p.
 *       the members of the bkc_timer_setting_t are as follows:
 *         exp_time_p:
 *           the interval before the timer expired
 *         exp_func:
 *           the function which will be called when the timer expires
 *         exp_func_param_p:
 *           the parameter for the exp_func
 *         exp_mode:
 *           BKC_TIMER_EXP_MODE_ONCE:
 *             the timer only expire one time
 *           BKC_TIMER_EXP_MODE_REPEAT
 *             the timer will expire repeatedly
 *         timer_name:
 *             the timer name designate by the caller
 * return value:
 *     BKC_TIMER_RSUC: sucessfull return
 *     BKC_TIMER_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func will set the settings of timer_p according to the
 *     setting structure designated by timer_setting_p
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_timer_set(bkc_timer_t *timer_p, bkc_timer_setting_t *timer_setting_p);

/*****************************************************************************
 *                  bkc_timer_start
 *****************************************************************************
 * parameters:
 *     timer_p:
 *       points to the timer which need to be started
 * return value:
 *     BKC_TIMER_RSUC: sucessfull return
 *     BKC_TIMER_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func will start the timer_p,
 *     and the settings of the timer can not be changed until
 *     bkc_timer_stop() has been called on the same timer
 *
 *     if start a timer which has already been started, nothing will be done,
 *     and a warning will be poped up
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_timer_start(bkc_timer_t *timer_p);

/*****************************************************************************
 *                  bkc_timer_stop
 *****************************************************************************
 * parameters:
 *     timer_p:
 *       points to the timer which need to be stopped
 * return value:
 *     BKC_TIMER_RSUC: sucessfull return
 *     BKC_TIMER_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func will stop the timer_p
 *
 *     if stop a timer which has already been stopped, nothing will be done,
 *     and a warning will be poped up
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_timer_stop(bkc_timer_t *timer_p);

/*****************************************************************************
 *                  bkc_timer_watchmm
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_TIMER_RSUC: sucessfull return
 *     BKC_TIMER_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_TIMER_SPF_RELEASE is not
 *     defined in bkc_timer_spf.h) this func will always show
 *     empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_timer_watchmm(void);

#endif

