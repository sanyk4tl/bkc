/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_stack
 *  file name: bkc_stack.h
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
#ifndef BKC_STACK_H
#define BKC_STACK_H

/*********************included files*************************************/

/*********************definition, types & constants ***********************/
/*ret value of funcs in bkc_stack*/
#define BKC_STACK_RSUC   (0)
#define BKC_STACK_RERR   (-1)
/*empty is a right return*/
#define BKC_STACK_REMPTY  (1)

/*types forward declaration in bkc_stack module*/
typedef struct bkc_stack_tag_t bkc_stack_t;

/*the node destroy func for bkc_stack destroy
  return value:
  0 means success,
  other means error*/
typedef int (*bkc_stack_unode_destroy_func_t)(void *unode_p, void *param_p);

/*the node print function*/
typedef int (*bkc_stack_unode_prt_func_t)(void *unode_p, void *param_p);
/*********************prototype of open functions************************/
/*************************************************************************
 *                  bkc_stack_create
 *************************************************************************
 * parameters:
 * return value:
 *     Not NULL: points to the sucessfully created stack
 *     NULL: abnormal return
 * description:
 *     create a stack
 *****************************  Notes  *****************************
 ************************************************************************/
bkc_stack_t *bkc_stack_create(void);

/*************************************************************************
 *                  bkc_stack_destroy
 *************************************************************************
 * parameters:
 *     stack_p:
 *         points to the stack that need to be destroyed which is created by
 *         bkc_stack_create
 *     dsy_func:
 *         user node destroy func applied to each node remains in the stack
 *     param_p:
 *         parameter for the destroy func designated by dsy_func
 * return value:
 *     BKC_STACK_RSUC: successfull return
 *     BKC_STACK_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     destroy the stack which was created by bkc_stack_create
 *
 *     stack_p can not be NULL. if stack_p == NULL, return value will be
 *     BKC_STACK_RERR, and a error msg will be prompted
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_stack_destroy(bkc_stack_t* stack_p,
         bkc_stack_unode_destroy_func_t dsy_func, void *param_p);

/*************************************************************************
 *                  bkc_stack_pop
 *************************************************************************
 * parameters:
 *     stack_p:
 *         points to the stack from which the head user node will be poped
 *     unode_pp:
 *         will contain the head user node pointer which will be poped
 * return value:
 *     BKC_STACK_RSUC: successfull poped the user node to unode_pp
 *     BKC_STACK_REMPTY: the stack is empty ,so pop no user node at all
 *     BKC_STACK_RERR: erroneous return
 * description:
 *     pop a user node from the head of the stack
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_stack_pop(bkc_stack_t *stack_p, void **unode_pp);

/*************************************************************************
 *                  bkc_stack_push
 *************************************************************************
 * parameters:
 *     stack_p:
 *         points to the stack which the unode_p will be pushed to
 *     unode_p:
 *         pointer to the user node
 * return value:
 *     BKC_STACK_RSUC: successfull push the user node(unode_p)
 *         to the head of the  stack_p
 *     BKC_STACK_RERR: erroneous return
 * description:
 *     push a user node to the head of the stack,
 *     stack_p and unode_p can not be NULL. if stack_p == NULL or
 *     unode_p == NULL, return value will be
 *     BKC_STACK_RERR, and a error msg will be prompted
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_stack_push(bkc_stack_t *stack_p, void *unode_p);

/*************************************************************************
 *                  bkc_stack_reset
 *************************************************************************
 * parameters:
 *     stack_p:
 *         points to the stack which was created by bkc_stack_create
 *     dsy_func:
 *         user node destroy func applied to each node remains in the stack
 *     param_p:
 *         parameter for the destroy func designated by dsy_func
 * return value:
 *     BKC_STACK_RSUC: successfull reset the  stack_p
 *     BKC_STACK_RERR: erroneous return
 * description:
 *     reset the stack_p, and  the stack_p will be empty after
 *     calling this func
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_stack_reset(bkc_stack_t *stack_p,
        bkc_stack_unode_destroy_func_t dsy_func, void *param_p);

/*************************************************************************
 *                  bkc_stack_getnnum
 *************************************************************************
 * parameters:
 *     stack_p:
 *         points to the stack which was created by bkc_stack_create
 *     num_p:
 *         used to return the number of the nodes contained  in this stack
 * return value:
 *     BKC_STACK_RSUC: successfull return
 *     BKC_STACK_RERR: erroneous return
 * description:
 *     get the number of the nodes in the stack.
 *     stack_p can not be NULL. if stack_p == NULL or
 *     unode_p == NULL, return value will be BKC_STACK_RERR,
 *     and a error msg will be prompted
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_stack_getnnum(bkc_stack_t *stack_p, unsigned int*num_p);

/*************************************************************************
 *                  bkc_stack_print
 *************************************************************************
 * parameters:
 *     stack_p:
 *         points to the stack which was created by bkc_stack_create
 *     prt_func:
 *         print function for each user node
 *     param_p:
 *         parameter for prt_func
 * return value:
 *     BKC_STACK_RSUC: successfull return
 *     BKC_STACK_RERR: erroneous return
 * description:
 *     print the info of the stack to the termial,
 *     this func is usefull when debugging this module.
 *     stack_p can not be NULL. if stack_p == NULL or
 *     unode_p == NULL, return value will be BKC_STACK_RERR,
 *     and a error msg will be prompted
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_stack_print(bkc_stack_t *stack_p, bkc_stack_unode_prt_func_t prt_func,
        void *param_p);

/*************************************************************************
 *                  bkc_stack_watchmm
 *************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_STACK_RSUC: sucessfull return
 *     BKC_STACK_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_STACK_SPF_RELEASE is not
 *     defined in bkc_stack_spf.h) this func will always show empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_stack_watchmm(void);

#endif

