/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_pthread
 *  file name: bkc_pthread.c
 *  description: the pthread facility supplied in bkc
 *****************************************************************************
 *  blackboard
 *  pthr means: bkc posix thread
 *  posix thread means: ntpl posix thread
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
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <sched.h>
/*for strerror*/
#include <string.h>
/*for nanosleep*/
#include <errno.h>
#include <time.h>
#include "bkc_pthread.h"
#include "bkc_pthread_spf.h"
#include "bkc_dlist.h"

/*********************definition, types & constants *************************/
/*the maximum length of the pthread name in bytes*/
#define BKC_PTHREAD_NAME_MAX   50
/*the maximum number of the destroyed-pthread that can be
  recorded in bkc_pthread_pool_t*/
#define BKC_PTHREAD_MAX_DESTROYED_REC_NUM   300


/*the module initialliztion checking, designed for the open interface
  for this module*/
#define BKC_PTHREAD_MODINIT_CK(break_step, fret, fail_reason)  {\
    BKC_FC_CT(bkc_pthread_is_mod_inited == 1, break_step, fret,\
        fail_reason); \
}

/*UNINIT check for this module*/
#define BKC_PTHREAD_MODUNINIT_CK(break_step, fret, fail_reason)  {\
    BKC_FC_CT(bkc_pthread_is_mod_inited == 0, break_step, fret,\
        fail_reason); \
}

/*for asyncly entering pool*/
#define BKC_PTHREAD_SPECDATA_ENTER(break_step, fret, fail_reason) {\
    BKC_CR = bkc_pthread_specdata_enter();\
    BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, break_step, fret, fail_reason);\
}
#define BKC_PTHREAD_SPECDATA_LEAVE(break_step, fret, fail_reason) {\
    BKC_CR = bkc_pthread_specdata_leave();\
    BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, break_step, fret, fail_reason);\
}

/*validation and wait condition check*/
#define BKC_PTHREAD_VWAITCK(pthr_p, pthr_mb_idx, ckmb_p, ckmb_cond, bk_point, ret) {\
    BKC_PTHREAD_ISVAL(pthr_p, bk_point, ret);\
    BKC_PTHREAD_GET_MB(pthr_p, ckmb_p, pthr_mb_idx, bk_point, ret, NULL);\
    while (ckmb_cond) {\
        BKC_DBGP("yield\n");\
        BKC_DBGP("ckmb_p is %d\n", *ckmb_p);\
        sched_yield();\
        BKC_PTHREAD_ISVAL(pthr_p, bk_point, ret);\
        BKC_PTHREAD_GET_MB(pthr_p, ckmb_p, pthr_mb_idx, bk_point, ret, NULL);\
    }\
    BKC_FC_RT();\
}

/*wait condition check*/
#define BKC_PTHREAD_WAITCK(pthr_p, pthr_mb_idx, ckmb_p, ckmb_cond, bk_point, ret) {\
    BKC_PTHREAD_GET_MB(pthr_p, ckmb_p, pthr_mb_idx, bk_point, ret, NULL);\
    while (ckmb_cond) {\
        BKC_DBGP("yield\n");\
        sched_yield();\
        BKC_PTHREAD_GET_MB(pthr_p, ckmb_p, pthr_mb_idx, bk_point, ret, NULL);\
    }\
    BKC_FC_RT();\
}

#define BKC_PTHREAD_SET_MB(pthr_p, mb, index, break_point, ret, fail_reason) {\
    BKC_CR = bkc_pthread_set_pthrmb(pthr_p, mb, NULL, index);\
    BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC , break_point, ret, fail_reason);\
}

#define BKC_PTHREAD_GET_MB(pthr_p, mb_p, index, break_point, ret, fail_reason) {\
    BKC_CR = bkc_pthread_get_pthrmb(pthr_p, mb_p, NULL, index);\
    BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC , break_point, ret, fail_reason);\
}

#define BKC_PTHREAD_ISVAL(pthr_p, bk_point, ret) {\
    int bkc_pthread_inn_is_val = 0;\
    bkc_pthread_inn_is_val = bkc_pthread_isval(pthr_p);\
    BKC_FC_CT(bkc_pthread_inn_is_val == 1, bk_point, ret, "invalid thread\n");\
}

/*for init and operate dlist in this module*/
#define BKC_PTHREAD_DEFINIT_DLOP(op_p, break_step, fret, fail_reason) {\
    BKC_CR = bkc_dlist_definit_op(op_p);\
    BKC_FC_CT(BKC_CR == BKC_DLIST_RSUC, break_step, fret, fail_reason);\
}
#define BKC_PTHREAD_OPERATE_DLIST(dlist_p, op_p, break_step, \
    fret, fail_reason) {\
    bkc_dlist_operate(dlist_p, op_p);\
    BKC_FC_CT(BKC_CR == BKC_DLIST_RSUC, break_step, fret, fail_reason);\
}

#define BKC_PTHREAD_PRT_ERRNO(ret, sucval)  {\
    if ((ret) != (sucval)) {\
        char *posix_errmsg_p = NULL;\
        BKC_PRT("[func %s][line %d]", __func__, __LINE__);\
        posix_errmsg_p = strerror(ret);\
        BKC_PRT("erron is %d, alias:%s\n", ret, posix_errmsg_p == NULL ?\
            "empty" : posix_errmsg_p);\
    }\
}

#define BKC_PTHREAD_SF_CT(cond, ret, break_step, fret, fail_reason) {\
    if (!(cond)) {\
        char *posix_errmsg_p = NULL;\
        BKC_PRT("[func %s][line %d]", __func__, __LINE__);\
        posix_errmsg_p = strerror(ret);\
        BKC_PRT("erron is %d, alias:%s\n", ret, posix_errmsg_p == NULL ?\
            "empty" : posix_errmsg_p);\
    }\
    BKC_FC_CT(cond, break_step, fret, fail_reason);\
}

/*types forward declaration in bkc_pthread module*/
typedef struct bkc_pthread_time_info_tag_t   bkc_pthread_time_info_t;
typedef struct bkc_pthread_pfdes_tag_t       bkc_pthread_pfdes_t;
/*the pthread pool for the bkc_pthread module*/
typedef struct bkc_pthread_pool_tag_t            bkc_pthread_pool_t;
typedef struct bkc_pthread_pool_bulletin_tag_t   bkc_pthread_pool_bulletin_t;
typedef struct bkc_pthread_mod_specdata_tag_t    bkc_pthread_mod_specdata_t;
typedef struct bkc_pthread_daemon_param_tag_t    bkc_pthread_daemon_param_t;
typedef struct bkc_pthread_cr_param_tag_t    bkc_pthread_cr_param_t;

struct bkc_pthread_create_info_tag_t {
    /*can be the process id in ascii or the user defined name*/
    char creator_name[BKC_PTHREAD_NAME_MAX];
    /*the filename which the pthread will be created*/
    char cr_file_name[BKC_PTHREAD_NAME_MAX];
    /*the function name in which the pthread will be created*/
    char cr_func_name[BKC_PTHREAD_NAME_MAX];
    /*the line on which the pthread will be created*/
    unsigned int cr_line;
};

struct bkc_pthread_time_info_tag_t {
    /*the begin-time of this pthread in seconds from the system up
      or in UTC/GMT, which was passed in by the called*/
    time_t create_time;
    /*the destory-time of this pthread, the only way currently
      set this member is by bkc_pthread_join, otherwise, it will be
      0, means an invalid or unknown destory time*/
    time_t destroy_time;
};

/*the platform pthread descriptor, currently we can directly use the
  pthread_t in pthread.h conformed to the posix standard for convenience*/
struct bkc_pthread_pfdes_tag_t {
    pthread_t pth;
};

/*status of the thread*/
typedef enum bkc_pthread_status_e {
    /*invalid status*/
    BKC_PTHREAD_STATUS_EMPTY,
    /*active status, and the thread in this status
      can be suspended, canceled, getsetattr, joined,
      and exit selfly*/
    BKC_PTHREAD_STATUS_ACTIVE,
    /*paused status, and the thread in this status
      can be resumed, canceled, getsetattr, joined*/
    BKC_PTHREAD_STATUS_PAUSED,
    /*destroyed status, and this means that the thread
      have been terminated by cancel or run over selfly.
      the only operation can be applied to this thread is join*/
    BKC_PTHREAD_STATUS_DESTROYED,
    /*joined status, and this means that the thread
      have been terminated by cancel or run over selfly,
      and be joined which means it has been thoroughly cleared, and
      no operation can be applied to it anymore*/
    BKC_PTHREAD_STATUS_JOINED
} bkc_pthread_status_m;

typedef enum bkc_pthread_cf_status_e {
    BKC_PTHREAD_CF_STATUS_EMPTY,
    BKC_PTHREAD_CF_STATUS_NONEFC,
    BKC_PTHREAD_CF_STATUS_CREATE,
    BKC_PTHREAD_CF_STATUS_CANCEL,
    BKC_PTHREAD_CF_STATUS_EXIT,
    BKC_PTHREAD_CF_STATUS_JOIN,
    BKC_PTHREAD_CF_STATUS_SUSPEND,
    BKc_PTHREAD_CF_STATUS_RESUME,
    BKC_PTHREAD_CF_STATUS_SELF,
    BKC_PTHREAD_CF_STATUS_GSATTR,
} bkc_pthread_cf_status_m;

struct bkc_pthread_tag_t {
    /*the status of this thread*/
    bkc_pthread_status_m status;
    /*the time-info of this thread*/
    bkc_pthread_time_info_t time_info;
    /*the pthread descriptor*/
    bkc_pthread_pfdes_t pthr_des;
    /*the create-info of this thread*/
    bkc_pthread_create_info_t create_info;
    /*the priority of the this thread
      value range is [0, 40], and the priority of 0 is the most favorable,
      40 least favorable*/
    int priority;

    /*detach status*
      0: not detached, and need to be joined by bkc_pthread_join()
      1: detached, and can not be joined any more*/
    int is_detached;
    
    /*for suspending and resuming thread*/
    pthread_cond_t sr_cond;
    pthread_mutex_t sr_mutex;
    
    /*calling function on this thread*/
    bkc_pthread_cf_status_m cf_status;
    /*last interrupted status*/
    bkc_pthread_cf_status_m lint_status;
};

struct bkc_pthread_pool_bulletin_tag_t {
    /*total number of threads*/
    int total_pthnum;
    /*the max num of the active threads*/
    int active_max;
    /*the num of the current active threads*/
    int cur_active_num;
    /*the num of the current paused threads*/
    /*unsigned int cur_paused_num;*/
    /*the num of the current destroyed threads*/
    int cur_destroyed_num;
    /*the num of joined thread*/
    int cur_joined_num;
    /*the num of paused thread*/
    int cur_paused_num;
};

struct bkc_pthread_pool_tag_t {

    /*the pthread bulletin*/
    bkc_pthread_pool_bulletin_t pthr_bulletin;

    /*the maximum of the pthread that can be run concurrently*/
    int max_cpthr_num;
    /*the active and paused pthread list*/
    bkc_dlist_t *pthr_list_p;

