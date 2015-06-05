/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_queue
 *  file name: bkc_queue.h
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
 *  pgr_id: sizhe(email:tancng#gmail.com) *
 *  description: created
 ************************************************************************/
#ifndef BKC_QUEUE_H
#define BKC_QUEUE_H

/*********************included files*************************************/
#include "bkc_dlist.h"

/*********************definition, types & constants ***********************/
/*ret value of funcs in bkc_queue*/
#define BKC_QUEUE_REMPTY     (1)
#define BKC_QUEUE_RSUC       (0)
#define BKC_QUEUE_RERR       (-1)
#define BKC_QUEUE_NOTFUNODE  (-3)

/*types forward declaration in bkc_queue module*/
typedef struct bkc_queue_tag_t bkc_queue_t;
/*the compare func for bkc_queue_remove*/
typedef bkc_queue_isitem_cmp_func_t(void *unode_p, void *cmp_clue_p);
/*the node destroy func for bkc_queue destroy
  return value:
  0 means success,
  other means error*/
typedef int (*bkc_queue_unode_destroy_func_t)(void *unode_p, void *param_p);
/*the node print function*/
typedef int (*bkc_queue_unode_prt_func_t)(void *unode_p, void *param_p);

/*********************prototype of open functions************************/
/*************************************************************************
 *                  bkc_queue_create
 *************************************************************************
 * parameters:
 * return value:
 *     Not NULL: points to the sucessfully created queue
 *     NULL: abnormal return
 * description:
 *     create a queue
 *****************************  Notes  *****************************
 ************************************************************************/
bkc_queue_t *bkc_queue_create(void);

/*************************************************************************
 *                  bkc_queue_destroy
 *************************************************************************
 * parameters:
 *     queue_p:
 *         points to the queue that need to be destroyed which is created by
 *         bkc_queue_create
 *     dsy_func:
 *         user node destroy func applied to each node remains in the queue
 *     param_p:
 *         parameter for the destroy func designated by dsy_func
 * return value:
 *     BKC_QUEUE_RSUC: successfull return
 *     BKC_QUEUE_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     destroy the queue which was created by bkc_queue_create
 *
 *     queue_p can not be NULL. if queue_p == NULL, return value will be
 *     BKC_QUEUE_RERR, and a error msg will be prompted
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_queue_destroy(bkc_queue_t *queue_p,
        bkc_queue_unode_destroy_func_t dsy_func, void *param_p);

/*************************************************************************
 *                  bkc_queue_push
 *************************************************************************
 * parameters:
 *     queue_p:
 *         points to the queue which the unode_p will be added to
 *     unode_p:
 *         pointer to the user node
 * return value:
 *     BKC_QUEUE_RSUC: successfull add the user node(unode_p)
 *         to the tail of the  queue_p
 *     BKC_QUEUE_RERR: erroneous return
 * description:
 *     add a user node to the tail of the queue,
 *     queue_p and unode_p can not be NULL. if queue_p == NULL or
 *     unode_p == NULL, return value will be
 *     BKC_QUEUE_RERR, and a error msg will be prompted
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_queue_enqueue(bkc_queue_t *queue_p, void *unode_p);

/*************************************************************************
 *                  bkc_queue_pop
 *************************************************************************
 * parameters:
 *     queue_p:
 *         points to the queue from which the head user node will be
 *         removed
 *     unode_pp:
 *         will contain the head user node pointer which will be removed
 * return value:
 *     BKC_QUEUE_RSUC: successfull remove the user node from the queue
 *     BKC_QUEUE_REMPTY: the queue is empty ,so remove no user node at all
 *     BKC_QUEUE_RERR: erroneous return
 * description:
 *     remove the user node from the head of the queue
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_queue_dequeue(bkc_queue_t *queue_p, void **unode_pp);

/*************************************************************************
 *                  bkc_queue_remove
 *************************************************************************
 * parameters:
 *     queue_p:
 *         points to the queue which was created by bkc_queue_create
 *     cmp_func:
 *         the compare function handler which the caller will passed in
 *         to designate which node need to dequeue specificly.
 *         if cmp_func(unode)== 0 then the unode in the queue will be removed
 *         if cmp_func is passed in as NULL, no user node will be removed
 *         at all, and a warning msg will be prompted
 *     cmp_clue_p: the compare clue for the cmp_func, if the cmp_func
 *         does not need a parameter, cmp_clue_p can be passed in as NULL
 *     dlist_p:
 *         if the ret of the bkc_queue_remove is BKC_QUEUE_RSUC, dlist_p
 *         will contain all the user nodes which has been removed from
 *         the queue, or else, the data in the dlist_p will be invalid.
 *         if dont want the removed user nodes , dlist_p can be
 *         passed in as NULL
 * return value:
 *     BKC_QUEUE_RSUC: successfull remove the numth user node in the  queue_p
 *     BKC_QUEUE_REMPTY: the queue does not have any user nodes
 *     BKC_QUEUE_NOTFUNODE: the user node designated by cmp_func does
 *         not exist in the queue, and the data in dlist_p is invalid
 *     BKC_QUEUE_RERR: erroneous return
 * description:
 *     remove the designated node from the queue_p,
 *     the wanted nodes which need to be removed from the queue will
 *     be designated by cmp_func(when cmp_func(unode)== 0 the user
 *     node will be the wanted node)
 *
 *     queue_p can not be NULL. if queue_p == NULL return value
 *     will be BKC_QUEUE_RERR, and a error msg will be prompted
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_queue_remove(bkc_queue_t *queue_p,
        bkc_queue_isitem_cmp_func_t cmp_func, void *cmp_clue_p,
        bkc_dlist_t *dlist_p);

/*************************************************************************
 *                  bkc_queue_reset
 *************************************************************************
 * parameters:
 *     queue_p:
 *         points to the queue which was created by bkc_queue_create
 * return value:
 *     BKC_QUEUE_RSUC: successfull reset the  queue_p
 *     BKC_QUEUE_RERR: erroneous return
 * description:
 *     reset the queue_p, and  the queue_p will be empty after
 *     calling this func
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_queue_reset(bkc_queue_t *queue_p);

/*************************************************************************
 *                  bkc_queue_getnnum
 *************************************************************************
 * parameters:
 *     queue_p:
 *         points to the queue which was created by bkc_queue_create
 *     num_p:
 *         used to return the number of the nodes contained  in this queue
 * return value:
 *     BKC_QUEUE_RSUC: successfull return
 *     BKC_QUEUE_RERR: erroneous return
 * description:
 *     get the number of the nodes in the queue.
 *     queue_p can not be NULL. if queue_p == NULL or
 *     unode_p == NULL, return value will be BKC_QUEUE_RERR,
 *     and a error msg will be prompted
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_queue_getnnum(bkc_queue_t *queue_p, unsigned int*num_p);

/*************************************************************************
 *                  bkc_queue_print
 *************************************************************************
 * parameters:
 *     queue_p:
 *         points to the queue which was created by bkc_queue_create
 *     prt_func:
 *         print function for each node
 *     param_p:
 *         parameter for prt_func
 * return value:
 *     BKC_QUEUE_RSUC: successfull return
 *     BKC_QUEUE_RERR: erroneous return
 * description:
 *     print the info of the queue to the termial,
 *     this func is usefull when debugging this module.
 *     queue_p can not be NULL. if queue_p == NULL or
 *     unode_p == NULL, return value will be BKC_QUEUE_RERR,
 *     and a error msg will be prompted
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_queue_print(bkc_queue_t *queue_p, bkc_queue_unode_prt_func_t prt_func,
        void *param_p);

#endif

