/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_pthread
 *  file name: bkc_pthread.h
 *  description: the posix thread facility supplied in bkc
 *****************************************************************************
 *  blackboard:
 *
 *
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
#ifndef BKC_PTHREAD_H
#define BKC_PTHREAD_H

/*********************included files*****************************************/

/*********************definition, types & constants *************************/
/*the maximum length of the pthread name in bytes*/
#define BKC_PTHREAD_NAME_MAX      (50)

#define BKC_PTHREAD_MAX_PRIORITY  (39)

/*ret value of funcs in bkc_pthread module*/
#define BKC_PTHREAD_RSUC   (0)
#define BKC_PTHREAD_RERR   (-1)

/*types forward declaration in bkc_pthread_join module*/
typedef struct bkc_pthread_tag_t bkc_pthread_t;

typedef struct bkc_pthread_create_info_s {
    /*can be the process id in ascii or the user defined name*/
    char creator_name[BKC_PTHREAD_NAME_MAX];
    /*the filename which the thread will be created*/
    char cr_file_name[BKC_PTHREAD_NAME_MAX];
    /*the function name in which the thread will be created*/
    char cr_func_name[BKC_PTHREAD_NAME_MAX];
    /*the line on which the thread will be created*/
    unsigned int cr_line;
} bkc_pthread_create_info_t;

/*the visible attribute of the thread to the caller*/
typedef struct bkc_pthread_attr_s {
    /*value range is [0, BKC_PTHREAD_MAX_PRIORITY], and the priority of 0 is the most favorable,
      BKC_PTHREAD_MAX_PRIORITY least favorable*/
    int priority;
    /*is detached*
      0: not detached, and need to be joined by bkc_pthread_join()
      1: detached, and can not be joined any more*/
    int is_detached;
    /*the create-info of this thread*/
    bkc_pthread_create_info_t create_info;

} bkc_pthread_attr_t;

/*selecting attributes for getting and setting*/
typedef enum bkc_pthread_selattr_e {
    BKC_PTHREAD_SELATTR_EMPTY,
    /*only need to get and set priority currently*/
    BKC_PTHREAD_SELATTR_PRIORITY,
    BKC_PTHREAD_SELATTR_DETACHS
} bkc_pthread_selattr_m;

/*operation mode for bkc_pthread_getsetattr()*/
typedef enum bkc_pthread_gsattr_mode_e {
    BKC_PTHREAD_GSATTR_MODE_EMPTY,
    /*for getting the attributes of the thread*/
    BKC_PTHREAD_GSATTR_MODE_GET,
    /*for setting the attributes of the thread*/
    BKC_PTHREAD_GSATTR_MODE_SET
} bkc_pthread_gsattr_mode_m;

/*selecting mode for setting and getting thread attributes*/
typedef enum bkc_pthread_prtinfo_mode_e {
    BKC_PTHREAD_PRTINFO_MODE_EMPTY,
    /*print the info of all the threads to the terminal*/
    BKC_PTHREAD_PRTINFO_MODE_ALL_TERM,
    /*print the info of all the threads to the designated file*/
    BKC_PTHREAD_PRTINFO_MODE_ALL_FILE,
    /*print the info of the specific thread to the terminal*/
    BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM,
    /*print the info of the specific thread to the file*/
    BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE,
    /*the following four are similar with the counterpart above in
      functionality, but with the difference that they
      will record the info as daemon in the background
      in a designated frequency*/
    BKC_PTHREAD_PRTINFO_MODE_ALL_TERM_DAEMON,
    BKC_PTHREAD_PRTINFO_MODE_ALL_FILE_DAEMON,
    BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM_DAEMON,
    BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE_DAEMON
} bkc_pthread_prtinfo_mode_m;

