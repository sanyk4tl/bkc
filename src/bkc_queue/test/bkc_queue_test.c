/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_queue
 *  file name: bkc_queue_test.c
 *  description:  facilities for testing the bkc_queue mod
 *************************************************************************
 *  log:
 *  >>>
 *  version:
 *      v1.0.1-v1.0.0
 *  time:
 *      Monday, May 5, 2009 (090504) at 1:10:27 AM UTC/GMT +8 hours
 *  location:
 *      Beijing, China on Earth
 *      latitude: 39.55 North, longtitude: 116.23 East
 *      international country code: + 86 (China)
 *      area code: 10
 *  weather:
 *      clear, mild
 *
 *  pgr_id:sizhe(email:tancng#gmail.com)
 *
 *  description: created
 ************************************************************************/
/*********************included files*************************************/
#include <stdio.h>
#include <stdlib.h>
#include "bkc_queue_tspf.h"
#include "bkc_queue.h"
#include "bkc_dlist.h"

/*********************definition, types & constants ***********************/
/*#define bkc_queue_T_WATCHM*/
/*********************variables******************************************/

/*********************prototype of local functions***********************/
static void bkc_queue_t_cppd(void);
static int bkc_queue_unode_prt_func(void *unode_p, void *param_p);
static int bkc_queue_isitem_cmp_func(void *unode_p, void *cmp_clue_p);
static void bkc_queue_t_rm(void);
static int bkc_queue_unode_destroy_func(void *unode_p, void *param_p);

#define BKC_QUEUE_T_RCK(cond) {\
    if (!(cond)) {\
        printf("\nck faild, file %s, func %s, line %d\n",\
            __FILE__, __func__, __LINE__);\
    }\
}

/*********************implementation of open and local functions*********/
int bkc_queue_unode_prt_func(void *unode_p, void *param_p)
{
    /*we print nothing*/
    printf("in prt mode, unode_p is %p\n", unode_p);
    return 0;
}

