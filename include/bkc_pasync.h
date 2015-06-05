/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_pasync
 *  file name: bkc_pasync.h
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
#ifndef BKC_PASYNC_H
#define BKC_PASYNC_H

/*********************included files*****************************************/
#include "bkc_time.h"

/*********************definition, types & constants *************************/
/*ret value of funcs in bkc_pasync*/
#define BKC_PASYNC_RSUC   (0)
#define BKC_PASYNC_RERR   (-1)

/*types forward declaration in bkc_pasync module*/
typedef struct bkc_pasync_sem_tag_t bkc_pasync_sem_t;
typedef struct bkc_pasync_mutex_tag_t bkc_pasync_mutex_t;
typedef struct bkc_pasync_cond_tag_t bkc_pasync_cond_t;
/*settings for semaphor, mutex, conditon*/
typedef struct bkc_pasync_sem_sett_tag_t  bkc_pasync_sem_sett_t;
typedef struct bkc_pasync_mutex_sett_tag_t  bkc_pasync_mutex_sett_t;
typedef struct bkc_pasync_cond_sett_tag_t  bkc_pasync_cond_sett_t;

/*the process mode for the mutex and cond.
  currently we only need private mode*/
typedef enum bkc_pasync_pmode_e {
    BKC_PASYNC_PMODE_EMPTY,
    BKC_PASYNC_PMODE_PRIVATE
} bkc_pasync_pmode_m;

struct bkc_pasync_mutex_sett_t {
    bkc_pasync_pmode_m pmode;
};

struct bkc_pasync_cond_sett_t {
    bkc_pasync_pmode_m pmode;
};

struct bkc_pasync_sem_sett_t {
    bkc_pasync_pmode_m pmode;
    /*initial value for the semaphore*/
    int value;
};

/*********************prototype of open functions****************************/
/*****************************************************************************
 *                  bkc_pasync_mod_init
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     init the bkc_pasync module. only after bkc_pasync_mod_init(),
 *     the other functions in bkc_pasync.h can be used
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pasync_mod_init(void);

/*****************************************************************************
 *                  bkc_pasync_mod_uninit
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     uninit the bkc_pasync module, thus, functions in bkc_pasync.h
 *     can not be called after calling this func
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pasync_mod_uninit(void);

/*****************************************************************************
 *                  bkc_pasync_definit_semsett
 *****************************************************************************
 * parameters:
 *     sem_setting_p:
 *       points to the structure of bkc_pasync_sem_sett_t which needs to be
 *       inited defaultly
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func will defaultly init a structure of bkc_pasync_sem_sett_t.
 *
 *     sem_setting_p->pmode will be BKC_PASYNC_PMODE_PRIVATE,
 *     sem_setting_p->value will be 0
 *
 *     before each structure of bkc_pasync_sem_sett_t can be used, this func
 *     needs to be called to init that structure
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_definit_semsett(bkc_pasync_sem_sett_t *sem_setting_p);

/*****************************************************************************
 *                  bkc_pasync_definit_condsett
 *****************************************************************************
 * parameters:
 *     cond_setting_p:
 *       points to the structure of bkc_pasync_cond_sett_t which needs to be
 *       inited defaultly
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func will defaultly init a structure of bkc_pasync_cond_sett_t.
 *     cond_setting_p->pmode will be BKC_PASYNC_PMODE_PRIVATE
 *
 *     before each structure of bkc_pasync_cond_sett_t can be used, this func
 *     needs to be called to init that structure
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_definit_condsett(bkc_pasync_cond_sett_t *cond_setting_p);

/*****************************************************************************
 *                  bkc_pasync_definit_mutexsett
 *****************************************************************************
 * parameters:
 *     mutex_setting_p:
 *       points to the structure of bkc_pasync_mutex_sett_t which needs to be
 *       inited defaultly
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func will defaultly init a structure of bkc_pasync_mutex_sett_t.
 *     mutex_setting_p->pmode will be BKC_PASYNC_PMODE_PRIVATE
 *
 *     before each structure of bkc_pasync_mutex_sett_t can be used, this func
 *     needs to be called to init that structure
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_definit_mutexsett(bkc_pasync_mutex_sett_t *mutex_setting_p);

/*****************************************************************************
 *                  bkc_pasync_sem_create
 *****************************************************************************
 * parameters:
 *     sem_sett_p:
 *       points to the structure of bkc_pasync_sem_sett_t which contains
 *       the settings for the semaphor that will be created
 * return value:
 *     Not NULL: points to the sucessfully created semaphor
 *     NULL: abnormal return
 * description:
 *     this func is used to create a semaphor according to sem_sett_p
 *
 *     sem_sett_p needs to have been inited by bkc_pasync_definit_semsett()
 *     before we designate any value of the member in sem_sett_p
 *****************************  notes  ************************************
 ****************************************************************************/