    /*the maximum of the destroyed pthread that can be
      recorded in d_list_p member*/
    int max_dpthr_num;
    /*destroyed pthread list, for get the tracing of threads,
      and the length of the list will never be larger than  max_dpthr_num */
    bkc_dlist_t *d_list_p;
};

/*for the daemon param */
struct bkc_pthread_daemon_param_tag_t {
    FILE *dae_file_p;
    /*printing daemon*/
    bkc_pthread_t *target_pthr_p;
    /*printing interval*/
    int prt_interval;
};

/*for specific data used in this thread*/
struct bkc_pthread_mod_specdata_tag_t {
    /*the enter mutex for this specdata*/
    pthread_mutex_t specdata_enter_mutex;

    FILE *bkc_pthread_dae_file_p;
    /*the printing daemon in the background
      for updating the bkc_pthread_dae_file_p
      when pthread_prt_thrinfo() is called with
      the param prt_mode be
      BKC_PTHREAD_PRTINFO_MODE_ALL_TERM_DAEMON,
      BKC_PTHREAD_PRTINFO_MODE_ALL_FILE_DAEMON,
      BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM_DAEMON,
      BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE_DAEMON*/
    bkc_pthread_t *bkc_pthread_dae_p;
    /*printing interval*/
    int bkc_pthread_prt_interval;

    /*the pthread pool*/
    bkc_pthread_pool_t bkc_pthr_pool;

    /*dynamic stable data list*/
    /*the create param for each pthread*/
    bkc_dlist_t *pthr_crparam_list_p;
};

struct bkc_pthread_cr_param_tag_t {
    int is_valid;
    bkc_pthread_t *bkc_pthread_p;
    bkc_pthread_sfunc func;
    void *arg_p;
};

/*********************variables**********************************************/
/*the flag of the initialization status of this module
  0:initialized, 1:uninitialized*/
static int bkc_pthread_is_mod_inited = 0;
/*the mutex for accessing the bkc_pthread_is_mod_inited flag in order that
  this module will only be inited serially once*/
static pthread_mutex_t bkc_pthread_imi_mutex = PTHREAD_MUTEX_INITIALIZER;
/*the specdata in this module*/
static bkc_pthread_mod_specdata_t bkc_pthr_specdata;

/*********************prototype of local functions***************************/
static int bkc_pthread_specdata_enter(void);

static int bkc_pthread_specdata_leave(void);

static int bkc_pthread_mod_initck(void);

static int bkc_pthread_pool_adpthr(bkc_pthread_t *pthr_p);

static int bkc_pthread_pool_mvout(bkc_pthread_t *pthr_p);

static int bkc_pthread_cr_pthr(pthread_t *pid_p, bkc_pthread_attr_t *attr_p,
        bkc_pthread_cr_param_t *param_p);

static int bkc_pthread_cl_pthr(pthread_t pid, void **ret_pp);

static int bkc_pthread_map_prio(int *posix_prio_p,
        int *bpthr_prio_p, int mode);

static int bkc_pthread_node_traverse_func(void *unode_p, void *param_p);

static int bkc_pthread_put_crparam(bkc_pthread_cr_param_t *param_p);

static bkc_pthread_cr_param_t *bkc_pthread_get_crparam(void);

static void *bkc_pthread_cr_func(void *arg_p);

static int bkc_pthread_node_destroy_func(void *pthr_node_p, void *param_p);

static int bkc_pthread_crparam_node_destroy_func(void *crparam_node_p,
        void *param_p);

static int bkc_pthread_node_isitem_cmp_func(void *pthr_node_p,
        void *cmp_clue_p);

static void bkc_pthread_destroy_notify_func(void *arg_p);

static void bkc_pthread_signal_notify_func(int signo);

static bkc_pthread_clear_crparamlist(void);

static bkc_pthread_clear_pthrlist(void);

static void bkc_pthread_signal_notify_func(int signo);

static int bkc_pthread_set_pthrmb(bkc_pthread_t *pthr_p, 
        int mb, void *aux_mb_p, int index);

static int bkc_pthread_get_pthrmb(bkc_pthread_t *pthr_p, 
        int *mb_p, void *aux_mb_p, int index); 

static int bkc_pthread_isval(bkc_pthread_t *pthr_p);

static int bkc_pthread_gs_detachs(bkc_pthread_t *pthr_p,
        bkc_pthread_attr_t *pthr_attr_p, int gsmode);

/*********************implementation of open and local functions*************/
/*****************************************************************************
 *                  bkc_pthread_specdata_enter
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     enter the specdata for pthread module
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_specdata_enter(void)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int ret = -1;
        ret = pthread_mutex_lock(&(
            bkc_pthr_specdata.specdata_enter_mutex));
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_specdata_leave
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     leave the specdata for pthread module
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_specdata_leave(void)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int ret = -1;
        ret = pthread_mutex_unlock(
            &(bkc_pthr_specdata.specdata_enter_mutex));
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_get_pthrmb
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     get member in thread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_get_pthrmb(bkc_pthread_t *pthr_p, 
		int *mb_p, void *aux_mb_p, int index)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(pthr_p != NULL && mb_p != NULL &&
            (index == 1 || index == 0), 1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
        
        if (index == 0) {
        	*mb_p = pthr_p->status;
        } else if (index == 1) {
            *mb_p = pthr_p->cf_status;
        } else {
            BKC_FC_ERRTHR("err index", 1, BKC_PTHREAD_RERR);
        }
        
        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_set_pthrmb
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     set member in thread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_set_pthrmb(bkc_pthread_t *pthr_p, 
        int mb, void *aux_mb_p, int index)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(pthr_p != NULL && (index == 1 || index == 0), 
            1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
        
        if (index == 0) {
            pthr_p->status = mb;
        } else if (index == 1) {
            pthr_p->cf_status = mb;
        } else {
            BKC_FC_ERRTHR("err index", 1, BKC_PTHREAD_RERR);
        }
        
        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_get_crparam
 *****************************************************************************
 * parameters: N/A
 * return value:
 * description:
 *     get the create param node
 ***************************** notes **************************************
 ****************************************************************************/
