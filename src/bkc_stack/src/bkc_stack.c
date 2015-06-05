/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_stack
 *  file name: bkc_stack.c
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
#include "bkc_stack.h"
#include "bkc_stack_spf.h"
#include "bkc_dlist.h"

/*********************definition, types & constants ***********************/
struct bkc_stack_tag_t {
    bkc_dlist_t *dlist_p;
};

/*********************variables******************************************/

/*********************prototype of local functions***********************/
static int bkc_stack_remove(bkc_stack_t *stack_p, int num);

/*********************implementation of open and local functions*********/
/*************************************************************************
 *                   bkc_stack_create
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
bkc_stack_t *bkc_stack_create(void)
{
    BKC_DITT(BKC_STACK_RSUC, 0);
    bkc_stack_t *stack_p = NULL;

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        stack_p = BKC_MALLOC(sizeof(bkc_stack_t));
        BKC_FC_CKSTHR(stack_p != NULL, 1, BKC_STACK_RERR, NULL);

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 2, BKC_STACK_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;

        BKC_RF(stack_p)->dlist_p = bkc_dlist_create(&op);
        BKC_FC_CKSTHR(BKC_RF(stack_p)->dlist_p != NULL,
            2, BKC_STACK_RERR, NULL);
    }

    BKC_FLOWCONTROL_CATCH {
        BKC_FC_CASE (1) {
            stack_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_CASE (2) {
            BKC_SFREE(stack_p);
            stack_p = NULL;
            BKC_FC_BREAK;
        }
        BKC_FC_DEFAULT {
            BKC_FC_BREAK;
        }
    }

    BKC_CKPRET(stack_p != NULL);
    BKC_RETURN(stack_p);
}

/*************************************************************************
 *                  bkc_stack_destroy
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_stack_destroy(bkc_stack_t* stack_p,
         bkc_stack_unode_destroy_func_t dsy_func, void *param_p)
{
    BKC_DITT(BKC_STACK_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        unsigned int node_num = 0;
        BKC_FC_CKSTHR(stack_p != NULL, 1, BKC_STACK_RERR, NULL);

        /*destroy the dlist member in stack_p*/
        BKC_FC_CKSTHR(BKC_RF(stack_p)->dlist_p != NULL,
            1, BKC_STACK_RERR, NULL);

        BKC_CKRET = bkc_stack_getnnum(stack_p, &node_num);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_STACK_RSUC, 1, BKC_STACK_RERR, NULL);
        /*alarm if the stack is not empty*/
        if (node_num != 0) {
            BKC_WARN("the stack is not empty when destroy\n");
        }

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);
        if (dsy_func != NULL) {
            op.dsymode = BKC_DLIST_DM_DELUSERDATA;
            op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
            op.ophdlerset.unode_destroy = dsy_func;
            op.clue_p = param_p;
        } else {
            op.dsymode = BKC_DLIST_DM_NOT_DELUSERDATA;
            op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
        }
        BKC_CKRET = bkc_dlist_destroy(BKC_RF(stack_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);
        BKC_RF(stack_p)->dlist_p = NULL;

        /*free the stack_p*/
        BKC_SFREE(stack_p);
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_stack_pop
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_stack_pop(bkc_stack_t *stack_p, void **unode_pp)
{
    BKC_DITT(BKC_STACK_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        unsigned int node_num = 0;
        void *fnode_p = NULL;

        BKC_FC_CKSTHR(stack_p != NULL && unode_pp != NULL,
            1, BKC_STACK_RERR, NULL);

        BKC_CKRET = bkc_stack_getnnum(stack_p, &node_num);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_STACK_RSUC, 1, BKC_STACK_RERR, NULL);

        if (node_num == 0) {
            BKC_WARN("stack is empty, please have a check\n");
            BKC_FC_SUCRET(BKC_STACK_REMPTY);
        }

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_REMOVE_HEAD;
        op.rmvmode = BKC_DLIST_RM_NOT_DELUSERDATA;
        BKC_CKRET = bkc_dlist_operate(BKC_RF(stack_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);

        fnode_p = op.runode_p;
        BKC_FC_CKSTHR(fnode_p != NULL , 1, BKC_STACK_RERR, NULL);

        *unode_pp = fnode_p;
    }

    BKC_STACK_SPF_MRETURN(BKC_FNRET, BKC_FNRET != BKC_STACK_RERR);
}

/*************************************************************************
 *                  bkc_stack_push
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_stack_push(bkc_stack_t *stack_p, void *unode_p)
{
    BKC_DITT(BKC_STACK_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CKSTHR(stack_p != NULL && unode_p != NULL,
            1, BKC_STACK_RERR, NULL);
        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_ADD_HEAD_NODE;
        op.tunode_p = unode_p;
        BKC_CKRET = bkc_dlist_operate(BKC_RF(stack_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_stack_remove
 *************************************************************************
 * parameters:
 *     stack_p:
 *         points to the stack which was created by bkc_stack_create
 *     num:
 *         the index of the user node in the stack
 * return value:
 *     BKC_STACK_RSUC: successfull remove the numth user node in the  stack_p
 *     BKC_STACK_REMPTY: the numth node does not exist in this stack
 *     BKC_STACK_RERR: erroneous return
 * description:
 *     remove the num-th node from the stack_p,
 *     head node is the first node(index is 0), and the tail node is the
 *     last node(index is the number of nodes in the stack subtract 1)
 *
 *     this func is not used and pubed out yet, it is only reserved
 *     for later extention
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_stack_remove(bkc_stack_t *stack_p, int num)
{
    BKC_DITT(BKC_STACK_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        void *fnode_p = NULL;
        unsigned int node_num = 0;
        BKC_FC_CKSTHR(stack_p != NULL, 1, BKC_STACK_RERR, NULL);

        /*get node num*/
        BKC_CKRET = bkc_stack_getnnum(stack_p, &node_num);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_STACK_RSUC, 1, BKC_STACK_RERR, NULL);

        if (node_num == 0) {
            BKC_WARN("stack is empty, please have a check\n");
            BKC_FC_SUCRET(BKC_STACK_REMPTY);
        }

        BKC_FC_CKSTHR(num < node_num && num >= 0, 1, BKC_STACK_RERR,
            "num is not valide, please have a check\n");

        /*remove the num-th node*/
        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_REMOVE_N_NODE;
        op.rmvmode = BKC_DLIST_RM_NOT_DELUSERDATA;
        op.pos = num;
        BKC_CKRET = bkc_dlist_operate(BKC_RF(stack_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);

        fnode_p = op.runode_p;
        BKC_FC_CKSTHR(fnode_p != NULL, 1, BKC_STACK_RERR,
            "not found the node, please have a check\n");
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_stack_reset
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_stack_reset(bkc_stack_t *stack_p,
        bkc_stack_unode_destroy_func_t dsy_func, void *param_p)
{
    BKC_DITT(BKC_STACK_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CKSTHR(stack_p != NULL, 1, BKC_STACK_RERR, NULL);

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_REMOVE_ALL;
        if (dsy_func != NULL) {
            op.rmvmode = BKC_DLIST_RM_NOT_DELUSERDATA;
            op.clue_p = param_p;
            op.ophdlerset.unode_destroy = dsy_func;
        } else {
            op.rmvmode = BKC_DLIST_RM_DELUSERDATA;
        }
        BKC_CKRET = bkc_dlist_operate(BKC_RF(stack_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);

    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_stack_getnnum
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_stack_getnnum(bkc_stack_t *stack_p, unsigned int*num_p)
{
    BKC_DITT(BKC_STACK_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CKSTHR(stack_p != NULL && num_p != NULL,
            1, BKC_STACK_RERR, NULL);

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_GET_NODE_NUM;
        BKC_CKRET = bkc_dlist_operate(BKC_RF(stack_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);
        *num_p = op.nodenum;
    }

    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_stack_print
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_stack_print(bkc_stack_t *stack_p, bkc_stack_unode_prt_func_t prt_func,
        void *param_p)
{
    BKC_DITT(BKC_STACK_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        bkc_dlist_operator_t op;
        BKC_FC_CKSTHR(stack_p != NULL , 1, BKC_STACK_RERR, NULL);

        BKC_CKRET = bkc_dlist_definit_op(&op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);
        op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
        op.clue_p = param_p;
        op.ophdlerset.unode_prt = prt_func;
        BKC_CKRET = bkc_dlist_operate(BKC_RF(stack_p)->dlist_p, &op);
        BKC_FC_CKSTHR(BKC_CKRET == BKC_DLIST_RSUC, 1, BKC_STACK_RERR, NULL);
    }
    BKC_RETURN(BKC_FNRET);
}

/*************************************************************************
 *                  bkc_stack_watchmm
 *************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_STACK_RSUC: sucessfull return
 *     BKC_STACK_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func can show the memory infomation about this module
 *     is in debugging mode in which BKC_STACK_SPF_RELEASE is not
 *     defined in bkc_stack_spf.h. if BKC_STACK_SPF_RELEASE is defined
 *     in bkc_stack_spf.h, this func will always show empty messages
 *
 *     this function is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module. in this case
 *     this func can be temporarily export to bkc_stack.h for outer
 *     refering
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_stack_watchmm(void)
{
    BKC_DITT(BKC_STACK_RSUC, 0);

    BKC_FLOWCONTROL_TRY {
        BKC_CKRET = bkc_stack_spf_watchmm();
        BKC_FC_CKSTHR(BKC_CKRET == BKC_STACK_RSUC, 1, BKC_STACK_RERR, NULL);
    }

    BKC_RETURN(BKC_FNRET);
}