/*the operation for background info-printing deamon*/
typedef enum bkc_pthread_daemon_op_mode_e {
    BKC_PTHREAD_DAEMON_OP_MODE_EMPTY,
    BKC_PTHREAD_DAEMON_OP_MODE_SUSPEND,
    BKC_PTHREAD_DAEMON_OP_MODE_RESUME,
    BKC_PTHREAD_DAEMON_OP_MODE_DESTROY
} bkc_pthread_daemon_op_mode_m;

typedef void *(*bkc_pthread_sfunc)(void *arg_p);

/*********************prototype of open functions****************************/
/*****************************************************************************
 *                  bkc_pthread_mod_init
 *****************************************************************************
 * parameters:
 *     max_cpthr_num:
 *       the maximum number of the pthreads that can be created and run
 *       concurrently
 *     log_fname_p:
 *       the log file name for bkc_pthread module, which is a the string
 *       terminated by '\0'. it must not be NULL when we need to call
 *       bkc_pthread_prt_thrinfo(), with the param prt_mode be
 *       BKC_PTHREAD_PRTINFO_MODE_ALL_FILE,
 *       BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE,
 *       BKC_PTHREAD_PRTINFO_MODE_ALL_FILE_DAEMON,
 *       BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE_DAEMON
 *       if we wont use the above prt_mode when calling
 *       bkc_pthread_prt_thrinfo() , log_fname_p can be set NULL here
 *     interval:
 *       to desigate the frequency(every interval seconds do a recording)
 *       for the prt_mode passed in as the above four daemon modes.
 *       if we wont use the above prt_mode when calling
 *       bkc_pthread_prt_thrinfo() , interval can be set -1 here, and
 *       log_fname_p can be passed in as NULL
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     init the bkc_pthread module. only after bkc_pthread_modinit(),
 *     the other functions in bkc_pthread.h can be used
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_mod_init(int max_cpthr_num, char *log_fname_p, int interval);

/*****************************************************************************
 *                  bkc_pthread_mod_uninit
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     uninit the bkc_pthread module, thus, functions in bkc_pthread.h
 *     can not be called after calling this func
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_mod_uninit(void);

/*****************************************************************************
 *                  bkc_pthread_prt_thrinfo
 *****************************************************************************
 * parameters:
 *     prt_mode:
 *       BKC_PTHREAD_PRTINFO_MODE_ALL_TERM:
 *         print the info of all the threads to the terminal
 *       BKC_PTHREAD_PRTINFO_MODE_ALL_FILE:
 *         print the info of all the threads to the designated file of which
 *         the name is contained in fname_p param
 *       BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM:
 *         print the info of the specific thread to the terminal
 *       BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE:
 *         print the info of the specific thread to the file of which
 *         the name is contained in fname_p param
 *       BKC_PTHREAD_PRTINFO_MODE_ALL_TERM_DAEMON:
 *       BKC_PTHREAD_PRTINFO_MODE_ALL_FILE_DAEMON:
 *       BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM_DAEMON:
 *       BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE_DAEMON:
 *         they are similar with the counterpart above in
 *         functionality, but with the difference that they will record the info
 *         as daemon in the background in a designated frequency. and the
 *         frequency is passed in through interval param when 
 *         bkc_pthread_mod_init().
 *
 *         if in this mode, the caller need not to call this func any more,
 *         and just need to call bkc_pthread_operate_pdeamon() to resume,
 *         suspend, or destroy the background deamon when we neednt 
 *         it any more
 *     pthr_p:
 *       points to the specific thread, when the prt_mode is
 *       BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM or
 *       BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE, otherwise it is not used and
 *       can be passed in as NULL
 *     fname_p:
 *       points to the string which terminated by '\0' and contain
 *       the file name when prt_mode is BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM or
 *       BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE, otherwise it is not used and
 *       can be passed in as NULL
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     print the information of specific thread or all the threads to
 *     the terminal or designated file
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_prt_thrinfo(bkc_pthread_prtinfo_mode_m prt_mode,
        bkc_pthread_t *pthr_p);

/*****************************************************************************
 *                  bkc_pthread_operate_pdeamon
 *****************************************************************************
 * parameters:
 *     bkc_pthread_daemon_op_mode_m:
 *       BKC_PTHREAD_DAEMON_OP_MODE_SUSPEND:
 *         for suspending the daemon
 *       BKC_PTHREAD_DAEMON_OP_MODE_RESUME:
 *         for resuming the daemon
 *       BKC_PTHREAD_DAEMON_OP_MODE_DESTROY
 *         for destroying the daemon
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     for operating the background info-printing daemon which created by
 *     bkc_pthread_prt_thrinfo() with param prt_mode as the four
 *     deamon modes. the operation includes suspend, resume and destroy
 * 
 *     please note, this assume that bkc_pthread_prt_thrinfo() has already
 *     been called with the param prt_mode as the four deamon modes before
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_operate_pdeamon(bkc_pthread_daemon_op_mode_m mode);

/*****************************************************************************
 *                  bkc_pthread_attr_definit
 *****************************************************************************
 * parameters:
 *     attr_p:
 *        points to the thread attributes structure which need to be defaultly
 *        initialized.
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func defaultly init the attributes designated by attr_p.
 *     every var of the type bkc_pthread_attr_t must be initialized
 *     by bkc_pthread_attr_definit() before using it.
 *
 *     it will init the attr_p->priority to be 0(the most favorable priority),
 *     and the values of members in attr_p->create_info to be an
 *     empty(NULL) string
 *
 *     the caller can change the value of the member in attr_p later
 *     according to the actual needs
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_attr_definit(bkc_pthread_attr_t *attr_p);

/*****************************************************************************
 *                  bkc_pthread_getsetattr
 *****************************************************************************
 * parameters:
 *     pthr_p:
 *       points to the thread of which the attributes need to be set
 *     pthr_attr_p:
 *       points to the thread attributes structure that contains the attritues
 *       will be set to or gotten from the thread tagged by pthr_p
 *     selmode:
 *       for precisely designating which member in pthr_attr_p will be
 *       set or gotten,
 *       and others will left unchanged in pthr_p.
 *       value can be:
 *       BKC_PTHREAD_SELATTR_PRIORITY:
 *         set and get the priority member in the thread.
 *     gsmode:
 *       for precisely designating the operation mode
 *       BKC_PTHREAD_GSATTR_MODE_GET:
 *         for getting attributes designated by selmode, and
 *         thr_attr_p will be stuffed with the gotten attributes.
 *       BKC_PTHREAD_GSATTR_MODE_SET:
 *         for setting attributes designated by selmode, and
 *         the attritues for setting is prepared in thr_attr_p
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     for getting or setting the attritues of the thread tagged by pthr_p
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_getsetattr(bkc_pthread_t *pthr_p, bkc_pthread_attr_t *pthr_attr_p,
        bkc_pthread_selattr_m selmode, bkc_pthread_gsattr_mode_m gsmode);

/*****************************************************************************
 *                  bkc_pthread_create
 *****************************************************************************
 * parameters:
 *     attr_p:
 *        points to the thread attributes structure which will be applied on
 *        the thread returned by this function
 *        if passed will NULL, then the default priority of the thread will
 *        be 0
 *     sfunc:
 *       the start function for this thread, sometimes it is called a routine
 *       or a callback, but function maybe more straightforward
 *     arg_p:
 *       the arguments for the start function
 * return value:
 *     Not NULL: points to the sucessfully created thread
 *     NULL: abnormal return
 * description:
 *     create a thread with the designated attribution and start function
 *
 *     if sfunc return successfully, the thread created here
 *     is then called terminated. another case in which a thread is also called
 *     terminated is that sfunc has called bkc_pthread_exit() initiatively
 ***************************** notes **************************************
 ****************************************************************************/