bkc_pthread_cr_param_t *bkc_pthread_get_crparam(void)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    bkc_pthread_cr_param_t *param_p = NULL;

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);

        param_p = BKC_MALLOC(sizeof (bkc_pthread_cr_param_t));
        BKC_FC_CT(param_p != NULL, 2, BKC_PTHREAD_RERR, NULL);

        /*set default value*/
        param_p->is_valid = 0;
        param_p->bkc_pthread_p = NULL;
        param_p->func = NULL;
        param_p->arg_p = NULL;

        /*add to bkc_pthr_pool.pthr_list_p*/
        BKC_PTHREAD_DEFINIT_DLOP(&op, 2, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = param_p;
        BKC_PTHREAD_OPERATE_DLIST(bkc_pthr_specdata.pthr_crparam_list_p,
            &op, 2, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            param_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_SFREE(param_p);
            param_p = NULL;
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(param_p);
}

/*****************************************************************************
 *                  bkc_pthread_put_crparam
 *****************************************************************************
 * parameters: N/A
 * return value:
*     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     put the creat param node
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_put_crparam(bkc_pthread_cr_param_t *param_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CT(param_p != NULL, 1, BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);

        /*set default value*/
        param_p->is_valid = 0;
        param_p->bkc_pthread_p = NULL;
        param_p->func = NULL;
        param_p->arg_p = NULL;

        BKC_PTHREAD_DEFINIT_DLOP(&op, 1, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_REMOVE_NODE;
        op.tunode_p = param_p;
        op.rmvmode = BKC_DLIST_RM_DELUSERDATA;
        op.ophdlerset.unode_destroy = bkc_pthread_crparam_node_destroy_func;
        BKC_PTHREAD_OPERATE_DLIST(
            bkc_pthr_specdata.pthr_crparam_list_p,
            &op, 1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_pool_adpthr
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     add the pthr_p to the pool
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_pool_adpthr(bkc_pthread_t *pthr_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CT(pthr_p != NULL, 1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);

        /*add to bkc_pthr_pool.pthr_list_p*/
        BKC_PTHREAD_DEFINIT_DLOP(&op, 2, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = pthr_p;
        BKC_PTHREAD_OPERATE_DLIST(
            bkc_pthr_specdata.bkc_pthr_pool.pthr_list_p, &op,
            2, BKC_PTHREAD_RERR, NULL);

        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.total_pthnum += 1;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_active_num += 1;

        if (bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_active_num >
            bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.active_max) {
            bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.active_max =
                bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_active_num;
        }

        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

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
int bkc_pthread_suspend(bkc_pthread_t *pthr_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int status = -1;
        int ret = -1;
        BKC_FC_CT(pthr_p != NULL, 1, BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        /*re-entering checking*/
        BKC_PTHREAD_VWAITCK(pthr_p, 1, &status,
            status != BKC_PTHREAD_CF_STATUS_NONEFC, 1, BKC_PTHREAD_RERR);

        BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_SUSPEND, 1,
            1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_GET_MB(pthr_p, &status, 0, 1, BKC_PTHREAD_RERR, NULL);
        BKC_FC_CT(status != BKC_PTHREAD_STATUS_EMPTY &&
            status != BKC_PTHREAD_STATUS_JOINED &&
            status != BKC_PTHREAD_STATUS_DESTROYED, 2, BKC_PTHREAD_RERR,
            "posix cancel error\n");
 
        if (status == BKC_PTHREAD_STATUS_PAUSED) {
            BKC_WARN("already paused, need not to suspend\n");
            BKC_FC_SUCTHRS(BKC_PTHREAD_RSUC, 2);
        }
 
        ret = pthread_kill(pthr_p->pthr_des.pth, SIGUSR1);
        BKC_PTHREAD_PRT_ERRNO(ret, 0);
        BKC_FC_CT(ret == 0, 2, BKC_PTHREAD_RERR, NULL);
        
        pthr_p->status = BKC_PTHREAD_STATUS_PAUSED;

        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_active_num -= 1;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_paused_num += 1;
        BKC_FC_CT(bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.
            cur_active_num >= 0 && bkc_pthr_specdata.bkc_pthr_pool.
            pthr_bulletin.cur_paused_num >= 0, 2, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1,
            1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1, 
                1, BKC_PTHREAD_RERR, NULL); 
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

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
int bkc_pthread_resume(bkc_pthread_t *pthr_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int status = -1;
        BKC_FC_CT(pthr_p != NULL, 1, BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        /*re-entering checking*/
        BKC_PTHREAD_VWAITCK(pthr_p, 1, &status,
            status != BKC_PTHREAD_CF_STATUS_NONEFC, 1, BKC_PTHREAD_RERR);

        BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_SUSPEND, 1,
            1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_GET_MB(pthr_p, &status, 0, 1, BKC_PTHREAD_RERR, NULL);
        BKC_FC_CT(status != BKC_PTHREAD_STATUS_EMPTY &&
            status != BKC_PTHREAD_STATUS_JOINED &&
            status != BKC_PTHREAD_STATUS_DESTROYED, 2, BKC_PTHREAD_RERR,
            "posix cancel error\n");
 
        if (status == BKC_PTHREAD_STATUS_ACTIVE) {
            BKC_WARN("already active, need not to resume\n");
            BKC_FC_SUCTHRS(BKC_PTHREAD_RSUC, 2);
        }
 
        BKC_CR = pthread_mutex_lock(&(pthr_p->sr_mutex));
        BKC_PTHREAD_PRT_ERRNO(BKC_CR, 0);
        BKC_FC_CT(BKC_CR == 0, 2, BKC_PTHREAD_RERR, NULL);
        BKC_CR = pthread_cond_signal(&(pthr_p->sr_cond));
        BKC_PTHREAD_PRT_ERRNO(BKC_CR, 0);
        BKC_FC_CT(BKC_CR == 0, 2, BKC_PTHREAD_RERR, NULL);
        BKC_CR = pthread_mutex_unlock(&(pthr_p->sr_mutex));
        BKC_PTHREAD_PRT_ERRNO(BKC_CR, 0);
        BKC_FC_CT(BKC_CR == 0, 2, BKC_PTHREAD_RERR, NULL);
        
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_active_num += 1;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_paused_num -= 1;
        BKC_FC_CT(bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.
            cur_active_num >= 0 && bkc_pthr_specdata.bkc_pthr_pool.
            pthr_bulletin.cur_paused_num >= 0, 2, BKC_PTHREAD_RERR, NULL);
        
        pthr_p->status = BKC_PTHREAD_STATUS_ACTIVE;

        BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1,
            1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1, 
                1, BKC_PTHREAD_RERR, NULL); 
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_pthread_node_destroy_func
 *************************************************************************
 * parameters:
 *     pthr_node_p:
 *       points to the instance of type bkc_pthread_t, which need to
 *       be destroyed
 * return value:
 *     0: sucessfull return
 *     -1: erroneous return
 *     (other): abnormal return
 * description:
 *     destroy handler for node in bkc_pthr_pool
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_pthread_node_destroy_func(void *pthr_node_p, void *param_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    bkc_pthread_t *pthr_p = NULL;

    BKC_FLOWCONTROL_TRY {
        int ret = -1;

        pthr_p = (bkc_pthread_t *)pthr_node_p;
        BKC_FC_CT(pthr_p != NULL && pthr_p->status  != BKC_PTHREAD_STATUS_EMPTY, 
            1, -1, NULL);
        
        if (pthr_p->status == BKC_PTHREAD_STATUS_ACTIVE ||
            pthr_p->status == BKC_PTHREAD_STATUS_PAUSED) {
            /*use the posix thread func*/
            ret = pthread_cancel(pthr_p->pthr_des.pth);
            BKC_FC_CT(ret == 0, 1, -1, NULL);
            ret = pthread_join(pthr_p->pthr_des.pth, NULL);
            BKC_FC_CT(ret == 0, 1, -1, NULL);
        } else if (pthr_p->status == BKC_PTHREAD_STATUS_DESTROYED) {
            /*not joined yet, always join it. these will include the
              threads that are set detached, and this will lead to
              a definitely clear cleaning*/
            ret = pthread_join(pthr_p->pthr_des.pth, NULL);
            BKC_FC_CT(ret == 0, 1, -1, NULL);
        }
        BKC_PRT("bkc_phtread %p is freed\n", pthr_p);
        BKC_SFREE(pthr_p);
    }

    BKC_RETURN(0);
}

/*************************************************************************
 *                  bkc_pthread_crparam_node_destroy_func
 *************************************************************************
 * parameters:
 *     pthr_node_p:
 *       points to the instance of type bkc_pthread_cr_param_t, which need to
 *       be destroyed
 * return value:
 *     0: sucessfull return
 *     -1: erroneous return
 *     (other): abnormal return
 * description:
 *     destroy handler for node in bkc_pthr_pool.pthr_crparam_list_p
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_pthread_crparam_node_destroy_func(void *crparam_node_p, void *param_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(crparam_node_p != NULL, 1, -1, NULL);
        BKC_PRT("pthread create param node %p is freed\n", crparam_node_p);
        BKC_SFREE(crparam_node_p);
    }

    BKC_RETURN(0);
}

/*************************************************************************
 *                  bkc_pthread_destroy_notify_func
 *************************************************************************
 * parameters:
 *     arg_p:
 *       points to the instance of type bkc_pthread_cr_param_t
 * return value:
 * description:
 *      destroy notifying function
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_pthread_destroy_notify_func(void *arg_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    bkc_pthread_cr_param_t *param_p = NULL;

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(arg_p != NULL, 1, -1, NULL);
        param_p = (bkc_pthread_cr_param_t *)arg_p;
        BKC_FC_CT(param_p->func != NULL, 1, -1, NULL);

        BKC_DBGP("bkc_pthread_destroy_notify_func:"
            "param_p->bkc_pthread_p is %p\n", param_p->bkc_pthread_p);

        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
        param_p->bkc_pthread_p->time_info.destroy_time = time(NULL);
        BKC_CR = pthread_mutex_unlock(&(param_p->bkc_pthread_p->sr_mutex));
        BKC_FC_CT(BKC_CR == 0, 2, -1, NULL);
        BKC_CR = pthread_mutex_destroy(&(param_p->bkc_pthread_p->sr_mutex));
        BKC_FC_CT(BKC_CR == 0, 2, -1, NULL);
        BKC_CR = pthread_cond_destroy(&(param_p->bkc_pthread_p->sr_cond));
        BKC_FC_CT(BKC_CR == 0, 2, -1, NULL);    

        if (param_p->bkc_pthread_p->status != BKC_PTHREAD_STATUS_PAUSED) {
            bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.
                cur_active_num -= 1;
        } else {
            bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.
                cur_paused_num -= 1;
        }

        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_destroyed_num += 1;

        BKC_NP("bkc_pthr_specdata.bkc_pthr_pool."
            "pthr_bulletin.cur_active_num is %d\n",
            bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_active_num);
        BKC_FC_CT(bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.
            cur_active_num >= 0 && bkc_pthr_specdata.bkc_pthr_pool.
            pthr_bulletin.cur_destroyed_num >= 0, 2, BKC_PTHREAD_RERR, NULL);

        param_p->bkc_pthread_p->status = BKC_PTHREAD_STATUS_DESTROYED;
        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
        
        /*recover the cf status in this thread*/
        BKC_PTHREAD_SET_MB(param_p->bkc_pthread_p,
            param_p->bkc_pthread_p->lint_status, 1, 1, 
            BKC_PTHREAD_RERR, NULL); 
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_NRETURN;
}

/*************************************************************************
 *                  bkc_pthread_signal_notify_func
 *************************************************************************
 * parameters:
 *     arg_p:
 *       points to the instance of type bkc_pthread_cr_param_t
 * return value:
 * description:
 *      destroy notifying function
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_pthread_sigusr1_notify_func(int signo)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    bkc_pthread_t *pthr_p = NULL;
   
    BKC_FLOWCONTROL_TRY {
        pthr_p = bkc_pthread_self();
        BKC_DBGP("bkc_pthread_sigusr1_notify_func:self is %p\n", pthr_p);
        pthread_mutex_t *sr_mutex_p = NULL;
        pthread_cond_t *sr_cond_p = NULL;
        
        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
        /*get the mutex and cond*/
        sr_mutex_p = &(pthr_p->sr_mutex);
        sr_cond_p = &(pthr_p->sr_cond);
        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);

        /*suspend the thread*/
        BKC_CR = pthread_cond_wait(sr_cond_p, sr_mutex_p);
        BKC_FC_CT(BKC_CR == 0, 1, -1, NULL);
    }

    BKC_NRETURN ;
}

/*************************************************************************
 *                  bkc_pthread_cr_func
 *************************************************************************
 * parameters:
 *     arg_p:
 *       points to the instance of type bkc_pthread_cr_param_t
 * return value:
 * description:
 *     create function for each pthread
 *****************************  Notes  *****************************
 ************************************************************************/
void *bkc_pthread_cr_func(void *arg_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    void *ret_p = NULL;
    bkc_pthread_cr_param_t *param_p = NULL;

    BKC_FLOWCONTROL_TRY {
        struct sigaction  actions;
        bkc_pthread_cf_status_m cf_status = BKC_PTHREAD_CF_STATUS_EMPTY;
        param_p = (bkc_pthread_cr_param_t *)arg_p;
        BKC_FC_CT(arg_p != NULL && param_p->func != NULL, 1, -1, NULL);

        /*check creating progress*/
        BKC_PTHREAD_WAITCK(param_p->bkc_pthread_p, 1, &cf_status, 
            cf_status == BKC_PTHREAD_CF_STATUS_CREATE, 1, BKC_PTHREAD_RERR);

        BKC_MEMSET(&actions, 0, sizeof(actions));
        BKC_CR = sigemptyset(&actions.sa_mask);
        BKC_FC_CT(BKC_CR == 0, 1, -1, NULL);
        actions.sa_flags = 0;
        actions.sa_handler = bkc_pthread_sigusr1_notify_func;
        BKC_CR = sigaction(SIGUSR1,&actions,NULL);
        BKC_FC_CT(BKC_CR == 0, 1, -1, NULL);

        pthread_cleanup_push(bkc_pthread_destroy_notify_func, param_p);
        ret_p = param_p->func(param_p->arg_p);
        pthread_cleanup_pop(1);
    }

    BKC_RETURN(ret_p);
}

/*****************************************************************************
 *                  bkc_pthread_pool_mvout
 *****************************************************************************
 * parameters:
 *     is_need_destroy:
 *       0: need not destroy the thread
 *       1: need to destroy the thread
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     mv the pthr_p out of bkc_pthr_pool from the pthr_list_p
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_pool_mvout(bkc_pthread_t *pthr_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CT(pthr_p != NULL, 1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);

        /*remove from pthr_list_p*/
        BKC_PTHREAD_DEFINIT_DLOP(&op, 2, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_REMOVE_NODE;
        op.tunode_p = pthr_p;
        op.rmvmode = BKC_DLIST_RM_DELUSERDATA;
        op.ophdlerset.unode_destroy = bkc_pthread_node_destroy_func;

        BKC_PTHREAD_OPERATE_DLIST(bkc_pthr_specdata.bkc_pthr_pool.
            pthr_list_p, &op, 1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_isval
 *****************************************************************************
 * parameters:
 * return value:
 *     1: pthr_p is validate thread
 *     0: pthr_p is invalidate thread
 *     (other): abnormal return
 * description:
 *     is the pthread validate
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_isval(bkc_pthread_t *pthr_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    int is_found = 0;

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CT(pthr_p != NULL, 1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_DEFINIT_DLOP(&op, 2, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_FIND_NODE;
        op.tunode_p = pthr_p;
        BKC_PTHREAD_OPERATE_DLIST(bkc_pthr_specdata.bkc_pthr_pool.
            pthr_list_p, &op, 1, BKC_PTHREAD_RERR, NULL);
 
        is_found = (op.runode_p != NULL) ? 1 : 0;
        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(is_found);
}

/*****************************************************************************
 *                  bkc_pthread_clear_crparamlist
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     clear the creat param list pthr_list_p in bkc_pthr_pool
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_clear_crparamlist(void)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        bkc_pthread_cr_param_t *cur_node_p = NULL;
        bkc_pthread_cr_param_t *next_node_p = NULL;

        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_DEFINIT_DLOP(&op, 2, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_FIND_HEAD_NODE;
        BKC_PTHREAD_OPERATE_DLIST(bkc_pthr_specdata.pthr_crparam_list_p,
            &op, 2, BKC_PTHREAD_RERR, NULL);
        next_node_p = op.runode_p;

        while (next_node_p != NULL) {
            cur_node_p = next_node_p;
            /*get next node*/
            bkc_dlist_definit_op(&op);
            op.opcode = BKC_DLIST_OPCODE_FIND_NEXT_NODE;
            op.tunode_p = cur_node_p;
            BKC_PTHREAD_OPERATE_DLIST(bkc_pthr_specdata.pthr_crparam_list_p,
                &op, 2, BKC_PTHREAD_RERR, NULL);
            next_node_p = op.runode_p;

            /*manipulate the current node*/
            if (cur_node_p->is_valid == 0) {
                BKC_PTHREAD_DEFINIT_DLOP(&op, 2, BKC_PTHREAD_RERR, NULL);
                op.opcode = BKC_DLIST_OPCODE_REMOVE_NODE;
                op.tunode_p = cur_node_p;
                op.rmvmode = BKC_DLIST_RM_DELUSERDATA;
                op.ophdlerset.unode_destroy =
                    bkc_pthread_crparam_node_destroy_func;
                BKC_PTHREAD_OPERATE_DLIST(bkc_pthr_specdata.
                    pthr_crparam_list_p, &op, 2, BKC_PTHREAD_RERR, NULL);
            }
        }
        BKC_FC_RT();

        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_clear_pthrlist
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     clear the pthr_list_p in bkc_pthr_specdata
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_clear_pthrlist(void)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        int ret = -1;
        bkc_pthread_t *cur_node_p = NULL;
        bkc_pthread_t *next_node_p = NULL;
        
        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);

        BKC_PTHREAD_DEFINIT_DLOP(&op, 2, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_FIND_HEAD_NODE;
        BKC_PTHREAD_OPERATE_DLIST(bkc_pthr_specdata.bkc_pthr_pool.pthr_list_p,
            &op, 2, BKC_PTHREAD_RERR, NULL);
        next_node_p = op.runode_p;

        while (next_node_p != NULL) {
            cur_node_p = next_node_p;
            /*get next node*/
            bkc_dlist_definit_op(&op);
            op.opcode = BKC_DLIST_OPCODE_FIND_NEXT_NODE;
            op.tunode_p = cur_node_p;
            BKC_PTHREAD_OPERATE_DLIST(bkc_pthr_specdata.bkc_pthr_pool.
                pthr_list_p, &op, 2, BKC_PTHREAD_RERR, NULL);
            next_node_p = op.runode_p;

            /*manipulate the current node*/
            if (cur_node_p->status == BKC_PTHREAD_STATUS_DESTROYED &&
                cur_node_p->is_detached == 1) {

                /*use the posix thread func*/
                ret = pthread_join(cur_node_p->pthr_des.pth, NULL);
                BKC_FC_CT(ret == 0, 2, BKC_PTHREAD_RERR, NULL);
                cur_node_p->status = BKC_PTHREAD_STATUS_JOINED;
            }
            
            if (cur_node_p->status == BKC_PTHREAD_STATUS_JOINED) {
                BKC_PTHREAD_DEFINIT_DLOP(&op, 2, BKC_PTHREAD_RERR, NULL);
                op.opcode = BKC_DLIST_OPCODE_REMOVE_NODE;
                op.tunode_p = cur_node_p;
                op.rmvmode = BKC_DLIST_RM_DELUSERDATA;
                op.ophdlerset.unode_destroy = bkc_pthread_node_destroy_func;
                BKC_PTHREAD_OPERATE_DLIST(bkc_pthr_specdata.bkc_pthr_pool.
                    pthr_list_p, &op, 2, BKC_PTHREAD_RERR, NULL);
            }
        }
        BKC_FC_RT();

        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_mod_init
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_mod_init(int max_cpthr_num, char *log_fname_p, int interval)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    int ret = -1;

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CT(max_cpthr_num > 0, 1, BKC_PTHREAD_RERR, NULL);

        ret = pthread_mutex_lock(&(bkc_pthread_imi_mutex));
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODUNINIT_CK(2, BKC_PTHREAD_RERR, "mod is inited, need"
            "no init again\n");

        ret = pthread_mutex_init(
            &(bkc_pthr_specdata.specdata_enter_mutex), NULL);
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
        bkc_pthr_specdata.bkc_pthr_pool.max_cpthr_num =
            max_cpthr_num;
        bkc_pthr_specdata.bkc_pthr_pool.max_dpthr_num =
            BKC_PTHREAD_MAX_DESTROYED_REC_NUM;

        /*create dlist*/
        BKC_CR = bkc_dlist_definit_op(&op);
        BKC_FC_CT(BKC_CR == BKC_DLIST_RSUC, 1, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
        /*use the same creating param*/
        bkc_pthr_specdata.bkc_pthr_pool.pthr_list_p = bkc_dlist_create(&op);
        BKC_FC_CT(bkc_pthr_specdata.bkc_pthr_pool.pthr_list_p != NULL,
            1, BKC_PTHREAD_RERR, NULL);
        bkc_pthr_specdata.bkc_pthr_pool.d_list_p = bkc_dlist_create(&op);
        BKC_FC_CT(bkc_pthr_specdata.bkc_pthr_pool.d_list_p != NULL,
            1, BKC_PTHREAD_RERR, NULL);

        /*init the bulletin*/
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.active_max = 0;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.total_pthnum = 0;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_active_num = 0;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_destroyed_num = 0;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_joined_num = 0;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_paused_num = 0;

        /*init the specdata*/

        /*open daemon log file*/
        if (log_fname_p != NULL) {
            bkc_pthr_specdata.bkc_pthread_dae_file_p =
                fopen(log_fname_p, "a+");
            BKC_PTHREAD_SF_CT(bkc_pthr_specdata.bkc_pthread_dae_file_p !=
                NULL, errno, 1, BKC_PTHREAD_RERR, "fopen err\n");
        } else {
            bkc_pthr_specdata.bkc_pthread_dae_file_p = NULL;
        }

        if (interval != -1) {
            bkc_pthr_specdata.bkc_pthread_prt_interval = interval;
        }

        bkc_pthr_specdata.bkc_pthread_dae_p = NULL;

        /*init dynamic stable data*/
        BKC_PTHREAD_DEFINIT_DLOP(&op, 1, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
        bkc_pthr_specdata.pthr_crparam_list_p = bkc_dlist_create(&op);
        BKC_FC_CT(bkc_pthr_specdata.pthr_crparam_list_p != NULL,
            1, BKC_PTHREAD_RERR, NULL);

        bkc_pthread_is_mod_inited = 1;
        
        ret = pthread_mutex_unlock(&(bkc_pthread_imi_mutex));
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            ret = pthread_mutex_unlock(&(bkc_pthread_imi_mutex));
            BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_mod_uninit
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_mod_uninit(void)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    int ret = -1;
    
    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;

        ret = pthread_mutex_lock(&(bkc_pthread_imi_mutex));
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
 
        BKC_PTHREAD_MODINIT_CK(2, BKC_PTHREAD_RERR, "mod is uninited, need"
            "no uninit again\n");

        /*cancel the daemon if it is still running*/
        if (bkc_pthr_specdata.bkc_pthread_dae_p != NULL
            && (bkc_pthr_specdata.bkc_pthread_dae_p->status ==
                BKC_PTHREAD_STATUS_PAUSED
                || bkc_pthr_specdata.bkc_pthread_dae_p->status ==
                    BKC_PTHREAD_STATUS_ACTIVE) ){

            BKC_CR = bkc_pthread_cancel(
                bkc_pthr_specdata.bkc_pthread_dae_p, NULL);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2, BKC_PTHREAD_RERR, NULL);
            BKC_CR = bkc_pthread_join(
                bkc_pthr_specdata.bkc_pthread_dae_p, NULL);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2, BKC_PTHREAD_RERR, NULL);
        }

        bkc_pthr_specdata.bkc_pthr_pool.max_cpthr_num = 0;
        bkc_pthr_specdata.bkc_pthr_pool.max_dpthr_num = 0;

        /*destroy dlist*/
    #if 0
        BKC_PTHREAD_DEFINIT_DLOP(&op, 2, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
        bkc_dlist_operate(bkc_pthr_specdata.bkc_pthr_pool.pthr_list_p, &op);
    #endif

        BKC_PTHREAD_DEFINIT_DLOP(&op, 2, BKC_PTHREAD_RERR, NULL);
        op.dsymode = BKC_DLIST_DM_DELUSERDATA;
        op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
        op.ophdlerset.unode_destroy = bkc_pthread_node_destroy_func;

        /*use the same destroy param*/
        BKC_CR = bkc_dlist_destroy(
            bkc_pthr_specdata.bkc_pthr_pool.pthr_list_p, &op);
        BKC_FC_CT(BKC_CR == BKC_DLIST_RSUC, 2, BKC_PTHREAD_RERR, NULL);
        BKC_CR = bkc_dlist_destroy(
            bkc_pthr_specdata.bkc_pthr_pool.d_list_p, &op);
        BKC_FC_CT(BKC_CR == BKC_DLIST_RSUC, 2, BKC_PTHREAD_RERR, NULL);

        /*uninit the bulletin data*/
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.total_pthnum = 0;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.active_max = 0;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_active_num = 0;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_destroyed_num = 0;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_joined_num = 0;
        bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_paused_num = 0;
        
        /*uninit the specdata*/
        if (bkc_pthr_specdata.bkc_pthread_dae_file_p != NULL) {
            ret = fclose(bkc_pthr_specdata.bkc_pthread_dae_file_p);
            BKC_PTHREAD_SF_CT(ret == 0, errno, 2, BKC_PTHREAD_RERR, 
                "flcose err\n");
            bkc_pthr_specdata.bkc_pthread_dae_file_p = NULL;
        }

        /*uninit stable data*/
        BKC_PTHREAD_DEFINIT_DLOP(&op, 2, BKC_PTHREAD_RERR, NULL);
        op.dsymode = BKC_DLIST_DM_DELUSERDATA;
        op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
        op.ophdlerset.unode_destroy = bkc_pthread_crparam_node_destroy_func;
        BKC_CR = bkc_dlist_destroy(
            bkc_pthr_specdata.pthr_crparam_list_p, &op);
        BKC_FC_CT(BKC_CR == BKC_DLIST_RSUC, 2, BKC_PTHREAD_RERR, NULL);

        bkc_pthr_specdata.bkc_pthread_prt_interval = -1;
        bkc_pthread_is_mod_inited = 0;
        
        ret = pthread_mutex_unlock(&(bkc_pthread_imi_mutex));
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
        ret = pthread_mutex_destroy(&(bkc_pthread_imi_mutex));
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            ret = pthread_mutex_unlock(&(bkc_pthread_imi_mutex));
            BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

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
 *     deamon modes. the operation includes suspend, resume and destroy.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_operate_pdeamon(bkc_pthread_daemon_op_mode_m mode)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_pthread_t *ipthr_p = NULL;

        BKC_FC_CT(mode != BKC_PTHREAD_DAEMON_OP_MODE_EMPTY,
            1, BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
        ipthr_p = bkc_pthr_specdata.bkc_pthread_dae_p;
        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);

        BKC_FC_CT(ipthr_p != NULL, 1, BKC_PTHREAD_RERR, NULL);
        
        switch (mode) {
        case BKC_PTHREAD_DAEMON_OP_MODE_SUSPEND:
            BKC_CR = bkc_pthread_suspend(ipthr_p);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            break;
        case BKC_PTHREAD_DAEMON_OP_MODE_RESUME:
            BKC_CR = bkc_pthread_resume(ipthr_p);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            break;
        case BKC_PTHREAD_DAEMON_OP_MODE_DESTROY:
            BKC_CR = bkc_pthread_cancel(ipthr_p, NULL);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            break;
        default:
             BKC_FC_ERRTHR(1, BKC_PTHREAD_RERR, "mode err\n");
            break;
        }
        BKC_FC_RT();
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_attr_definit
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_attr_definit(bkc_pthread_attr_t *attr_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(attr_p != NULL, 1, BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        attr_p->is_detached = 0;
        attr_p->priority = 0;
        BKC_MEMSET(attr_p->create_info.creator_name, '\0',
            BKC_PTHREAD_NAME_MAX);
        BKC_MEMSET(attr_p->create_info.cr_file_name, '\0',
            BKC_PTHREAD_NAME_MAX);
        BKC_MEMSET(attr_p->create_info.cr_func_name, '\0',
            BKC_PTHREAD_NAME_MAX);
        /*start line of the file*/
        attr_p->create_info.cr_line = 0;
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_create
 *****************************************************************************
 * parameters:
 *     attr_p:
 *        points to the thread attributes structure which will be applied on
 *        the thread returned by this function
 *        if passed will NULL, then the default priority of the thread will
 *        be 0, and the creat info such as the calling info(file, line, func)
 *        are all omitted
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
 *     is then called terminated. another case in which a thread is also
 *     called terminated is that sfunc has called bkc_pthread_exit()
 *     initiatively
 ***************************** notes **************************************
 ****************************************************************************/
bkc_pthread_t *bkc_pthread_create(bkc_pthread_attr_t *attr_p,
        bkc_pthread_sfunc sfunc, void *arg_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    bkc_pthread_t *pthr_p = NULL;
    pthread_t posix_pid;
    bkc_pthread_cr_param_t *param_p = NULL;

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(attr_p != NULL && sfunc != NULL, 1, BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        BKC_FC_CT(bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.
            cur_active_num < bkc_pthr_specdata.bkc_pthr_pool.max_dpthr_num, 
            1, BKC_PTHREAD_RERR, "to many thread\n");
            
        /*get the bkc thread node*/
        pthr_p = BKC_MALLOC(sizeof(bkc_pthread_t));
        BKC_FC_CT(pthr_p != NULL, 2, BKC_PTHREAD_RERR, NULL);
        pthr_p->status = BKC_PTHREAD_STATUS_ACTIVE;
        pthr_p->cf_status = BKC_PTHREAD_CF_STATUS_CREATE;
        pthr_p->lint_status = BKC_PTHREAD_CF_STATUS_NONEFC;
        /*set the create time*/
        pthr_p->time_info.create_time = time(NULL);
        /*(time_t)(-1) is the invalid time of time_t*/
        pthr_p->time_info.destroy_time = ((time_t)(-1));
        pthr_p->is_detached = attr_p->is_detached;
        
        BKC_MEMCPY(pthr_p->create_info.cr_file_name, 
            attr_p->create_info.cr_file_name, BKC_PTHREAD_NAME_MAX - 1);
        BKC_MEMCPY(pthr_p->create_info.creator_name, 
            attr_p->create_info.creator_name, BKC_PTHREAD_NAME_MAX - 1);
        BKC_MEMCPY(pthr_p->create_info.cr_func_name, 
            attr_p->create_info.cr_func_name, BKC_PTHREAD_NAME_MAX - 1); 
        pthr_p->create_info.cr_line = attr_p->create_info.cr_line;

        /*get the create param*/
        param_p = bkc_pthread_get_crparam();
        BKC_FC_CT(param_p != NULL, 2, BKC_PTHREAD_RERR, NULL);
        param_p->func = sfunc;
        param_p->arg_p = arg_p;
        param_p->bkc_pthread_p = pthr_p;
        param_p->is_valid = 1;

        /*create the underlying posix thread*/
        BKC_CR = bkc_pthread_cr_pthr(&posix_pid, attr_p, param_p);
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 3, BKC_PTHREAD_RERR,
            "posix thread creat failed\n");
        BKC_MEMCPY(&(pthr_p->pthr_des.pth), &(posix_pid), sizeof(pthread_t));

        /*clear the pthread list and creat param list in bkc_pthr_specdata,
          this is a routine operation*/
        BKC_CR = bkc_pthread_clear_crparamlist();
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 4, BKC_PTHREAD_RERR, NULL);
        BKC_CR = bkc_pthread_clear_pthrlist();
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 4, BKC_PTHREAD_RERR, NULL);

        /*add to the bkc thread list*/
        BKC_CR = bkc_pthread_pool_adpthr(pthr_p);
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 4, BKC_PTHREAD_RERR, NULL);
        
        BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1, 
            4, BKC_PTHREAD_RERR, NULL); 
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            pthr_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_SFREE(pthr_p);
            pthr_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (3) {
            BKC_SFREE(pthr_p);
            pthr_p = NULL;
            BKC_CR = bkc_pthread_put_crparam(param_p);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (4) {
            BKC_SFREE(pthr_p);
            pthr_p = NULL;
            BKC_CR = bkc_pthread_put_crparam(param_p);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            BKC_CR = bkc_pthread_cl_pthr(posix_pid, NULL);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_CKPRET(pthr_p != NULL);
    BKC_RETURN(pthr_p);
}

/*****************************************************************************
 *                  bkc_pthread_cr_pthr
 *****************************************************************************
 * parameters:
 *     pid_p:
 *        points to the posix thread which need to be created
 *     attr_p:
 *        points to the thread attributes structure which will be applied on
 *        the thread returned by this function
 *        if passed will NULL, then the default priority of the thread will
 *        be 0, and the creat info such as the calling info(file, line, func)
 *        are all omitted
 *     sfunc:
 *       the start function for this thread, sometimes it is called a routine
 *       or a callback, but function maybe more straightforward
 *     arg_p:
 *       the arguments for the start function
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     create a posix thread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_cr_pthr(pthread_t *pid_p, bkc_pthread_attr_t *attr_p,
        bkc_pthread_cr_param_t *param_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int ret;
        int posix_prio = 0;

        /*default value */
        int posix_max_prio = 99;
        int posix_min_prio = 1;
        pthread_attr_t posix_attr;
        struct sched_param sparam;

        BKC_FC_CT(pid_p != NULL && param_p != NULL,
            1, BKC_PTHREAD_RERR, NULL);

        /* initialized with default attributes */
        ret = pthread_attr_init(&posix_attr);
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
        /* set the scheduling policy to SCHED_RR */
        ret = pthread_attr_setschedpolicy(&posix_attr, SCHED_RR);
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);

        /*map the priority range*/
        BKC_DBGP("posix_prio is %d, attr_p->priority is %d\n",
            posix_prio, attr_p->priority);
        BKC_CR = bkc_pthread_map_prio(&posix_prio, &(attr_p->priority), 1);
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
        BKC_DBGP("thread posix priority is %d", posix_prio);

        sparam.sched_priority = posix_prio;
        ret = pthread_attr_setschedparam(&posix_attr, &sparam);
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);

        /*init the mutex, cond for suspending and resuming*/
        BKC_CR = pthread_mutex_init(&(param_p->bkc_pthread_p->sr_mutex), NULL);
        BKC_FC_CT(BKC_CR == 0, 1, -1, NULL);
        BKC_CR = pthread_cond_init(&(param_p->bkc_pthread_p->sr_cond), NULL);
        BKC_FC_CT(BKC_CR == 0, 1, -1, NULL);
        BKC_CR = pthread_mutex_lock(&(param_p->bkc_pthread_p->sr_mutex));
        BKC_FC_CT(BKC_CR == 0, 1, -1, NULL);

        /*create the posix thread*/
        ret = pthread_create(pid_p, &posix_attr,
            bkc_pthread_cr_func, param_p);
        BKC_FC_CT(ret >= 0, 1,
            BKC_PTHREAD_RERR, "posix thread creat failed\n");
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_map_prio
 *****************************************************************************
 * parameters:
 *     posix_prio_p:
 *        posix priority
 *     bpthr_prio_p:
 *        bkc_pthread priority
 *     mode:
 *        0: from posix to bkc_pthread
 *        1: from bkc_pthread to posix
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     map priority
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_map_prio(int *posix_prio_p, int *bpthr_prio_p, int mode)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int posix_min_prio = 1;
        int posix_max_prio = 99;
        int posix_prio = -1;
        int bpthr_prio = -1;

        BKC_FC_CT(posix_prio_p != NULL && bpthr_prio_p != NULL,
            1, BKC_PTHREAD_RERR, NULL);

        posix_min_prio = sched_get_priority_min(SCHED_RR);
        posix_max_prio = sched_get_priority_max(SCHED_RR);

        BKC_DBGP("posix_min_prio is %d, posix_max_prio is %d\n",
            posix_min_prio, posix_max_prio);
        BKC_DBGP("bkc_pthread min prio is %d, bkc_pthread_max prio is %d\n",
            0, BKC_PTHREAD_MAX_PRIORITY);

        if (mode == 0) {
            /*from posix to bkc_pthread*/
            posix_prio = *posix_prio_p;
            bpthr_prio = BKC_PTHREAD_MAX_PRIORITY - (*posix_prio_p) *
                ((BKC_PTHREAD_MAX_PRIORITY + 1) /
                (posix_max_prio - posix_min_prio + 1));
            BKC_DBGP("in posix priority is %d,  out bkc priority is %d\n",
                posix_prio, bpthr_prio);
            BKC_FC_CT(bpthr_prio >= 0 &&
                bpthr_prio <= BKC_PTHREAD_MAX_PRIORITY,
                1, BKC_PTHREAD_RERR, NULL);
            *bpthr_prio_p = bpthr_prio;

        } else if (mode == 1) {
            /*from bkc_pthread to posix*/
            posix_prio = posix_min_prio + (BKC_PTHREAD_MAX_PRIORITY -
                *bpthr_prio_p) * (posix_max_prio - posix_min_prio + 1) /
                (BKC_PTHREAD_MAX_PRIORITY + 1);
            BKC_DBGP("in bkc priority is %d,  out posix priority is %d\n",
                *bpthr_prio_p, posix_prio);
            BKC_FC_CT(posix_prio >= posix_min_prio &&
                posix_prio <= posix_max_prio, 1, BKC_PTHREAD_RERR, NULL);
            *posix_prio_p = posix_prio;
        } else {
            BKC_ERR("err mode\n");
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_cl_pthr
 *****************************************************************************
 * parameters:
 *     pid_p:
 *        points to the posix thread which need to be canceled
 *     ret_pp:
 *       for storing the exit result fed by the pthread_exit(ret_p),
 *       in which the value of ret_p will be eventually stored in *ret_pp
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     create a posix thread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_cl_pthr(pthread_t pid, void **ret_pp)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int ret = -1;
        /*use the posix thread func*/
        ret = pthread_cancel(pid);
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

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
int bkc_pthread_cancel(bkc_pthread_t *pthr_p, void **ret_pp)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int status = -1;
        BKC_FC_CT(pthr_p != NULL, 1, BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        /*re-entering checking*/
        BKC_PTHREAD_VWAITCK(pthr_p, 1, &status, 
            status != BKC_PTHREAD_CF_STATUS_NONEFC && 
            status != BKC_PTHREAD_CF_STATUS_JOIN, 1, BKC_PTHREAD_RERR);
            
        if (status == BKC_PTHREAD_CF_STATUS_JOIN)
            pthr_p->lint_status = BKC_PTHREAD_CF_STATUS_JOIN;
        
        BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_CANCEL, 1, 
            1, BKC_PTHREAD_RERR, NULL); 

        BKC_PTHREAD_GET_MB(pthr_p, &status, 0, 1, BKC_PTHREAD_RERR, NULL);
        BKC_FC_CT(status != BKC_PTHREAD_STATUS_EMPTY && 
            status != BKC_PTHREAD_STATUS_JOINED, 2, BKC_PTHREAD_RERR,
            "posix cancel error\n");

        if (status == BKC_PTHREAD_STATUS_DESTROYED) {
            BKC_WARN("already destroyed, need not to cancel\n");
            BKC_FC_SUCTHRS(BKC_PTHREAD_RSUC, 2);
        }

        /*cancel the posix thread*/
        /*use the posix thread func*/
        BKC_CR = bkc_pthread_cl_pthr(pthr_p->pthr_des.pth, ret_pp);
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2, BKC_PTHREAD_RERR,
            "posix cancel error\n");

        /*in fact ,this step will be completed in the destroy handler, 
          here is only a hint*/
        /*
        BKC_PTHREAD_SET_MB(pthr_p, pthr_p->lint_status, 1, 
            1, BKC_PTHREAD_RERR, NULL); */
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1, 
                1, BKC_PTHREAD_RERR, NULL); 
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

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
 *     if the thread calling bkc_pthread_join() is canceled(e.g.
 *     by bkc_pthread_cancel()), then the target thread shall not be joined
 *     again
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_join(bkc_pthread_t *pthr_p, void **ret_pp)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int status = -1;
        int ret = -1;
        BKC_FC_CT(pthr_p != NULL, 1, BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        /*re-entering checking*/
        BKC_PTHREAD_VWAITCK(pthr_p, 1, &status, 
            status != BKC_PTHREAD_CF_STATUS_NONEFC, 1, BKC_PTHREAD_RERR);

        BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_JOIN, 1, 
            1, BKC_PTHREAD_RERR, NULL); 

        BKC_PTHREAD_GET_MB(pthr_p, &status, 0, 1, BKC_PTHREAD_RERR, NULL);
        BKC_FC_CT(status != BKC_PTHREAD_STATUS_EMPTY &&
            status != BKC_PTHREAD_STATUS_JOINED, 2, BKC_PTHREAD_RERR,
            "posix cancel error\n");
 
        /*use the posix thread func*/
        ret = pthread_join(pthr_p->pthr_des.pth, ret_pp);
        BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
        
        /*wait for destroying complete*/
        BKC_PTHREAD_VWAITCK(pthr_p, 0, &status, 
            status != BKC_PTHREAD_STATUS_DESTROYED, 2, BKC_PTHREAD_RERR);

        pthr_p->status = BKC_PTHREAD_STATUS_JOINED;

        BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1, 
            1, BKC_PTHREAD_RERR, NULL); 
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1, 
                1, BKC_PTHREAD_RERR, NULL); 
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}



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
 *       will exit, and need not returned value to the calling pthread
 * description:
 *     bkc_pthread_exit() function shall terminate the calling thread and
 *     make the value ret_p available to any successful bkc_pthread_join with
 *     the terminating thread
 ***************************** notes **************************************
 ****************************************************************************/
void bkc_pthread_exit(void *ret_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    bkc_pthread_t *pthr_p = NULL;

    BKC_FLOWCONTROL_TRY {
        int status = -1;
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        /*get self*/
        pthr_p = bkc_pthread_self();
        BKC_FC_CT(pthr_p != NULL, 1, BKC_PTHREAD_RERR, NULL);
        
        /*re-entering checking*/
        BKC_PTHREAD_VWAITCK(pthr_p, 1, &status, 
            status != BKC_PTHREAD_CF_STATUS_NONEFC &&
            status != BKC_PTHREAD_CF_STATUS_JOIN, 1, BKC_PTHREAD_RERR);

        if (status == BKC_PTHREAD_CF_STATUS_JOIN)
            pthr_p->lint_status = BKC_PTHREAD_CF_STATUS_JOIN;
        else
            pthr_p->lint_status = BKC_PTHREAD_CF_STATUS_NONEFC;

        BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_EXIT, 1, 
            1, BKC_PTHREAD_RERR, NULL); 

        BKC_PTHREAD_GET_MB(pthr_p, &status, 0, 1, BKC_PTHREAD_RERR, NULL);
        
        BKC_FC_CT(status != BKC_PTHREAD_STATUS_EMPTY &&
            status != BKC_PTHREAD_STATUS_JOINED, 2, BKC_PTHREAD_RERR,
            "posix cancel error\n");
        
        if (status == BKC_PTHREAD_STATUS_DESTROYED) {
            BKC_WARN("already destroyed, need not to exit\n");
            BKC_FC_SUCTHRS(BKC_PTHREAD_RSUC, 2);
        }

        /*exit the posix thread, this is and need to be the last step
          in this func*/
        pthread_exit(ret_p);
        
        /*in fact ,this step will be completed in the destroy handler, 
          here is only a hint*/
        /*
        BKC_PTHREAD_SET_MB(pthr_p, pthr_p->lint_status, 1, 
            1, BKC_PTHREAD_RERR, NULL); */
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1, 
                1, BKC_PTHREAD_RERR, NULL); 
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }
    
    BKC_NRETURN;
}