int bkc_queue_isitem_cmp_func(void *unode_p, void *cmp_clue_p)
{
    int ret = -1;

    printf("bkc_queue_isitem_cmp_func is called\n");

    if (unode_p == 0xFFFFFFFF) {
        printf("0xFFFFFFFF is found\n");
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}

int bkc_queue_unode_destroy_func(void *unode_p, void *param_p)
{
    printf("in the destroy func\n");
    printf("unode_p is %p, param_p is %p\n", unode_p, param_p);
    return 0;
}

/*************************************************************************
 *                  bkc_queue_t_rm
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test remove
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_queue_t_rm(void)
{
    bkc_queue_t *queue_p = NULL;
    void *dequeued_node_p = NULL;
    int ret = -1;
    unsigned int nodenum = 0;
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;

    dlist_p = bkc_dlist_create(&op);
    queue_p = bkc_queue_create();

    ret = bkc_queue_enqueue(queue_p, 0xFFFFFFFF);
    BKC_QUEUE_T_RCK(ret == 0);
    ret = bkc_queue_enqueue(queue_p, 0xFFFFFFFE);
    BKC_QUEUE_T_RCK(ret == 0);
    ret = bkc_queue_enqueue(queue_p, 0xFFFFFFFD);
    BKC_QUEUE_T_RCK(ret == 0);
    ret = bkc_queue_print(queue_p, bkc_queue_unode_prt_func, NULL);
    BKC_QUEUE_T_RCK(ret == 0);

    ret = bkc_queue_remove(queue_p, bkc_queue_isitem_cmp_func, NULL, dlist_p);
    BKC_QUEUE_T_RCK(ret == 0);
    ret = bkc_queue_print(queue_p, bkc_queue_unode_prt_func, NULL);
    BKC_QUEUE_T_RCK(ret == 0);

#if 1
    printf("the deleted node list \n");
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_queue_unode_prt_func;
    ret = bkc_dlist_operate(dlist_p, &op);
    BKC_QUEUE_T_RCK(ret == 0);
#endif

#if 1
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.clue_p = NULL;//can be any param for
        //bkc_dlist_t_unode_destroy_func
    op.ophdlerset.unode_destroy = bkc_queue_unode_destroy_func;
    ret = bkc_dlist_destroy(dlist_p, &op);
    BKC_QUEUE_T_RCK(ret == 0);
#endif

    ret = bkc_queue_dequeue(queue_p, &dequeued_node_p);
    BKC_QUEUE_T_RCK(ret == 0);
    printf("dequeued node is %x\n", dequeued_node_p);

    ret = bkc_queue_print(queue_p, bkc_queue_unode_prt_func, NULL);
    BKC_QUEUE_T_RCK(ret == 0);

    ret = bkc_queue_destroy(queue_p, bkc_queue_unode_destroy_func, NULL);
    BKC_QUEUE_T_RCK(ret == 0);

    bkc_queue_watchmm();
}

/*************************************************************************
 *                  bkc_queue_t_cppd
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create, enqueue, dequeue and destroy
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_queue_t_cppd(void)
{
    bkc_queue_t *queue_p = NULL;
    void *dequeued_node_p = NULL;
    int ret = -1;
    unsigned int nodenum = 0;

    queue_p = bkc_queue_create();

    ret = bkc_queue_enqueue(queue_p, 0xFFFFFFFF);
    BKC_QUEUE_T_RCK(ret == 0);
    ret = bkc_queue_enqueue(queue_p, 0xFFFFFFFE);
    BKC_QUEUE_T_RCK(ret == 0);
    ret = bkc_queue_enqueue(queue_p, 0xFFFFFFFD);
    BKC_QUEUE_T_RCK(ret == 0);
    ret = bkc_queue_enqueue(queue_p, 0xFFFFFFFC);
    BKC_QUEUE_T_RCK(ret == 0);
    ret = bkc_queue_enqueue(queue_p, 0xFFFFFFFB);
    BKC_QUEUE_T_RCK(ret == 0);
    ret = bkc_queue_enqueue(queue_p, 0xFFFFFFFA);
    BKC_QUEUE_T_RCK(ret == 0);

    ret = bkc_queue_print(queue_p, bkc_queue_unode_prt_func, NULL);
    BKC_QUEUE_T_RCK(ret == 0);

    ret = bkc_queue_getnnum(queue_p, &nodenum);
    BKC_QUEUE_T_RCK(ret == 0);
    printf("the nodenum is %d\n", nodenum);

    ret = bkc_queue_dequeue(queue_p, &dequeued_node_p);
    BKC_QUEUE_T_RCK(ret == 0);
    printf("dequeued node is %x\n", dequeued_node_p);

    ret = bkc_queue_dequeue(queue_p, &dequeued_node_p);
    BKC_QUEUE_T_RCK(ret == 0);
    printf("dequeued node is %x\n", dequeued_node_p);

    ret = bkc_queue_dequeue(queue_p, &dequeued_node_p);
    BKC_QUEUE_T_RCK(ret == 0);
    printf("dequeued node is %x\n", dequeued_node_p);

    ret = bkc_queue_dequeue(queue_p, &dequeued_node_p);
    BKC_QUEUE_T_RCK(ret == 0);
    printf("dequeued node is %x\n", dequeued_node_p);

    ret = bkc_queue_getnnum(queue_p, &nodenum);
    BKC_QUEUE_T_RCK(ret == 0);
    printf("the nodenum is %d\n", nodenum);

    ret = bkc_queue_dequeue(queue_p, &dequeued_node_p);
    BKC_QUEUE_T_RCK(ret == 0);
    printf("dequeued node is %x\n", dequeued_node_p);

    ret = bkc_queue_dequeue(queue_p, &dequeued_node_p);
    BKC_QUEUE_T_RCK(ret == 0);
    printf("dequeued node is %x\n", dequeued_node_p);

    dequeued_node_p = NULL;
    ret = bkc_queue_dequeue(queue_p, &dequeued_node_p);
    /*here ck must be failed*/
    BKC_QUEUE_T_RCK(ret == 0);
    printf("dequeued node is %x\n", dequeued_node_p);

    bkc_queue_destroy(queue_p, bkc_queue_unode_destroy_func, NULL);

    bkc_queue_watchmm();
}

/*************************************************************************
 *                  main
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented
 *****************************  Notes  *****************************
 ************************************************************************/
int main(int arc, char *argv[])
{
    /*bkc_queue_t_cppd();*/
    bkc_queue_t_rm();
    return 0;
}
