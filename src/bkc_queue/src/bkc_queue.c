/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_queue
 *  file name: bkc_queue.c
 *  description: the string facility supplied in bkc
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
/*********************included files*************************************/
#include <stdlib.h>
#include <stdio.h>
#include "bkc_queue.h"
#include "bkc_queue_spf.h"
#include "bkc_dlist.h"

/*********************definition, types & constants ***********************/
struct bkc_queue_tag_t {
    bkc_dlist_t *dlist_p;
};

/*********************variables******************************************/

/*********************prototype of local functions***********************/

/*********************implementation of open and local functions*********/
/*************************************************************************
 *                   bkc_queue_create
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
bkc_queue_t *bkc_queue_create(void)
{
    BKC_DITT(BKC_QUEUE_RSUC, 0);
    bkc_queue_t *queue_p = NULL;

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        queue_p = BKC_MALLOC(sizeof(bkc_queue_t));
        BKC_FC_CKSTHR(queue_p != NULL, 1, BKC_QUEUE_RERR, NULL);

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 2, BKC_QUEUE_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;

        BKC_RF(queue_p)->dlist_p = bkc_dlist_create(&op);
        BKC_FC_CKSTHR(BKC_RF(queue_p)->dlist_p != NULL,
            2, BKC_QUEUE_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            queue_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_SFREE(queue_p);
            queue_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_CKPRET(queue_p != NULL);
    BKC_RETURN(queue_p);
}

/*************************************************************************
 *                  bkc_queue_destroy
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_queue_destroy(bkc_queue_t *queue_p,
        bkc_queue_unode_destroy_func_t dsy_func, void *param_p)
{
    BKC_DITT(BKC_QUEUE_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        unsigned int node_num = 0;
        BKC_FC_CKSTHR(queue_p != NULL, 1, BKC_QUEUE_RERR, NULL);

        /*destroy the dlist member in queue_p*/
        BKC_FC_CKSTHR(BKC_RF(queue_p)->dlist_p != NULL,
            1, BKC_QUEUE_RERR, NULL);

        BKC_CKRET = bkc_queue_getnnum(queue_p, &node_num);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_QUEUE_RSUC, 1, BKC_QUEUE_RERR, NULL);
        /*alarm if the queue is not empty*/
        if (node_num != 0) {
            BKC_WARN("the queue is not empty when destroy\n");
        }

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
        if (dsy_func != NULL) {
            op.dsymode = BKC_DLIST_DM_DELUSERDATA;
            op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
            op.ophdlerset.unode_destroy = dsy_func;
            op.clue_p = param_p;
        } else {
            op.dsymode = BKC_DLIST_DM_NOT_DELUSERDATA;
            op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
        }
        BKC_CKRET = bkc_dlist_destroy(BKC_RF(queue_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
        BKC_RF(queue_p)->dlist_p = NULL;

        /*free the queue_p*/
        BKC_SFREE(queue_p);
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_queue_dequeue
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_queue_dequeue(bkc_queue_t *queue_p, void **unode_pp)
{
    BKC_DITT(BKC_QUEUE_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        unsigned int node_num = 0;
        void *fnode_p = NULL;

        BKC_FC_CKSTHR(queue_p != NULL && unode_pp != NULL,
            1, BKC_QUEUE_RERR, NULL);

        BKC_CKRET = bkc_queue_getnnum(queue_p, &node_num);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_QUEUE_RSUC, 1, BKC_QUEUE_RERR, NULL);

        if (node_num == 0) {
            BKC_WARN("queue is empty, please have a check\n");
            BKC_FC_SUCRET(BKC_QUEUE_REMPTY);
        }

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_REMOVE_HEAD;
        op.rmvmode = BKC_DLIST_RM_NOT_DELUSERDATA;
        BKC_CKRET = bkc_dlist_operate(BKC_RF(queue_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);

        fnode_p = op.runode_p;
        BKC_FC_CKSTHR(fnode_p != NULL , 1, BKC_QUEUE_RERR, NULL);

        *unode_pp = fnode_p;
    }

    BKC_MRETURN(BKC_FNRET, BKC_FNRET != BKC_QUEUE_RERR);
}

/*************************************************************************
 *                  bkc_queue_enqueue
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_queue_enqueue(bkc_queue_t *queue_p, void *unode_p)
{
    BKC_DITT(BKC_QUEUE_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CKSTHR(queue_p != NULL && unode_p != NULL,
            1, BKC_QUEUE_RERR, NULL);
        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        BKC_CKRET = bkc_dlist_operate(BKC_RF(queue_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_queue_remove
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_queue_remove(bkc_queue_t *queue_p,
        bkc_queue_isitem_cmp_func_t cmp_func, void *cmp_clue_p,
        bkc_dlist_t *dlist_p)
{
    BKC_DITT(BKC_QUEUE_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        /*op for adding*/
        bkc_dlist_operator_t op_fadd;
        /*op for delete*/
        bkc_dlist_operator_t op_fdelete;
        void *fnode_p = NULL;
        unsigned int node_num = 0;
        BKC_FC_CKSTHR(queue_p != NULL, 1, BKC_QUEUE_RERR, NULL);

        /*get node num*/
        BKC_CKRET = bkc_queue_getnnum(queue_p, &node_num);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_QUEUE_RSUC, 1, BKC_QUEUE_RERR, NULL);
        if (node_num == 0) {
            BKC_WARN("queue is empty, please have a check\n");
            BKC_FC_SUCRET(BKC_QUEUE_REMPTY);
        }

        if (cmp_func == NULL) {
            BKC_WARN("the cmp_func is NULL, no user node will be removed\n");
            BKC_FC_SUCRET(BKC_QUEUE_RSUC);
        }

        /*find all the user nodes which need to be removed, and add them
          to the dlist_p if dlist_p is not NULL*/
        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_FIND_NSPEC_NODE;
        op.clue_p = cmp_clue_p;
        op.tunode_p = NULL;
        op.ophdlerset.unode_isitem_cmp = cmp_func;
        BKC_CKRET = bkc_dlist_operate(queue_p->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);

        fnode_p = op.runode_p;
        while (fnode_p != NULL) {
            if (dlist_p != NULL) {
                BKC_CKRET = bkc_dlist_definit_op(&op_fadd);
                BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
                op_fadd.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
                op_fadd.tunode_p = fnode_p;
                BKC_CKRET = bkc_dlist_operate(dlist_p, &op_fadd);
                BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
            }

            BKC_CKRET = bkc_dlist_definit_op(&op_fdelete);
            BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
            op_fdelete.opcode = BKC_DLIST_OPCODE_REMOVE_NODE;
            op_fdelete.tunode_p = fnode_p;
            op_fdelete.clue_p = NULL;
            op_fdelete.rmvmode = BKC_DLIST_RM_NOT_DELUSERDATA;
            BKC_CKRET = bkc_dlist_operate(queue_p->dlist_p, &op_fdelete);
            BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC,
                1, BKC_QUEUE_RERR, NULL);

            BKC_CKRET = bkc_dlist_operate(queue_p->dlist_p, &op);
            BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC,
                1, BKC_QUEUE_RERR, NULL);
            fnode_p = op.runode_p;
        }
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_queue_reset
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_queue_reset(bkc_queue_t *queue_p)
{
    BKC_DITT(BKC_QUEUE_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CKSTHR(queue_p != NULL, 1, BKC_QUEUE_RERR, NULL);

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_REMOVE_ALL;
        op.rmvmode = BKC_DLIST_RM_NOT_DELUSERDATA;
        BKC_CKRET = bkc_dlist_operate(BKC_RF(queue_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_queue_reset
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_queue_getnnum(bkc_queue_t *queue_p, unsigned int*num_p)
{
    BKC_DITT(BKC_QUEUE_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CKSTHR(queue_p != NULL && num_p != NULL,
            1, BKC_QUEUE_RERR, NULL);

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_GET_NODE_NUM;
        BKC_CKRET = bkc_dlist_operate(BKC_RF(queue_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
        *num_p = op.nodenum;
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_queue_print
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_queue_print(bkc_queue_t *queue_p, bkc_queue_unode_prt_func_t prt_func,
        void *param_p)
{
    BKC_DITT(BKC_QUEUE_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CKSTHR(queue_p != NULL , 1, BKC_QUEUE_RERR, NULL);

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
        op.clue_p = param_p;
        op.ophdlerset.unode_prt = prt_func;
        BKC_CKRET = bkc_dlist_operate(BKC_RF(queue_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_QUEUE_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_queue_watchmm
 *************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_QUEUE_RSUC: sucessfull return
 *     BKC_QUEUE_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func can show the memory infomation about this module
 *     is in debugging mode in which BKC_QUEUE_SPF_RELEASE is not
 *     defined in bkc_queue_spf.h. if BKC_QUEUE_SPF_RELEASE is defined
 *     in bkc_queue_spf.h, this func will always show empty messages
 *
 *     this function is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module. in this case
 *     this func can be temporarily export to bkc_queue.h for outer
 *     refering
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_queue_watchmm(void)
{
    BKC_DITT(BKC_QUEUE_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_CKRET = bkc_queue_spf_watchmm();
        BKC_FC_CKSTHR(BKC_CKRET == BKC_QUEUE_RSUC, 1, BKC_QUEUE_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