/*************************************************************************
 *                  bkc_pthread_node_isitem_cmp_func
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     compare handler for bkc_pthread node
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_pthread_node_isitem_cmp_func(void *pthr_node_p, void *cmp_clue_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    bkc_pthread_t *ipthr_node_p = NULL;
    pthread_t *pid_p = NULL;
    int iscmp = -1;

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(pthr_node_p != NULL && cmp_clue_p != NULL,
            1, BKC_PTHREAD_RERR, NULL);

        ipthr_node_p = (bkc_pthread_t *)pthr_node_p;
        pid_p = (pthread_t *)cmp_clue_p;
        iscmp = pthread_equal(ipthr_node_p->pthr_des.pth, *pid_p);

        /*map the equal and not equal condition*/
        if (iscmp > 0)
            iscmp = 0;
        else
            iscmp = -1;
    }

    BKC_RETURN(iscmp);
}

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
bkc_pthread_t *bkc_pthread_self(void)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    bkc_pthread_t *ret_pthr_p = NULL;

    BKC_FLOWCONTROL_TRY {
        pthread_t posix_pid;
        bkc_dlist_operator_t op;
        int status = -1;

        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
        posix_pid = pthread_self();
        BKC_PTHREAD_DEFINIT_DLOP(&op, 3, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_FIND_SPEC_NODE;
        op.clue_p = &posix_pid;
        op.ophdlerset.unode_isitem_cmp = bkc_pthread_node_isitem_cmp_func;
        BKC_PTHREAD_OPERATE_DLIST(
            bkc_pthr_specdata.bkc_pthr_pool.pthr_list_p, &op, 3,
            BKC_PTHREAD_RERR, NULL);
        ret_pthr_p = op.runode_p;
        BKC_FC_CT(ret_pthr_p != NULL, 3, BKC_PTHREAD_RERR,
            "not find the self thread\n");
        BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);

        /*re-entering checking*/
        BKC_PTHREAD_VWAITCK(ret_pthr_p, 1, &status, 
            status != BKC_PTHREAD_CF_STATUS_NONEFC &&
            status != BKC_PTHREAD_CF_STATUS_JOIN, 1, BKC_PTHREAD_RERR);
            
        if (status == BKC_PTHREAD_CF_STATUS_JOIN)
            ret_pthr_p->lint_status = BKC_PTHREAD_CF_STATUS_JOIN;
        else
            ret_pthr_p->lint_status = BKC_PTHREAD_CF_STATUS_NONEFC;
            
        BKC_PTHREAD_SET_MB(ret_pthr_p, BKC_PTHREAD_CF_STATUS_SELF, 1, 
            1, BKC_PTHREAD_RERR, NULL); 

        /*we need do nothing but just a validation*/
        BKC_PTHREAD_GET_MB(ret_pthr_p, &status, 0, 1, BKC_PTHREAD_RERR, NULL);
        BKC_FC_CT(status != BKC_PTHREAD_STATUS_EMPTY &&
            status != BKC_PTHREAD_STATUS_JOINED && 
            status != BKC_PTHREAD_STATUS_DESTROYED, 2, BKC_PTHREAD_RERR,
            "posix cancel error\n");

        BKC_PTHREAD_SET_MB(ret_pthr_p, ret_pthr_p->lint_status, 1, 
            1, BKC_PTHREAD_RERR, NULL); 
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SET_MB(ret_pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1, 
                1, BKC_PTHREAD_RERR, NULL); 
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
    }

    BKC_CKPRET(ret_pthr_p != NULL);
    BKC_RETURN(ret_pthr_p);
}