bkc_pthread_t *bkc_pthread_create(bkc_pthread_attr_t *attr_p,
    bkc_pthread_sfunc sfunc, void *arg_p);

/*****************************************************************************
 *                  bkc_pthread_cancel
 *****************************************************************************
 * parameters:
 *     pthr_p:
 *       points to the thread that need to be canceled
 *     ret_pp:
 *       for storing the exit result fed by the bkc_pthread_exit(ret_p),
 *       in which the value of ret_p will be eventually stored in *ret_pp
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func shall cancel the destined thread tagged by pthr_p
 *
 *     the cancellation process will be asynchronous
 *     with respect to the calling thread returning from bkc_pthread_cancel(),
 *
 *     once a thread are canceled, it can not be used any more
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_cancel(bkc_pthread_t *pthr_p, void **ret_pp);

/*****************************************************************************
 *                  bkc_pthread_join
 *****************************************************************************
 * parameters:
 *     pthr_p:
 *       points to the thread that need to be waited by the caller
 *     ret_pp:
 *       for storing the exit result fed by the bkc_pthread_exit(ret_p),
 *       in which the value of ret_p will be eventually stored to *ret_pp
 *
 *       if the thread tagged by pthr_p returns successfully,
 *       but not by bkc_pthread_exit(), the effect shall be as if there
 *       was an implicit call to bkc_pthread_exit() using the
 *       return value of sfunc ,which was attached to the thread before,
 *       as the exit value. and the value will eventually be
 *       stored to *ret_pp when calling bkc_pthread_join(pthr_p, ret_pp)
 *       later
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func shall suspend execution of the calling thread until the
 *     target thread terminates, unless the target thread has already
 *     terminated
 *
 *     the results of multiple simultaneous calls to bkc_pthread_join()
 *     specifying the same target thread are undefined
 *
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_join(bkc_pthread_t *pthr_p, void **ret_pp);

/*****************************************************************************
 *                  bkc_pthread_exit
 *****************************************************************************
 * parameters:
 *     ret_p:
 *       the value in ret_p will be finally stored in *ret_pp
 *       when calling bkc_pthread_join(pthr_p, ret_pp) later
 * return value:
 *     N/A:
 *       once the call the bkc_pthread_exit(), the calling pthread
 *       will exit, and neednt return value to the calling pthread
 * description:
 *     bkc_pthread_exit() function shall terminate the calling thread and
 *     make the value ret_p available to any successful bkc_pthread_join()
 *     with the terminating thread
 ***************************** notes **************************************
 ****************************************************************************/
