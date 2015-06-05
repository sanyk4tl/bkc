/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_store
 *  file name: bkc_store.h
 *  description: the basic store facility supplied in bkc
 *****************************************************************************
 *  black board:
 *  >
 *  a store is just a description of a space for storing, such as a file,
 *  a segment of the flash or hard disk, etc.
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
#ifndef BKC_STORE_H
#define BKC_STORE_H

/*********************included files*****************************************/

/*********************definition, types & constants *************************/
/*ret value of funcs in bkc_store*/
#define BKC_STORE_RSUC   (0)
#define BKC_STORE_RERR   (-1)

/*operation code*/
typedef enum bkc_store_opcode_e {
    BKC_STORE_OPCODE_EMPTY,
    BKC_STORE_OPCODE_READ,
    BKC_STORE_OPCODE_WRITE
} bkc_store_opcode_m;

/*operation handler for each store*/
/*open handler
  return value:
  not NULL: points to the successfully opened physical image,
      such as a file, etc.
  other means error*/
typedef void *(*bkc_store_open_func_t)(void *main_param_p, void *sup_param_p);

/*close handler
  return value:
  0 means success,
  other means error*/
typedef int (*bkc_store_close_func_t)(void *main_param_p);

/*read handler
  return value:
  large than or equal to 0: the successfull read bytes
  other means error*/
typedef int (*bkc_store_read_func_t)(void *buf_p, int size, int
    nmemb, void *pi_p);

/*write handler
  return value:
  large than or equal to 0: the successfull written bytes
  other means error*/
typedef int (*bkc_store_write_func_t)(void *buf_p, int size, int
    nmemb, void *pi_p);

/*create(make) physical image handler
  return value:
  0: successful return
  other means error*/
typedef int (*bkc_store_mk_pi_func_t)(void *main_param_p, void *sup_param_p);

/*remove physical image handler
  return value:
  0: successful return
  other means error*/
typedef int (*bkc_store_rm_pi_func_t)(void *main_param_p, void *sup_param_p);

/*types forward declaration in bkc_store module*/
typedef struct bkc_store_operator_tag_t   bkc_store_operator_t;
typedef struct bkc_store_ophdlerset_tag_t bkc_store_ophdlerset_t;
typedef struct bkc_store_tag_t bkc_store_t;

/*operator for the store structure*/
struct bkc_store_operator_tag_t {
    /**/
};

/*store operation handler set*/
struct bkc_store_ophdlerset_tag_t {
    bkc_store_open_func_t   open_func;
    bkc_store_close_func_t  close_func;
    bkc_store_mk_pi_func_t  mk_pi_func;
    bkc_store_rm_pi_func_t  rm_pi_func;
};

/*********************prototype of open functions****************************/
/*****************************************************************************
 *                  bkc_store_open
 *****************************************************************************
 * parameters:
 *     op_p:
 *       points to the operator that contains the
 *       create-info of a store
 * return value:
 *     Not NULL:
 *       points to the sucessfully created store
 *     NULL: abnormal return
 * description:
 *     caller will passes the create-info to this function
 *     through op_p.
 *
 *     if creating failed, the return value will be NULL,
 *     and will get an error message
 ****************************** notes *************************************
 ****************************************************************************/
bkc_store_t *bkc_store_open(bkc_store_operator_t *op_p);

/*****************************************************************************
 *                  bkc_store_close
 *****************************************************************************
 * parameters:
 *     store_p:
 *       points to the store that need to be closed, which is opened by
 *       bkc_store_open()
 *     op_p:
 *       points to the operator that contains the
 *       close-info of the store
 * return value:
 *     BKC_STORE_RSUC: sucessfull return
 *     BKC_STORE_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     close the store designated by store_p, and store_p can not be
 *     used any more
 *
 *     caller will passes the close-info through op_p
 *
 *     if closing failed, the return value will not be
 *     BKC_DLIST_RSUC, and will get an error message
 ****************************** notes *************************************
 ****************************************************************************/
int bkc_store_close(bkc_store_t *store_p, bkc_store_operator_t *op_p);

/*****************************************************************************
 *                  bkc_store_mk_pi
 *****************************************************************************
 * parameters:
 *     op_p:
 *       points to the operator that contains the create-info of the physical
 *       image for a store. and the create-info is also called make-info
 * return value:
 *     BKC_STORE_RSUC: sucessfull return
 *     BKC_STORE_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to create a physical image for a store.
 *     such as create a file directly in the file system without open it,
 *     or get a raw flash segment for later opening, reading and writing
 *
 *     if create failed, the return value will not be BKC_STORE_RSUC,
 *     and will get an error message
 ****************************** notes *************************************
 ****************************************************************************/
int bkc_store_mk_pi(bkc_store_operator_t *op_p);

/*****************************************************************************
 *                  bkc_store_mk_pi
 *****************************************************************************
 * parameters:
 *     op_p:
 *       points to the operator that contains the remove-info of the physical
 *       image for a store.
 * return value:
 *     BKC_STORE_RSUC: sucessfull return
 *     BKC_STORE_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to remove a physical image for a store.
 *     such as remove a file directly in the file system without open it,
 *     or release a raw flash segment which got by bkc_store_mk_pi().
 *
 *     if remove failed, the return value will not be BKC_STORE_RSUC,
 *     and will get an error message
 ****************************** notes *************************************
 ****************************************************************************/
int bkc_store_rm_pi(bkc_store_operator_t *op_p);

/*****************************************************************************
 *                  bkc_store_operate
 *****************************************************************************
 * parameters:
 *     store_p:
 *       points to the store which will be manipulated according
 *       to op_p
 *     op_p: points to the bkc_store_operator_t that desigates the
 *       specific manipulation applied to this store
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     store_p is opened by bkc_store_open, and can be closed by
 *     bkc_dlist_close.
 *
 *     specific operation code supported currently are as follows:
 *     BKC_STORE_OPCODE_READ
 *     BKC_STORE_OPCODE_WRITE
 *
 *     if operation failed, the return value will not be BKC_STORE_RSUC,
 *     and will get an error message
 ****************************** notes *************************************
 ****************************************************************************/
int bkc_store_operate(bkc_store_t *store_p, bkc_store_operator_t *op_p);

/*****************************************************************************
 *                  bkc_store_watchmm
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_STORE_RSUC: sucessfull return
 *     BKC_STORE_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_STORE_SPF_RELEASE is not
 *     defined in bkc_store_spf.h) this func will always show
 *     empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 ****************************** notes *************************************
 ****************************************************************************/
int bkc_store_watchmm(void);

#endif