/*****************************************************************************
 *                  bkc_pthread_gs_pxprio
 *****************************************************************************
 * parameters:
 *     pthr_p:
 *       points to the thread of which the attributes need to be set
 *     pthr_attr_p:
 *       points to the thread attributes structure that contains the attritues
 *       will be set to or gotten from the thread tagged by pthr_p
 *     gsmode:
 *       for precisely designating the operation mode
 *       0:
 *         for getting attributes designated by selmode, and
 *         thr_attr_p will be stuffed with the gotten attributes.
 *       1:
 *         for setting attributes designated by selmode, and
 *         the attritues for setting is prepared in thr_attr_p
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     for getting or setting the posix attritues of the thread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_gs_pxprio(bkc_pthread_t *pthr_p,
        bkc_pthread_attr_t *pthr_attr_p, int gsmode)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int ret = -1;
        struct sched_param sparam;
        int policy;

        BKC_FC_CT(pthr_p != NULL && pthr_attr_p  != NULL, 1,
            BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        switch (gsmode) {
        case 0:
            ret = pthread_getschedparam(pthr_p->pthr_des.pth, &policy,
                &sparam);
            BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
            BKC_CR = bkc_pthread_map_prio(&(sparam.sched_priority),
                &(pthr_attr_p->priority), 0);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            break;
        case 1:
        #if 0
            BKC_CR = bkc_pthread_map_prio(&(sparam.sched_priority),
                &(pthr_attr_p->priority), 1);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            ret = pthread_setschedparam(pthr_p->pthr_des.pth,
                SCHED_OTHER, &sparam);
            BKC_FC_CT(ret == 0, 1, BKC_PTHREAD_RERR, NULL);
        #endif
            BKC_WARN("set priority dynamicly is not "
                "supported by the phtread library yet\n");
            break;
        default:
             BKC_FC_ERRTHR(1, BKC_PTHREAD_RERR, "gsmode err\n");
            break;
        }
        BKC_FC_RT();
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_gs_detachs
 *****************************************************************************
 * parameters:
 *     pthr_p:
 *       points to the thread of which the attributes need to be set
 *     pthr_attr_p:
 *       points to the thread attributes structure that contains the attritues
 *       will be set to or gotten from the thread tagged by pthr_p
 *     gsmode:
 *       for precisely designating the operation mode
 *       0:
 *         for getting attributes designated by selmode, and
 *         thr_attr_p will be stuffed with the gotten attributes.
 *       1:
 *         for setting attributes designated by selmode, and
 *         the attritues for setting is prepared in thr_attr_p
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     for getting or setting the detach of the thread
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_gs_detachs(bkc_pthread_t *pthr_p,
        bkc_pthread_attr_t *pthr_attr_p, int gsmode)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int ret = -1;
        struct sched_param sparam;
        int policy;

        BKC_FC_CT(pthr_p != NULL && pthr_attr_p  != NULL, 1,
            BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        switch (gsmode) {
        case 0:
            pthr_p->is_detached = pthr_attr_p->is_detached;
            break;
        case 1:
            pthr_attr_p->is_detached = pthr_p->is_detached;
            break;
        default:
             BKC_FC_ERRTHR(1, BKC_PTHREAD_RERR, "gsmode err\n");
            break;
        }
        BKC_FC_RT();
    }

    BKC_RETURN(BKC_FNRET);
}

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
 *       bkc_pthread_selattr_PRIORITY:
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
int bkc_pthread_getsetattr(bkc_pthread_t *pthr_p,
        bkc_pthread_attr_t *pthr_attr_p, bkc_pthread_selattr_m selattr,
        bkc_pthread_gsattr_mode_m gsmode)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        pthread_t posix_pid;
        int status = -1;
        BKC_FC_CT(pthr_p != NULL && pthr_attr_p  != NULL, 1,
            BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        /*re-entering checking*/
        BKC_PTHREAD_VWAITCK(pthr_p, 1, &status, 
            status != BKC_PTHREAD_CF_STATUS_NONEFC && 
            status != BKC_PTHREAD_CF_STATUS_JOIN, 1, BKC_PTHREAD_RERR);

        BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_GSATTR, 1, 
            1, BKC_PTHREAD_RERR, NULL); 

        BKC_PTHREAD_GET_MB(pthr_p, &status, 0, 1, BKC_PTHREAD_RERR, NULL);
        BKC_FC_CT(status != BKC_PTHREAD_STATUS_EMPTY &&
            status != BKC_PTHREAD_STATUS_JOINED, 2, BKC_PTHREAD_RERR,
            "posix cancel error\n");

        switch (selattr) {
        case BKC_PTHREAD_SELATTR_PRIORITY:
            switch (gsmode) {
            case BKC_PTHREAD_GSATTR_MODE_GET:
                BKC_CR = bkc_pthread_gs_pxprio(pthr_p, pthr_attr_p, 0);
                BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2,
                    BKC_PTHREAD_RERR, NULL);
                break;
            case BKC_PTHREAD_GSATTR_MODE_SET:
                BKC_CR = bkc_pthread_gs_pxprio(pthr_p, pthr_attr_p, 1);
                BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2,
                    BKC_PTHREAD_RERR, NULL);
                break;
            default:
                 BKC_FC_ERRTHR(2, BKC_PTHREAD_RERR, "gsmode err\n");
                break;
            }
            BKC_FC_RT();

            break;
        case BKC_PTHREAD_SELATTR_DETACHS:
            switch (gsmode) {
            case BKC_PTHREAD_GSATTR_MODE_GET:
                BKC_CR = bkc_pthread_gs_detachs(pthr_p, pthr_attr_p, 0);
                BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2,
                    BKC_PTHREAD_RERR, NULL);
                break;
            case BKC_PTHREAD_GSATTR_MODE_SET:
                BKC_CR = bkc_pthread_gs_detachs(pthr_p, pthr_attr_p, 1);
                BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2,
                    BKC_PTHREAD_RERR, NULL);
                break;
            default:
                 BKC_FC_ERRTHR(2, BKC_PTHREAD_RERR, "gsmode err\n");
                break;
            }
            BKC_FC_RT();

            break;
        default:
            BKC_FC_ERRTHR(2, BKC_PTHREAD_RERR, "selattr err\n");
            break;
        }
        BKC_FC_RT();

        BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1, 
            1, BKC_PTHREAD_RERR, NULL); 
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SET_MB(pthr_p, BKC_PTHREAD_CF_STATUS_NONEFC, 1, 
                1, BKC_PTHREAD_RERR, NULL); 
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

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
 *         frequency is passed in through interval param.
 *
 *         if in this mode, the caller need not to call this func any more,
 *         and just need to call bkc_pthread_operate_pdea() to resume,
 *         suspend, or destroy the background deamon. we must destroy the
 *         daemon when we neednt it any more
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
 *     seconds:
 *       to desigate the frequency(every interval seconds do a recording)
 *       for the prt_mode passed in as the four daemon modes.
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
        bkc_pthread_t *pthr_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        /*pthr_p can be NULL for BKC_PTHREAD_PRTINFO_MODE_ALL_TERM/FILE*/
        BKC_FC_CT(prt_mode != BKC_PTHREAD_PRTINFO_MODE_EMPTY,
            1, BKC_PTHREAD_RERR, NULL);
        BKC_PTHREAD_MODINIT_CK(1, BKC_PTHREAD_RERR, "mod uninited\n");

        switch (prt_mode) {
        case BKC_PTHREAD_PRTINFO_MODE_ALL_TERM:
            BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
            BKC_CR = bkc_pthread_prt_allitem_tfile(stdout);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2, BKC_PTHREAD_RERR, NULL);
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            break;
        case BKC_PTHREAD_PRTINFO_MODE_ALL_FILE:
            BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
            BKC_CR = bkc_pthread_prt_allitem_tfile(
                bkc_pthr_specdata.bkc_pthread_dae_file_p);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2, BKC_PTHREAD_RERR, NULL);
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            break;
        case BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM:
            BKC_PTHREAD_ISVAL(pthr_p, 1, BKC_PTHREAD_RERR);
            BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
            BKC_CR = bkc_pthread_prt_specitem_tfile(
                pthr_p, stdout);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2, BKC_PTHREAD_RERR, NULL);
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            break;
        case BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE:
            BKC_PTHREAD_ISVAL(pthr_p, 1, BKC_PTHREAD_RERR);
            BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
            BKC_CR = bkc_pthread_prt_specitem_tfile(
                pthr_p, bkc_pthr_specdata.bkc_pthread_dae_file_p);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2, BKC_PTHREAD_RERR, NULL);
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            break;
        case BKC_PTHREAD_PRTINFO_MODE_ALL_TERM_DAEMON:
            /*daemon will asyncly prt the thread info selfly*/
            BKC_CR = bkc_pthread_prt_aitf_daemon(stdout);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            break;
        case BKC_PTHREAD_PRTINFO_MODE_ALL_FILE_DAEMON:
            /*daemon will asyncly prt the thread info selfly*/
            BKC_CR = bkc_pthread_prt_aitf_daemon(
                bkc_pthr_specdata.bkc_pthread_dae_file_p);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            break;
        case BKC_PTHREAD_PRTINFO_MODE_SPEC_TERM_DAEMON:
            /*daemon will asyncly prt the thread info selfly*/
            BKC_PTHREAD_ISVAL(pthr_p, 1, BKC_PTHREAD_RERR);
            BKC_CR = bkc_pthread_prt_spectof_daemon(
                pthr_p, stdout);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            break;
        case BKC_PTHREAD_PRTINFO_MODE_SPEC_FILE_DAEMON:
            /*daemon will asyncly prt the thread info selfly*/
            BKC_PTHREAD_ISVAL(pthr_p, 1, BKC_PTHREAD_RERR);
            BKC_CR = bkc_pthread_prt_spectof_daemon(
                pthr_p, bkc_pthr_specdata.bkc_pthread_dae_file_p);
            BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
            break;
        default:
            BKC_FC_ERRTHR(2, BKC_PTHREAD_RERR, "bkc_pthread_prt_thrinfo err\n");
            break;
        }
        BKC_FC_RT();
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_prt_allitem_tfile
 *****************************************************************************
 * parameters:
 *     file_p:
 *         the log file handle
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     print the information of all the threads to
 *     the designated file
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_prt_allitem_tfile(FILE *file_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(file_p != NULL, 1, -1, NULL);
        BKC_CR = bkc_pthread_pool_prtapthr(file_p);
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, -1, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_prtdae_aftof_sfunc
 *****************************************************************************
 * parameters:
 *     arg_p:
 *         points to the param used in this func
 * return value:
 *
 * description:
 *     print the information of all the threads to
 *     the designated file throught a daemon in the background
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_pthread_prtdae_aftof_sfunc(void *dae_param_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    bkc_pthread_daemon_param_t *idae_param_p = NULL;

    BKC_FLOWCONTROL_TRY {
        int retcode = -1;
        struct timespec exptime;
        struct timespec remtime;

        idae_param_p = (bkc_pthread_daemon_param_t *)dae_param_p;
        BKC_FC_CT(idae_param_p != NULL, 1, NULL, NULL);

        exptime.tv_sec = idae_param_p->prt_interval;
        exptime.tv_nsec = 0;

        for (;;) {
            retcode = nanosleep(&exptime, &remtime);

            BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
            if (retcode == 0) {
                BKC_CR = bkc_pthread_pool_prtapthr(
                    idae_param_p->dae_file_p);
                BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 2, NULL, NULL);
                exptime.tv_sec = bkc_pthr_specdata.bkc_pthread_prt_interval;
                exptime.tv_nsec = 0;
            } else if (retcode == -1 && errno == EINTR) {
                BKC_WARN("nanosleep wake up by signal handler\n");
                exptime = remtime;
            } else {
                BKC_FC_ERRTHR(-1, NULL, "nanosleep err\n");
            }
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
        }
        BKC_FC_RT();
    }
    
    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }
    BKC_RETURN(NULL);
}