bkc_pasync_sem_t *bkc_pasync_sem_create(bkc_pasync_sem_sett_t *sem_sett_p);

/*****************************************************************************
 *                  bkc_pasync_sem_destroy
 *****************************************************************************
 * parameters:
 *     sem_p:
 *       points to the semaphor that needs to be destroyed
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to destory the semaphor designated by sem_p
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_sem_destroy(bkc_pasync_sem_t *sem_p);

/*****************************************************************************
 *                  bkc_pasync_sem_p
 *****************************************************************************
 * parameters:
 *     sem_p:
 *       points to the semaphor structure that needs being applied to
 *       with passeren(in Dutch) operation
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to apply passeren(in Dutch) to the sem_p.
 *     Passeren can be seemed as pass in English
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_sem_p(bkc_pasync_sem_t *sem_p);

/*****************************************************************************
 *                  bkc_pasync_sem_v
 *****************************************************************************
 * parameters:
 *     sem_p:
 *       points to the semaphor that needs being applied to with
 *       verhoog(in Dutch) operation
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to apply verhoog(in Dutch) to the sem_p.
 *     verhoog can be seemed as increment in English
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_sem_v(bkc_pasync_sem_t *sem_p);

/*****************************************************************************
 *                  bkc_pasync_mutex_create
 *****************************************************************************
 * parameters:
 *     mutex_sett_p:
 *       points to the structure of bkc_pasync_mutex_sett_t which contains
 *       the settings for the mutex that will be created
 * return value:
 *     Not NULL: points to the sucessfully created mutex
 *     NULL: abnormal return
 * description:
 *     this func is used to create a mutex according to mutex_sett_p.
 *****************************  notes  ************************************
 ****************************************************************************/
bkc_pasync_mutex_t *bkc_pasync_mutex_create(
        bkc_pasync_mutex_sett_t *mutex_sett_p);

/*****************************************************************************
 *                  bkc_pasync_mutex_destroy
 *****************************************************************************
 * parameters:
 *     mutex_p:
 *       points to the mutex which need to be destroyed
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to destroy a mutex designated by mutex_p
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_mutex_destroy(bkc_pasync_mutex_t *mutex_p);

/*****************************************************************************
 *                  bkc_pasync_mutex_lock
 *****************************************************************************
 * parameters:
 *     mutex_p:
 *       points to the mutex that needs being applied to with the
 *       lock operation
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to apply lock operation to mutex_p
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_mutex_lock(bkc_pasync_mutex_t *mutex_p);

/*****************************************************************************
 *                  bkc_pasync_mutex_unlock
 *****************************************************************************
 * parameters:
 *     mutex_p:
 *       points to the mutex that needs being applied to with the
 *       unlock operation
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to apply unlock operation to mutex_p
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_mutex_unlock(bkc_pasync_mutex_t *mutex_p);

/*****************************************************************************
 *                  bkc_pasync_cond_create
 *****************************************************************************
 * parameters:
 *     cond_sett_p:
 *       points to the structure of bkc_pasync_cond_sett_t which contains
 *       the settings for the condition that will be created
 * return value:
 *     Not NULL: points to the sucessfully created condition
 *     NULL: abnormal return
 * description:
 *     this func is used to create a condition according to cond_sett_p.
 *****************************  notes  ************************************
 ****************************************************************************/
bkc_pasync_cond_t * bkc_pasync_cond_create(
        bkc_pasync_cond_sett_t *cond_sett_p);

/*****************************************************************************
 *                  bkc_pasync_cond_destroy
 *****************************************************************************
 * parameters:
 *     cond_p:
 *       points to the condtion that needs to be destroyed
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to destroy the condition designated by cond_p
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_cond_destroy(bkc_pasync_cond_t *cond_p);

/*****************************************************************************
 *                  bkc_pasync_cond_wait
 *****************************************************************************
 * parameters:
 *     cond_p:
 *       points to the condtion that needs being applied to with the
 *       wait operation
 *     duration_time_p:
 *       points to the time structure which designates the
 *       the duration to  wait
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to apply wait operation to cond_p
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_cond_wait(bkc_pasync_cond_t *cond_p,
        bkc_time_t *duration_time_p);

/*****************************************************************************
 *                  bkc_pasync_cond_signal
 *****************************************************************************
 * parameters:
 *     cond_p:
 *       points to the condtion that needs being applied to with the
 *       signal operation
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to apply signal operation to cond_p
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_cond_signal(bkc_pasync_cond_t *cond_p);

/*****************************************************************************
 *                  bkc_pasync_watchmm
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PASYNC_RSUC: sucessfull return
 *     BKC_PASYNC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_PASYNC_SPF_RELEASE is not
 *     defined in bkc_pasync_spf.h) this func will always show
 *     empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_pasync_watchmm(void);

#endif