void bkc_pthread_exit(void *ret_p);

/*****************************************************************************
 *                  bkc_pthread_suspend
 *****************************************************************************
 * parameters:
 *     pthr_p:
 *       points to the thread which need to be suspended
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     for suspending the thread tagged by pthr_p.
 *     currently, the suspending of pthr_p is asynchronous with
 *     respect to the calling thread returning from bkc_pthread_suspend()
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_suspend(bkc_pthread_t *pthr_p);

/*****************************************************************************
 *                  bkc_pthread_resume
 *****************************************************************************
 * parameters:
 *     pthr_p:
 *       points to the thread which need to be resumed
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     for resuming the thread tagged by pthr_p.
 *     currently, the resuming of pthr_p is asynchronous with
 *     respect to the calling thread returning from bkc_pthread_resume()
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_resume(bkc_pthread_t *pthr_p);

/*****************************************************************************
 *                  bkc_pthread_self
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     points to the calling thread structure
 * description:
 *     this func shall return the pointer to the calling thread
 ***************************** notes **************************************
 ****************************************************************************/
bkc_pthread_t *bkc_pthread_self(void);

/*****************************************************************************
 *                  bkc_pthread_watchmm
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_PTHREAD_SPF_RELEASE is not
 *     defined in bkc_pthread_spf.h) this func will always show
 *     empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 *****************************  Notes  ************************************
 ****************************************************************************/
int bkc_pthread_watchmm(void);

#endif