/*****************************************************************************
 *                  bkc_pthread_prt_aitf_daemon
 *****************************************************************************
 * parameters:
 *     file_p:
 *         the log file handle
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     print the information of all the threads to
 *     the designated file throught a daemon in the background
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_prt_aitf_daemon(FILE *file_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_pthread_attr_t pattr;
        static bkc_pthread_daemon_param_t dae_param;

        BKC_FC_CT(file_p != NULL, 1, -1, NULL);
        BKC_CR = bkc_pthread_attr_definit(&pattr);
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, -1, NULL);
        /*detach this thread*/
        pattr.is_detached = 1;
        BKC_MEMCPY(pattr.create_info.creator_name, "daemon thread", 
            BKC_PTHREAD_NAME_MAX - 1);
        BKC_MEMCPY(pattr.create_info.cr_func_name, 
            "bkc_pthread_prtdae_aftof_sfunc",  BKC_PTHREAD_NAME_MAX - 1);
        dae_param.dae_file_p = file_p;
        dae_param.target_pthr_p = NULL;
        dae_param.prt_interval = bkc_pthr_specdata.bkc_pthread_prt_interval;

        /*destroy the daemon if it is still running*/
        if (bkc_pthr_specdata.bkc_pthread_dae_p != NULL) {
            BKC_CR = bkc_pthread_cancel(
                bkc_pthr_specdata.bkc_pthread_dae_p, NULL);
            BKC_FC_CT(bkc_pthr_specdata.bkc_pthread_dae_p != NULL,
                1, -1, NULL);
        }

        bkc_pthr_specdata.bkc_pthread_dae_p = bkc_pthread_create(&pattr,
                bkc_pthread_prtdae_aftof_sfunc, &dae_param);
        BKC_FC_CT(bkc_pthr_specdata.bkc_pthread_dae_p != NULL, 1, -1, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_prtdae_spectof_sfunc
 *****************************************************************************
 * parameters:
 *     arg_p:
 *         points to the param used in this func
 * return value:
 *
 * description:
 *     print the information of all the threads to
 *     the designated file throught a daemon in the background
 ***************************** notes **************************************
 ****************************************************************************/
void *bkc_pthread_prtdae_spectof_sfunc(void *dae_param_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    bkc_pthread_daemon_param_t *idae_param_p = NULL;

    BKC_FLOWCONTROL_TRY {
        int retcode = -1;
        struct timespec exptime;
        struct timespec remtime;

        idae_param_p = (bkc_pthread_daemon_param_t *)dae_param_p;
        BKC_FC_CT(idae_param_p != NULL, 1, NULL, NULL);

        exptime.tv_sec = idae_param_p->prt_interval;
        exptime.tv_nsec = 0;

        for (;;) {
            retcode = nanosleep(&exptime, &remtime);

            BKC_PTHREAD_SPECDATA_ENTER(1, BKC_PTHREAD_RERR, NULL);
            if (retcode == 0) {
                BKC_CR = bkc_pthread_prt_specitem_tfile(
                    idae_param_p->target_pthr_p, idae_param_p->dae_file_p);
                BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, NULL, NULL);

                exptime.tv_sec = bkc_pthr_specdata.bkc_pthread_prt_interval;
                exptime.tv_nsec = 0;
            } else if (retcode == -1 && errno == EINTR) {
                BKC_WARN("nanosleep wake up by signal handler\n");
                exptime = remtime;
            } else {
                BKC_FC_ERRTHR(-1, NULL, "nanosleep err\n");
            }
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
        }
        BKC_FC_RT();
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_PTHREAD_SPECDATA_LEAVE(1, BKC_PTHREAD_RERR, NULL);
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_RETURN(NULL);
}

/*****************************************************************************
 *                  bkc_pthread_prt_spectof_daemon
 *****************************************************************************
 * parameters:
 *     file_p:
 *         the log file handle
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     print the information of all the threads to
 *     the designated file throught a daemon in the background
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_prt_spectof_daemon(bkc_pthread_t *pthr_p, FILE *file_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_pthread_attr_t pattr;
        static bkc_pthread_daemon_param_t dae_param;

        BKC_FC_CT(pthr_p != NULL && file_p != NULL, 1, -1, NULL);
        BKC_CR = bkc_pthread_attr_definit(&pattr);
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, -1, NULL);

        BKC_MEMCPY(pattr.create_info.creator_name, "daemon thread", 
            BKC_PTHREAD_NAME_MAX - 1);
        BKC_MEMCPY(pattr.create_info.cr_func_name, 
            "bkc_pthread_prtdae_aftof_sfunc",  BKC_PTHREAD_NAME_MAX - 1);
        /*detach this thread*/
        pattr.is_detached = 1;
        dae_param.dae_file_p = file_p;
        dae_param.target_pthr_p = pthr_p;
        dae_param.prt_interval = bkc_pthr_specdata.bkc_pthread_prt_interval;

        /*destroy the daemon if it is still running*/
        if (bkc_pthr_specdata.bkc_pthread_dae_p != NULL) {
            BKC_CR = bkc_pthread_cancel(
                bkc_pthr_specdata.bkc_pthread_dae_p, NULL);
            BKC_FC_CT(bkc_pthr_specdata.bkc_pthread_dae_p != NULL,
                1, -1, NULL);
        }

        bkc_pthr_specdata.bkc_pthread_dae_p = bkc_pthread_create(&pattr,
            bkc_pthread_prtdae_spectof_sfunc, &dae_param);
        BKC_FC_CT(bkc_pthr_specdata.bkc_pthread_dae_p != NULL, 1, -1, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_prt_specitem_tfile
 *****************************************************************************
 * parameters:
 *     file_p:
 *         the log file handle
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     print the information of specific thread, this is just a capsulation
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_prt_specitem_tfile(bkc_pthread_t *pthr_p, FILE *file_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(file_p != NULL && pthr_p != NULL, 1, -1, NULL);
        BKC_CR = bkc_pthread_pool_prtspthr(pthr_p, file_p);
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, -1, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_pool_prtspthr
 *****************************************************************************
 * parameters:
 *     pthr_p:
 *         points to the thread of which the info will be dumped
 *     file_p:
 *         points to the file which the info of the tread will be dumped to
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     print the single thread in the pool
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_pool_prtspthr(bkc_pthread_t *pthr_p, FILE *file_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(pthr_p != NULL, 1, BKC_PTHREAD_RERR, NULL);

        BKC_FPRINTF(file_p, "pthr_p index is %p\n", pthr_p);
        BKC_FPRINTF(file_p, "pthr_p->status is %s\n",
            pthr_p->status == BKC_PTHREAD_STATUS_PAUSED ?
            "BKC_PTHREAD_STATUS_PAUSED" :
            pthr_p->status == BKC_PTHREAD_STATUS_DESTROYED ?
            "BKC_PTHREAD_STATUS_DESTROYED" : "BKC_PTHREAD_STATUS_ACTIVE");

        BKC_FPRINTF(file_p, "pthr_p->pthr_des.pth is %p\n",
            pthr_p->pthr_des.pth);

        BKC_FPRINTF(file_p, "pthr_p->create_info.creator_name is %s\n",
            pthr_p->create_info.creator_name);
        BKC_FPRINTF(file_p, "pthr_p->create_info.cr_file_name is %s\n",
            pthr_p->create_info.cr_file_name);
        BKC_FPRINTF(file_p, "pthr_p->create_info.cr_func_name is %s\n",
            pthr_p->create_info.cr_func_name);
        BKC_FPRINTF(file_p, "pthr_p->create_info.cr_line is %u\n",
            pthr_p->create_info.cr_line);

        BKC_FPRINTF(file_p, "pthr_p->time_info.create_time is %s",
            ctime(&(pthr_p->time_info.create_time)));
        BKC_FPRINTF(file_p, "pthr_p->time_info.destroy_time is %s",
            ctime(&(pthr_p->time_info.destroy_time)));

        BKC_FPRINTF(file_p, "pthr_p->priority is %d\n", pthr_p->priority);
        BKC_FPRINTF(file_p, "pthr_p->is_detached is %d\n", 
            pthr_p->is_detached);
        BKC_FPRINTF(file_p, "\n");
    }

    BKC_RETURN(BKC_FNRET);
}

/*****************************************************************************
 *                  bkc_pthread_pool_prtapthr
 *****************************************************************************
 * parameters:
 *     file_p:
 *         points to the file which the info of the tread will be dumped to
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     print all the thread in the pool
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_pool_prtapthr(FILE *file_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        int loop = 0;
        bkc_dlist_operator_t op;
        BKC_FC_CT(file_p != NULL, 1, BKC_PTHREAD_RERR, NULL);

        BKC_FPRINTF(file_p,"######################################\n");
        BKC_FPRINTF(file_p, "bkc_pthr_pool.max_cpthr_num is %d \n",
            bkc_pthr_specdata.bkc_pthr_pool.max_cpthr_num);
        BKC_FPRINTF(file_p, "bkc_pthr_pool.pthr_bulletin.total_pthnum "
            "is %d\n",
            bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.total_pthnum);
        BKC_FPRINTF(file_p,
            "bkc_pthr_pool.pthr_bulletin.cur_active_num is %d\n",
            bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_active_num);
        BKC_FPRINTF(file_p, "bkc_pthr_pool.pthr_bulletin.active_max "
            "is %d\n",
            bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.active_max);
        BKC_FPRINTF(file_p, "bkc_pthr_pool.pthr_bulletin.cur_joined_num "
            "is %d\n",
            bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_joined_num);
        BKC_FPRINTF(file_p, "bkc_pthr_pool.pthr_bulletin.cur_paused_num "
            "is %d\n",
            bkc_pthr_specdata.bkc_pthr_pool.pthr_bulletin.cur_paused_num);


        BKC_FPRINTF(file_p,"\n");
        BKC_FPRINTF(file_p,"pthread dlist in bkc_pthr_pool\n");
        /*traverse all the nodes*/
        BKC_PTHREAD_DEFINIT_DLOP(&op, 1, BKC_PTHREAD_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_TRAVERSE_NODE;
        op.clue_p = file_p;
        op.ophdlerset.unode_traverse = bkc_pthread_node_traverse_func;
        BKC_PTHREAD_OPERATE_DLIST(
            bkc_pthr_specdata.bkc_pthr_pool.pthr_list_p,
            &op, 1, BKC_PTHREAD_RERR, NULL);
        BKC_FPRINTF(file_p,"######################################\n");

    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_pthread_node_traverse_func
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     traversing handler for printing the thread node in dlist
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_pthread_node_traverse_func(void *unode_p, void *param_p)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);
    bkc_pthread_t *pthr_p = NULL;
    FILE *ifile_p = NULL;

    BKC_FLOWCONTROL_TRY {
        BKC_FC_CT(unode_p != NULL && param_p != NULL, 1,
            BKC_PTHREAD_RERR, NULL);
        pthr_p = (bkc_pthread_t *)unode_p;
        ifile_p = (FILE *)param_p;
        BKC_CR = bkc_pthread_pool_prtspthr(pthr_p, ifile_p);
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, -1, NULL);
    }

    BKC_RETURN(0);
}

/*****************************************************************************
 *                  bkc_pthread_watchmm
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_PTHREAD_RSUC: sucessfull return
 *     BKC_PTHREAD_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func can show the memory infomation about this module
 *     is in debugging mode in which BKC_PTHREAD_SPF_RELEASE is not
 *     defined in bkc_pthread_spf.h. if BKC_PTHREAD_SPF_RELEASE is defined
 *     in bkc_pthread_spf.h, this func will always show empty messages
 *
 *     this function is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module. in this case
 *     this func can be temporarily export to bkc_pthread.h for outer
 *     refering
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_pthread_watchmm(void)
{
    BKC_DITT(BKC_PTHREAD_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_CR = bkc_pthread_spf_watchmm();
        BKC_FC_CT(BKC_CR == BKC_PTHREAD_RSUC, 1, BKC_PTHREAD_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}
