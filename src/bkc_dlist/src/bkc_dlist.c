/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_dlist
 *  file name: bkc_dlist.c
 *  description: N/A
 *************************************************************************
 *  log:
 *  >>>
 *  version:
 *      v1.0.0-v1.0.0
 *  time:
 *      Thursday, April 16, 2009 (090416) at 12:45:07 AM UTC/GMT +8 hours
 *  location:
 *      ChongQing, China on Earth
 *      latitude: 29.3 North, longtitude: 106.35 East
 *      international country code: + 86 (China)
 *      area code: 23
 *  weather:
 *      clear, mild
 *      temperature: 70 F (comfort level is 70 F)
 *      wind: 3 mph from 190 south by west
 *
 *  pgr_id:sizhe(email:tancng#gmail.com)
 *
 *  description: created
 ************************************************************************/
/*********************included files*************************************/
#include <stdlib.h>
#include <stdio.h>
#include "bkc_dlist.h"
#include "bkc_dlist_spf.h"

/*********************definition, types & constants ***********************/
/*types forward declaration in bkc_dlist module*/
typedef struct bkc_dlist_node_tag_t bkc_dlist_node_t;

/*dlist node*/
struct bkc_dlist_node_tag_t {
    /*point to the user node*/
    void *unode_p;
    /*point to the parent dlist */
    bkc_dlist_t *parent_dlist_p;
    struct bkc_dlist_node_tag_t *prev_p;
    struct bkc_dlist_node_tag_t *next_p;
};

/*dlist*/
struct bkc_dlist_tag_t {
    /*total item number */
    unsigned int tinum;
    /*convenient for operation. in fact, sentinel_p->next_p
      will be the head_p*/
    bkc_dlist_node_t *sentinel_p;
    bkc_dlist_node_t *tail_p;
    /*the node last be focused or be operated on*/
    bkc_dlist_node_t *iterator_p;
};

/*mode of inserting a dnode to another dnode */
typedef enum bkc_dlist_dninsert_e {
    BKC_DLIST_DNINS_EMPTY,
    BKC_DLIST_DNINS_PREV,
    BKC_DLIST_DNINS_NEXT
} bkc_dlist_dninsert_m;

/*********************variables******************************************/

/*********************prototype of local functions***********************/
static int bkc_dlist_add_node(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static int bkc_dlist_remove_node(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static int bkc_dlist_find_node(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static int bkc_dlist_traverse_node(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static int bkc_dlist_get_dlsize(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static int bkc_dlist_prt_info(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static int bkc_dlist_get_nodenum(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static int bkc_dlist_clearmb(bkc_dlist_t *dlist_p);

static int bkc_dlist_initmb(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static int bkc_dlist_uninitmb(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static bkc_dlist_node_t *bkc_dlist_newdn(void);

static int bkc_dlist_rlsdn(bkc_dlist_node_t *dnode_p,
        bkc_dlist_dsymode_m dsymode,
        bkc_dlist_unode_destroy_func_t unode_destroy, void *param_p);

static bkc_dlist_t * bkc_dlist_newdl(void);

static int bkc_dlist_definitmb(bkc_dlist_t *dlist_p);

static int bkc_dlist_rlsdl(bkc_dlist_t *dlist_p);

static int bkc_dlist_insdntodn(bkc_dlist_node_t *beacon_dnode_p,
        bkc_dlist_node_t *insert_dnode_p, bkc_dlist_dninsert_m insmode);

static int bkc_dlist_insdntodl(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t *beacon_dnode_p,
        bkc_dlist_node_t *insert_dnode_p, bkc_dlist_dninsert_m insmode);

static int bkc_dlist_fdnbptr(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t **rdnode_pp, void *unode_p , int isckitr);

static int bkc_dlist_insdnrtun(bkc_dlist_t *dlist_p,
        void *beacon_unode_p,  bkc_dlist_node_t *insert_dnode_p,
        bkc_dlist_dninsert_m insmode);

static int bkc_dlist_adntodl(bkc_dlist_t *dlist_p, void *beacon_unode_p,
        bkc_dlist_node_t *add_dnode_p, bkc_dlist_opcode_m opcode);

static int bkc_dlist_deldnfmpn(bkc_dlist_node_t *dnode_p,
        bkc_dlist_rmvmode_m rmvmode,
        bkc_dlist_unode_destroy_func_t unode_destroy, void *param_p);

static int bkc_dlist_fdnbcmp(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t **fdnode_pp,
        bkc_dlist_node_t *sdnode_p, bkc_dlist_node_t *ednode_p,
        bkc_dlist_unode_isitem_cmp_func_t unode_isitem_cmp,
        void *clue_p);

static int bkc_dlist_isdnmatched(bkc_dlist_node_t *dnode_p,
        bkc_dlist_unode_isitem_cmp_func_t unode_isitem_cmp,
        void *clue_p, int *ismatched_p);

static int bkc_dlist_achritr(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t *nitr_p);

static int bkc_dlist_fdn(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t **fdnode_pp, void *unode_p, void *clue_p,
        bkc_dlist_unode_isitem_cmp_func_t unode_isitem_cmp,
        bkc_dlist_opcode_m opcode, unsigned int pos);

static int bkc_dlist_fdnpn(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t **fdnode_pp, void *unode_p,
        bkc_dlist_opcode_m opcode);

static int bkc_dlist_fdnnode(bkc_dlist_t *dlist_p, bkc_dlist_node_t **fdnode_pp,
        unsigned int pos);

static int bkc_dlist_fdnnspc(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t **fdnode_pp, void *unode_p, void *clue_p,
        bkc_dlist_unode_isitem_cmp_func_t unode_isitem_cmp);

static int bkc_dlist_remove_snode(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static int bkc_dlist_remove_nnode(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static int bkc_dlist_remove_htnode(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

static int bkc_dlist_remove_allnodes(bkc_dlist_t *dlist_p,
        bkc_dlist_operator_t *op_p);

/*********************implementation of open and local functions*********/
/*************************************************************************
 *                  bkc_dlist_create
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
bkc_dlist_t *bkc_dlist_create(bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);
    bkc_dlist_t *dlist_p = NULL;

    /*try and emit abnormites*/
    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(op_p != NULL
            && op_p->opcode == BKC_DLIST_OPCODE_CREATE_LIST,
            1, BKC_DLIST_RERR);

        dlist_p = bkc_dlist_newdl();
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL, 1, BKC_DLIST_RERR);

        BKC_DLIST_SPF_CKRET = bkc_dlist_initmb(dlist_p, op_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            2, BKC_DLIST_RERR);
    }

    /*catch abnormities*/
    BKC_DLIST_SPF_CATCH {
        BKC_DLIST_SPF_CASE (2) {
            BKC_DLIST_SPF_CKRET = bkc_dlist_rlsdl(dlist_p);
            BKC_DLIST_SPF_CKSRET(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
                BKC_DLIST_RERR);
            dlist_p = NULL;
            BKC_DLIST_SPF_BREAK;
        }
        BKC_DLIST_SPF_CASE (1) {
            /*unsucessfull return*/
            dlist_p = NULL;
            BKC_DLIST_SPF_BREAK;
        }
        BKC_DLIST_SPF_DEFAULT {
            /*successfull return*/
            BKC_DLIST_SPF_BREAK;
        }
    }

    /*check the running status and return value*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    BKC_DLIST_SPF_CKPRET(dlist_p != NULL);
    return dlist_p;
}

/*************************************************************************
 *                  bkc_dlist_clearmb
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist which need to set default value of its members
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     set the default value of each member in dlist_p
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_clearmb(bkc_dlist_t *dlist_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);
    /*try and emit abnormites*/
    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL, 1, BKC_DLIST_RERR);
        dlist_p->tinum = 0;
        dlist_p->sentinel_p = NULL;
        dlist_p->tail_p = NULL;
        dlist_p->iterator_p = NULL;
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_newdl
 *************************************************************************
 * parameters: N/A
 * return value:
 *     pointer to bkc_dlist_t.
 *     Not NULL: points to the sucessfully created dlist.
 *     NULL: abnormal return
 * description:
 *     new a dlist
 *****************************  Notes  *****************************
 ***********************************************************************/
bkc_dlist_t * bkc_dlist_newdl(void)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);
    bkc_dlist_t *dlist_p = NULL;

    /*try and emit abnormites*/
    BKC_DLIST_SPF_TRY {
        dlist_p = BKC_DLIST_SPF_MALLOC(sizeof(bkc_dlist_t));
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL, 1, BKC_DLIST_RERR);

        /*init the dlist member defautly*/
        BKC_DLIST_SPF_CKRET = bkc_dlist_clearmb(dlist_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
            BKC_DLIST_RSUC, 2, BKC_DLIST_RERR);

        dlist_p->sentinel_p = bkc_dlist_newdn();
        BKC_DLIST_SPF_CKTHROW(dlist_p->sentinel_p != NULL,
            2, BKC_DLIST_RERR);

        BKC_DLIST_SPF_CKRET = bkc_dlist_definitmb(dlist_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
            BKC_DLIST_RSUC, 3, BKC_DLIST_RERR);
    }

    BKC_DLIST_SPF_CATCH {
        BKC_DLIST_SPF_CASE (3) {
            BKC_DLIST_SPF_CKRET = bkc_dlist_rlsdn(
                dlist_p->sentinel_p, BKC_DLIST_DM_NOT_DELUSERDATA, NULL, NULL);
            BKC_DLIST_SPF_CKSRET(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
                BKC_DLIST_RERR);
            BKC_DLIST_SPF_SFREE(dlist_p);
            dlist_p = NULL;
            BKC_DLIST_SPF_BREAK;
        }
        BKC_DLIST_SPF_CASE (2) {
            BKC_DLIST_SPF_SFREE(dlist_p);
            BKC_DLIST_SPF_BREAK;
        }
        BKC_DLIST_SPF_CASE (1) {
            BKC_DLIST_SPF_BREAK;
        }
        BKC_DLIST_SPF_DEFAULT {
            BKC_DLIST_SPF_BREAK;
        }
    }

    BKC_DLIST_SPF_CKPRET(dlist_p != NULL);
    return dlist_p;
}

/*************************************************************************
 *                  bkc_dlist_definitmb
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist which need to be released
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     default init the members in the dlist_p
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_dlist_definitmb(bkc_dlist_t *dlist_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);
    /*try and emit abnormites*/
    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL &&
            dlist_p->sentinel_p != NULL, 1, BKC_DLIST_RERR);
        dlist_p->sentinel_p->parent_dlist_p = dlist_p;
        dlist_p->tail_p = dlist_p->sentinel_p;
        dlist_p->iterator_p = dlist_p->sentinel_p;

        /*clear the members*/
        dlist_p->tinum = 0;
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_rlsdl
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist which need to be released
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     release a dlist
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_rlsdl(bkc_dlist_t *dlist_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    /*try and emit abnormites*/
    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL, 1, BKC_DLIST_RERR);
        /*release sentinel_p*/
        BKC_DLIST_SPF_SFREE(dlist_p->sentinel_p);
        /*clear other members*/
        BKC_DLIST_SPF_CKRET = bkc_dlist_clearmb(dlist_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            1, BKC_DLIST_RERR);
        /*release the list*/
        BKC_DLIST_SPF_SFREE(dlist_p);
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_uninitmb
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist which need to release all its nodes
 *     op_p: points to the operator that contains the uninit-info
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     uninit the dlist_p accord to op_p
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_uninitmb(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);
    /*try and emit abnormites*/
    BKC_DLIST_SPF_TRY {
        unsigned int cnt = 0;
        bkc_dlist_node_t *dnode_p = NULL;

        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && op_p != NULL, 1,
            BKC_DLIST_RERR);

        /*uninit nodes*/
        for (cnt = 0, dnode_p = dlist_p->sentinel_p->next_p;
            cnt < dlist_p->tinum && dnode_p != NULL;
            cnt++, dnode_p = dlist_p->sentinel_p->next_p) {

            dlist_p->sentinel_p->next_p = dnode_p->next_p;

            /*release the user data if necessary*/
            BKC_DLIST_SPF_FNRET = bkc_dlist_rlsdn(dnode_p,
                op_p->dsymode, op_p->ophdlerset.unode_destroy,
                op_p->clue_p);
            BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_FNRET ==
                BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
        }
        BKC_DLIST_SPF_RLYTHROW();
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_definit_op
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_dlist_definit_op(bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);
    /*try and emit abnormites */
    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(op_p != NULL, 1, BKC_DLIST_RERR);
        op_p->opcode = BKC_DLIST_OPCODE_EMPTY;
        op_p->tunode_p = NULL;
        op_p->runode_p = NULL;
        op_p->bunode_p = NULL;
        op_p->clue_p = NULL;
        op_p->dlsize = 0;
        op_p->ophdlerset.unode_destroy = NULL;
        op_p->ophdlerset.unode_isitem_cmp = NULL;
        op_p->ophdlerset.unode_get_nsize = NULL;
        op_p->ophdlerset.unode_traverse = NULL;
        op_p->ophdlerset.unode_prt = NULL;
        op_p->dsymode = BKC_DLIST_DM_EMPTY;
        op_p->rmvmode = BKC_DLIST_RM_EMPTY;
        op_p->pos = 0;
        op_p->nodenum = 0;
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_destroy
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_dlist_destroy(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    /*try and emit abnormites */
    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(op_p != NULL
            && op_p->opcode == BKC_DLIST_OPCODE_DESTROY_LIST,
            1, BKC_DLIST_RERR);

        /*uninit the dlist*/
        BKC_DLIST_SPF_CKRET = bkc_dlist_uninitmb(dlist_p, op_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKRET = bkc_dlist_rlsdl(dlist_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            1, BKC_DLIST_RERR);
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_add_node
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist in which the designated dnode will be added
 *     op_p: points to the operator that contains the add-info
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     add a user node contained in the op_p to the dlist
 *     bkc_dlist_operate can be referred for more info
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_add_node(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);
    bkc_dlist_node_t *ndnode_p = NULL;

    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && op_p != NULL,
            1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKTHROW(op_p->tunode_p != NULL,
            1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKTHROW(
            op_p->opcode == BKC_DLIST_OPCODE_ADD_HEAD_NODE
            || op_p->opcode == BKC_DLIST_OPCODE_ADD_TAIL_NODE
            || op_p->opcode == BKC_DLIST_OPCODE_ADD_PREV_NODE
            || op_p->opcode == BKC_DLIST_OPCODE_ADD_NEXT_NODE,
            1, BKC_DLIST_RERR);

        /*get and init a new dnode*/
        ndnode_p = bkc_dlist_newdn();
        BKC_DLIST_SPF_CKTHROW(ndnode_p != NULL, 1, BKC_DLIST_RERR);
        ndnode_p->parent_dlist_p = dlist_p;
        ndnode_p->unode_p = op_p->tunode_p;

        BKC_DLIST_SPF_CKRET = bkc_dlist_adntodl(dlist_p, op_p->bunode_p,
            ndnode_p, op_p->opcode);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
            BKC_DLIST_RSUC, 2, BKC_DLIST_RERR);
    }

    BKC_DLIST_SPF_CATCH {
        BKC_DLIST_SPF_CASE (2) {
            BKC_DLIST_SPF_SFREE(ndnode_p);
            BKC_DLIST_SPF_BREAK;
        }
        BKC_DLIST_SPF_CASE (1) {
            BKC_DLIST_SPF_BREAK;
        }
        BKC_DLIST_SPF_DEFAULT {
            BKC_DLIST_SPF_BREAK;
        }
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_adntodl
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist in which the designated dnode will be added
 *     beacon_unode_p: points to the beacon user node
 *     add_dnode_p: the user node which need to be added
 *     opcode: add mode
 *         BKC_DLIST_OPCODE_ADD_HEAD_NODE:
 *             add to the head of dlist
 *         BKC_DLIST_OPCODE_ADD_TAIL_NODE:
 *             add to the tail of dlist
 *         BKC_DLIST_OPCODE_ADD_PREV_NODE:
 *             add as the prev_p of the beacon_unode_p
 *         BKC_DLIST_OPCODE_ADD_NEXT_NODE:
 *             add as the next_p of the beacon_unode_p
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     add add_dnode_p to the dlist
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_adntodl(bkc_dlist_t *dlist_p, void *beacon_unode_p,
        bkc_dlist_node_t *add_dnode_p, bkc_dlist_opcode_m opcode)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL , 1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKTHROW(add_dnode_p != NULL, 2, BKC_DLIST_RERR);

        switch (opcode) {
            case BKC_DLIST_OPCODE_ADD_TAIL_NODE:
                BKC_DLIST_SPF_CKRET = bkc_dlist_insdntodl(dlist_p,
                    dlist_p->tail_p, add_dnode_p, BKC_DLIST_DNINS_NEXT);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_ADD_HEAD_NODE:
                BKC_DLIST_SPF_CKRET = bkc_dlist_insdntodl(dlist_p,
                    dlist_p->sentinel_p, add_dnode_p, BKC_DLIST_DNINS_NEXT);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_ADD_PREV_NODE:
                BKC_DLIST_SPF_CKTHROW(beacon_unode_p != NULL,
                    2, BKC_DLIST_RERR);
                BKC_DLIST_SPF_CKRET = bkc_dlist_insdnrtun(dlist_p,
                    beacon_unode_p, add_dnode_p, BKC_DLIST_DNINS_PREV);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_ADD_NEXT_NODE:
                BKC_DLIST_SPF_CKTHROW(beacon_unode_p != NULL,
                    2, BKC_DLIST_RERR);
                BKC_DLIST_SPF_CKRET = bkc_dlist_insdnrtun(dlist_p,
                    beacon_unode_p, add_dnode_p, BKC_DLIST_DNINS_NEXT);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            default:
                BKC_DLIST_SPF_ERR("wrong opcode,please have a check\n");
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_FABN,
                    1, BKC_DLIST_RERR);
                break;
        }
        BKC_DLIST_SPF_RLYTHROW();
    }

    BKC_DLIST_SPF_CATCH {
        BKC_DLIST_SPF_CASE (2) {
            BKC_DLIST_SPF_ERR("add failed, parameter is null, "
                "please have a check\n");
            BKC_DLIST_SPF_BREAK;
        }
        BKC_DLIST_SPF_CASE (1) {
            BKC_DLIST_SPF_BREAK;
        }
        BKC_DLIST_SPF_DEFAULT {
            BKC_DLIST_SPF_BREAK;
        }
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_newdn
 *************************************************************************
 * parameters: N/A
 * return value:
 *     pointer to bkc_dlist_node_t.
 *     Not NULL: points to the sucessfully created bkc_dlist_node_t.
 *     NULL: abnormal return
 * description:
 *     new a bkc_dlist_node_t
 *****************************  Notes  *****************************
 ***********************************************************************/
bkc_dlist_node_t *bkc_dlist_newdn(void)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    bkc_dlist_node_t *ndnode_p = NULL;/*new node*/
    /*try and emit abnormites */
    BKC_DLIST_SPF_TRY {
        ndnode_p = BKC_DLIST_SPF_MALLOC(sizeof(bkc_dlist_node_t));
        BKC_DLIST_SPF_CKTHROW(ndnode_p != NULL, 1, BKC_DLIST_RERR);

        ndnode_p->next_p = NULL;
        ndnode_p->prev_p = NULL;
        ndnode_p->parent_dlist_p = NULL;
        ndnode_p->unode_p = NULL;
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(ndnode_p != NULL);
    return ndnode_p;
}

/*************************************************************************
 *                  bkc_dlist_rlsdn
 *************************************************************************
 * parameters:
 *     dnode_p: points to the dnode which need to be released
 *     dsymode:
 *         BKC_DLIST_DM_DELUSERDATA:
 *             destroy the user node data by unode_destroy
 *         BKC_DLIST_DM_NOT_DELUSERDATA
 *             not destroy the user node data
 *     unode_destroy:
 *         destroy handler
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     release the dnode_p
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_rlsdn(bkc_dlist_node_t *dnode_p,
        bkc_dlist_dsymode_m dsymode,
        bkc_dlist_unode_destroy_func_t unode_destroy, void *param_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    /*try and emit abnormites */
    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dnode_p != NULL
            && dsymode != BKC_DLIST_DM_EMPTY, 1, BKC_DLIST_RERR);

        if (dsymode == BKC_DLIST_DM_DELUSERDATA) {
            BKC_DLIST_SPF_CKTHROW(unode_destroy != NULL,
                1, BKC_DLIST_RERR);
            BKC_DLIST_SPF_CKRET = unode_destroy(dnode_p->unode_p, param_p);
            BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == 0,
                1, BKC_DLIST_RERR);
        }

        BKC_DLIST_SPF_SFREE(dnode_p);
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_initmb
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist of which the members need to be inited
 *     op_p: points to the operator that contains the init-info
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     init dlist_p member according to op_p
 *     there is no more requirement in op_p for creating currently,
 *     so need not do anything. this is an counterpart to
 *     bkc_dlist_uninitmb() which is inuse
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_initmb(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);
    /*try and emit abnormites*/
    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && op_p != NULL,
            1, BKC_DLIST_RERR);
        /*create requirements can be realized here, just a reservations
          for extensible requirements*/
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_insdntodn
 *************************************************************************
 * parameters:
 *     beacon_dnode_p: points to the beacon dnode
 *     insert_dnode_p: points to the dnode which need to be inserted
 *     insmode:
 *         BKC_DLIST_DNINS_PREV:
 *             insert the insert_dnode_p as prev_p of beacon_dnode_p
 *         BKC_DLIST_DNINS_NEXT:
 *             insert the insert_dnode_p as next_p of beacon_dnode_p
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     insert insert_dnode_p to beacon_dnode_p
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_insdntodn(bkc_dlist_node_t *beacon_dnode_p,
        bkc_dlist_node_t *insert_dnode_p, bkc_dlist_dninsert_m insmode)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    /*try and emit abnormites */
    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(beacon_dnode_p != NULL
            && insert_dnode_p != NULL, 1, BKC_DLIST_RERR);

        if (insmode == BKC_DLIST_DNINS_PREV) {
            if (beacon_dnode_p->prev_p != NULL) {
                beacon_dnode_p->prev_p->next_p = insert_dnode_p;
                insert_dnode_p->prev_p = beacon_dnode_p->prev_p;
            }
            insert_dnode_p->next_p = beacon_dnode_p;
            beacon_dnode_p->prev_p = insert_dnode_p;
        } else if (insmode == BKC_DLIST_DNINS_NEXT) {
            if (beacon_dnode_p->next_p != NULL) {
                beacon_dnode_p->next_p->prev_p = insert_dnode_p;
                insert_dnode_p->next_p = beacon_dnode_p->next_p;
            }
            insert_dnode_p->prev_p = beacon_dnode_p;
            beacon_dnode_p->next_p = insert_dnode_p;
        } else {
            BKC_DLIST_SPF_ERR("not supported insmode,"
                "please have a check\n");
            BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_FABN,
                1, BKC_DLIST_RERR);
        }
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_insdntodl
 *************************************************************************
 * parameters:
 *     dlist_p: points to the dlist
 *     beacon_dnode_p: points to the beacon dnode
 *     insert_dnode_p: points to the dnode which need to be inserted
 *     insmode:
 *         BKC_DLIST_DNINS_PREV:
 *             insert the insert_dnode_p as prev_p of beacon_dnode_p
 *         BKC_DLIST_DNINS_NEXT:
 *             insert the insert_dnode_p as next_p of beacon_dnode_p
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     insert insert_dnode_p to dlist referring to beacon_dnode_p
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_insdntodl(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t *beacon_dnode_p, bkc_dlist_node_t *insert_dnode_p,
        bkc_dlist_dninsert_m insmode)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    /*try and emit abnormites */
    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && beacon_dnode_p != NULL
            && insert_dnode_p != NULL, 1, BKC_DLIST_RERR);

        BKC_DLIST_SPF_CKRET = bkc_dlist_insdntodn(beacon_dnode_p,
            insert_dnode_p, insmode);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            1, BKC_DLIST_RERR);

        BKC_DLIST_SPF_CKTHROW(dlist_p->tinum >= 0,
            1, BKC_DLIST_RERR);
        dlist_p->tinum++;

        /*add to tail, and need to update the tail info*/
        if (beacon_dnode_p == dlist_p->tail_p &&
            insmode == BKC_DLIST_DNINS_NEXT) {
            dlist_p->tail_p = insert_dnode_p;
        }
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_fdnbptr
 *************************************************************************
 * parameters:
 *     dlist_p: points to the dlist
 *     rdnode_pp: the returned dnode
 *     unode_p: points to the user node which need to be found
 *     isckitr:
 *       0: will not check the iterator in dlist
 *       1: will check the iterator firstly when searching in dlist
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     find a dnode by the pointer of a user node.
 *     the found dnode will be returned through retdnode_pp
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_fdnbptr(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t **rdnode_pp, void *unode_p , int isckitr)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    /*try and emit abnormites*/
    BKC_DLIST_SPF_TRY {
        /*iterant dnode*/
        bkc_dlist_node_t *dnode_p = NULL;
        unsigned int cnt = 0;

        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && unode_p != NULL
            && rdnode_pp != NULL, 1, BKC_DLIST_RERR);

        if (isckitr != 0 && dlist_p->iterator_p != NULL) {
            dnode_p = (dlist_p->iterator_p->unode_p == unode_p
                && dlist_p->iterator_p != dlist_p->sentinel_p ?
                dlist_p->iterator_p : NULL);
        }

        if (dnode_p == NULL) {
            for (cnt = 0, dnode_p = dlist_p->sentinel_p->next_p;
                cnt < dlist_p->tinum && dnode_p != NULL;
                cnt++, dnode_p = dnode_p->next_p) {

                if (dnode_p->unode_p == unode_p) {
                    /*found the unode_p*/
                    break;
                }
            }
            BKC_DLIST_SPF_RLYTHROW();
            BKC_DLIST_SPF_CKTHROW(cnt <= dlist_p->tinum,
                1, BKC_DLIST_RERR);
        }
        /*return the result dnode*/
        *rdnode_pp = dnode_p;
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_insdnrtun
 *************************************************************************
 * parameters:
 *     dlist_p: points to the dlist
 *     beacon_unode_p: points to the beacon user node
 *     insert_dnode_p: points to the dnode which need to be inserted
 *     insmode:
 *         BKC_DLIST_DNINS_PREV:
 *             insert the insert_dnode_p as prev_p of beacon_dnode_p
 *         BKC_DLIST_DNINS_NEXT:
 *             insert the insert_dnode_p as next_p of beacon_dnode_p
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     insert dnode to  dlist referring to the beacon_unode_p
 *     the beacon_unode_p points to a user node
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_insdnrtun(bkc_dlist_t *dlist_p, void *beacon_unode_p,
        bkc_dlist_node_t *insert_dnode_p, bkc_dlist_dninsert_m insmode)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        bkc_dlist_node_t *idnode_p = NULL;

        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && beacon_unode_p != NULL
            && insert_dnode_p != NULL, 1, BKC_DLIST_RERR);

        BKC_DLIST_SPF_CKRET = bkc_dlist_fdnbptr(dlist_p,
            &idnode_p, beacon_unode_p, 1);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
            BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);

        if (idnode_p != NULL) {
            BKC_DLIST_SPF_CKRET = bkc_dlist_insdntodl(dlist_p,
                idnode_p, insert_dnode_p, insmode);
            BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
        } else {
            BKC_DLIST_SPF_ERR("not find the beacon node,"
                "please have a check\n");
            BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_FABN,
                1, BKC_DLIST_RERR);
        }
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_remove_snode
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist in which the designated dnode will be removed
 *     op_p: points to the operator that contains the remove-info
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     remove the specific user node designated by op_p->tunode_p from the dlist
 *     op_p->rmvmode descripts how to remove this node
 *          BKC_DLIST_RM_DELUSERDATA:
 *              remove and destroy it's data by unode_destroy
 *          BKC_DLIST_RM_NOT_DELUSERDATA: just remove the user node
 *              without destroying user node data
 *     bkc_dlist_operate can be referred for more info
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_remove_snode(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        bkc_dlist_node_t *idnode_p = NULL;
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL
            && op_p->opcode == BKC_DLIST_OPCODE_REMOVE_NODE,
            1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKTHROW(op_p->tunode_p != NULL, 1, BKC_DLIST_RERR);

        BKC_DLIST_SPF_CKRET = bkc_dlist_fdnbptr(dlist_p, &idnode_p,
            op_p->tunode_p, 1);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKTHROW(idnode_p != NULL, 1, BKC_DLIST_RERR);

        dlist_p->tinum--;
        BKC_DLIST_SPF_CKTHROW(dlist_p->tinum >= 0, 1, BKC_DLIST_RERR);

        if (idnode_p == dlist_p->tail_p)
            dlist_p->tail_p = dlist_p->tail_p->prev_p;

        BKC_DLIST_SPF_CKRET = bkc_dlist_deldnfmpn(idnode_p,
            op_p->rmvmode, op_p->ophdlerset.unode_destroy, op_p->clue_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            1, BKC_DLIST_RERR);
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_remove_nnode
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist in which the designated dnode will be removed
 *     op_p: points to the operator that contains the remove-info
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     remove the num-th user node designated by op_p->pos from the dlist
 *     op_p->rmvmode descripts how to remove this node
 *         BKC_DLIST_RM_DELUSERDATA:
 *             remove and destroy it's data by unode_destroy
 *         BKC_DLIST_RM_NOT_DELUSERDATA: just remove the user node
 *             without destroying user node data
 *     op_p->runode_p will return the designated node which need to
 *         be removed if op_p->rmvmode == BKC_DLIST_RM_NOT_DELUSERDATA.
 *         if op_p->rmvmode == BKC_DLIST_RM_DELUSERDATA,
 *         op_p->runode_p will be NULL after removing
 *     bkc_dlist_operate can be referred for more info
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_remove_nnode(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        bkc_dlist_node_t *idnode_p = NULL;
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL
            && op_p->opcode == BKC_DLIST_OPCODE_REMOVE_N_NODE,
            1, BKC_DLIST_RERR);

        /*prepare to return the user node*/
        op_p->runode_p = NULL;

        BKC_DLIST_SPF_CKRET = bkc_dlist_fdnnode(dlist_p, &idnode_p, op_p->pos);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKTHROW(idnode_p != NULL, 1, BKC_DLIST_RERR);

        dlist_p->tinum--;
        BKC_DLIST_SPF_CKTHROW(dlist_p->tinum >= 0, 1, BKC_DLIST_RERR);

        if (idnode_p == dlist_p->tail_p)
            dlist_p->tail_p = dlist_p->tail_p->prev_p;

        /*prepare to return the user node*/
        if (op_p->rmvmode == BKC_DLIST_RM_NOT_DELUSERDATA)
            op_p->runode_p = idnode_p->unode_p;

        BKC_DLIST_SPF_CKRET = bkc_dlist_deldnfmpn(idnode_p,
            op_p->rmvmode, op_p->ophdlerset.unode_destroy, op_p->clue_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            1, BKC_DLIST_RERR);
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_remove_htnode
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist in which the designated dnode will be removed
 *     op_p: points to the operator that contains the remove-info
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     remove the head or tail user node from the dlist
 *     op_p->rmvmode descripts how to remove this node
 *         BKC_DLIST_RM_DELUSERDATA:
 *             remove and destroy it's data by unode_destroy
 *         BKC_DLIST_RM_NOT_DELUSERDATA: just remove the user node
 *             without destroying user node data
 *     op_p->runode_p will return the designated node which need to
 *         be removed if op_p->rmvmode == BKC_DLIST_RM_NOT_DELUSERDATA.
 *         if op_p->rmvmode == BKC_DLIST_RM_DELUSERDATA,
 *         op_p->runode_p will be NULL after removing
 *     bkc_dlist_operate can be referred for more info
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_remove_htnode(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        bkc_dlist_node_t *idnode_p = NULL;
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL
            && (op_p->opcode == BKC_DLIST_OPCODE_REMOVE_HEAD
                || op_p->opcode == BKC_DLIST_OPCODE_REMOVE_TAIL),
            1, BKC_DLIST_RERR);

        /*prepare to return the user node*/
        op_p->runode_p = NULL;

        if (op_p->opcode == BKC_DLIST_OPCODE_REMOVE_HEAD)
            idnode_p = dlist_p->sentinel_p->next_p;
        else if (op_p->opcode == BKC_DLIST_OPCODE_REMOVE_TAIL)
            idnode_p = dlist_p->tail_p;
        else
            idnode_p = NULL;

        BKC_DLIST_SPF_CKTHROW(idnode_p != NULL, 1, BKC_DLIST_RERR);

        dlist_p->tinum--;
        BKC_DLIST_SPF_CKTHROW(dlist_p->tinum >= 0, 1, BKC_DLIST_RERR);

        if (idnode_p == dlist_p->tail_p)
            dlist_p->tail_p = dlist_p->tail_p->prev_p;

        if (op_p->rmvmode == BKC_DLIST_RM_NOT_DELUSERDATA)
            op_p->runode_p = idnode_p->unode_p;

        BKC_DLIST_SPF_CKRET = bkc_dlist_deldnfmpn(idnode_p,
            op_p->rmvmode, op_p->ophdlerset.unode_destroy, op_p->clue_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            1, BKC_DLIST_RERR);
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_remove_allnodes
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist in which the all dnodes will be removed
 *     op_p: points to the operator that contains the remove-info
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     remove all the user nodes from the dlist
 *     op_p->rmvmode descripts how to remove this node
 *         BKC_DLIST_RM_DELUSERDATA:
 *             remove and destroy it's data by unode_destroy
 *         BKC_DLIST_RM_NOT_DELUSERDATA: just remove the user node
 *             without destroying user node data
 *
 *     bkc_dlist_operate can be referred for more info
 *     if the dlist_p have no nodes at all, no nodes will be removed,
 *     and this func will return BKC_DLIST_RSUC
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_remove_allnodes(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    /*try and emit abnormites */
    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(op_p != NULL
            && op_p->opcode == BKC_DLIST_OPCODE_REMOVE_ALL,
            1, BKC_DLIST_RERR);

        BKC_DLIST_SPF_CKTHROW(op_p->rmvmode == BKC_DLIST_RM_DELUSERDATA
            || op_p->rmvmode == BKC_DLIST_RM_NOT_DELUSERDATA,
            1, BKC_DLIST_RERR);
        if (op_p->rmvmode == BKC_DLIST_RM_DELUSERDATA) {
            op_p->dsymode = BKC_DLIST_DM_DELUSERDATA;
        } else {
            op_p->dsymode = BKC_DLIST_DM_NOT_DELUSERDATA;
        }

        /*clear the dlist*/
        BKC_DLIST_SPF_CKRET = bkc_dlist_uninitmb(dlist_p, op_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKRET = bkc_dlist_definitmb(dlist_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
            BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
    }

    /*neednt to catch*/
    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_remove_node
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist in which the all dnodes will be removed
 *     op_p: points to the operator that contains the remove-info
 *     op_p->opcode:
 *         BKC_DLIST_OPCODE_REMOVE_NODE:
 *             remove the specific user node designated by
 *             op_p->tunode_p from the dlist
 *         BKC_DLIST_OPCODE_REMOVE_N_NODE:
 *             remove the num-th user node designated by
 *             op_p->pos from the dlist
 *         BKC_DLIST_OPCODE_REMOVE_HEAD:
 *             remove the head user node from the dlist
 *         BKC_DLIST_OPCODE_REMOVE_TAIL
 *             remove the tail user node from the dlist
 *         BKC_DLIST_OPCODE_REMOVE_ALL
 *             remove all the user nodes from the dlist
 * return value:
 *     BKC_DLIST_RSUC: successfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     the designated dnode/dnodes will be removed from dlist_p
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_remove_node(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && op_p != NULL,
             1, BKC_DLIST_RERR);

        switch (op_p->opcode) {
            case BKC_DLIST_OPCODE_REMOVE_NODE:
                BKC_DLIST_SPF_CKRET = bkc_dlist_remove_snode(dlist_p, op_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_REMOVE_N_NODE:
                BKC_DLIST_SPF_CKRET = bkc_dlist_remove_nnode(dlist_p, op_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_REMOVE_HEAD:
            case BKC_DLIST_OPCODE_REMOVE_TAIL:
                BKC_DLIST_SPF_CKRET = bkc_dlist_remove_htnode(dlist_p, op_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_REMOVE_ALL:
                BKC_DLIST_SPF_CKRET = bkc_dlist_remove_allnodes(dlist_p, op_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
             default:
                BKC_DLIST_SPF_ERR("wrong remove-opcode,please have a check\n");
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_FABN,
                    1, BKC_DLIST_RERR);
                break;
        }
        BKC_DLIST_SPF_RLYTHROW();
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_deldnfmpn
 *************************************************************************
 * parameters:
 *     dnode_p: points to the bkc_dlist_node_t which needed to be deleted
 *     rmvmode:
 *         BKC_DLIST_RM_DELUSERDATA:
 *              remove and destroy it's data by unode_destroy
 *         BKC_DLIST_RM_NOT_DELUSERDATA: just remove the user node
 *              without destroying user node data
 *     unode_destroy: destroy handler
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     delete a dnode from its prev and next neighbours
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_deldnfmpn(bkc_dlist_node_t *dnode_p,
        bkc_dlist_rmvmode_m rmvmode,
        bkc_dlist_unode_destroy_func_t unode_destroy, void *param_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        bkc_dlist_node_t *idnode_p = NULL;
        bkc_dlist_dsymode_m dsymode = BKC_DLIST_DM_EMPTY;

        BKC_DLIST_SPF_CKTHROW(dnode_p != NULL, 1, BKC_DLIST_RERR);

        idnode_p = dnode_p;
        if (idnode_p->prev_p != NULL)
            idnode_p->prev_p->next_p = idnode_p->next_p;
        if (idnode_p->next_p != NULL)
            idnode_p->next_p->prev_p = idnode_p->prev_p;

        dsymode = (rmvmode == BKC_DLIST_RM_DELUSERDATA ?
            BKC_DLIST_DM_DELUSERDATA : BKC_DLIST_DM_NOT_DELUSERDATA);
        BKC_DLIST_SPF_CKRET = bkc_dlist_rlsdn(idnode_p, dsymode,
            unode_destroy, param_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
            1, BKC_DLIST_RERR);
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_find_node
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist in which the designated dnode will be found
 *     op_p: points to the operator that contains the find-info
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     find the user node in dlist designated by op_p.
 *     found user node will be return through op_p->runode_p
 *     bkc_dlist_operate can be referred for more info
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_find_node(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        bkc_dlist_node_t *fdnode_p = NULL;
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && op_p != NULL,
            1, BKC_DLIST_RERR);

        BKC_DLIST_SPF_CKTHROW(op_p->opcode == BKC_DLIST_OPCODE_FIND_SPEC_NODE
            || op_p->opcode == BKC_DLIST_OPCODE_FIND_NSPEC_NODE
            || op_p->opcode == BKC_DLIST_OPCODE_FIND_HEAD_NODE
            || op_p->opcode == BKC_DLIST_OPCODE_FIND_TAIL_NODE
            || op_p->opcode == BKC_DLIST_OPCODE_FIND_PREV_NODE
            || op_p->opcode == BKC_DLIST_OPCODE_FIND_NEXT_NODE
            || op_p->opcode == BKC_DLIST_OPCODE_FIND_N_NODE
            || op_p->opcode == BKC_DLIST_OPCODE_FIND_NODE,
            1, BKC_DLIST_RERR);

        BKC_DLIST_SPF_CKRET = bkc_dlist_fdn(dlist_p,
            &fdnode_p, op_p->tunode_p, op_p->clue_p,
            op_p->ophdlerset.unode_isitem_cmp, op_p->opcode, op_p->pos);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
            BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);

        /*get the found result*/
        if (fdnode_p != NULL)
           op_p->runode_p = fdnode_p->unode_p;
        else
           op_p->runode_p = NULL;

        if (op_p->opcode == BKC_DLIST_OPCODE_FIND_NSPEC_NODE)
           op_p->tunode_p = op_p->runode_p;
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_fdnbcmp
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist in which the designated dnode will be found
 *     sdnode_p: points to the start search dnode in the dlist
 *     ednode_p: points to the end search dnode in the dlist
 *     fdnode_pp: the found dnode
 *     unode_isitem_cmp: compare handler used in searching
 *     clue_p: compare handler parameter
 *     isckitr:
 *       0: will not check the iterator in dlist
 *       1: will check the iterator firstly when searching in dlist
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     find dnode by cmp handler
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_fdnbcmp(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t **fdnode_pp,
        bkc_dlist_node_t *sdnode_p, bkc_dlist_node_t *ednode_p,
        bkc_dlist_unode_isitem_cmp_func_t unode_isitem_cmp,
        void *clue_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        unsigned int cnt = 0;
        int ismatched = 0;
        bkc_dlist_node_t *fdnode_p = NULL;

        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && fdnode_pp != NULL
            && unode_isitem_cmp != NULL,
            1, BKC_DLIST_RERR);

        BKC_DLIST_SPF_PRT("sdnode_p->unode is %x,"
            "ednode_p->unode is %x\n",
            sdnode_p != NULL ? (int)sdnode_p->unode_p : 0,
            ednode_p != NULL ? (int)ednode_p->unode_p : 0);

        if (sdnode_p != NULL && ednode_p != NULL) {
            for (cnt = 0, fdnode_p = sdnode_p;
                cnt < dlist_p->tinum && fdnode_p != NULL
                && fdnode_p != ednode_p;
                cnt++, fdnode_p = fdnode_p->next_p) {

                ismatched = unode_isitem_cmp(fdnode_p->unode_p, clue_p);
                /*the item is matched*/
                if (ismatched == 0)
                    break;
            }
            BKC_DLIST_SPF_RLYTHROW();
            BKC_DLIST_SPF_CKTHROW(cnt <= dlist_p->tinum,
                1, BKC_DLIST_RERR);
        } else {
            fdnode_p = NULL;
        }

        /*get the result dnode*/
        *fdnode_pp = fdnode_p;
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_isdnmatched
 *************************************************************************
 * parameters:
 *     dnode_p: points to the dnode which need to be judged
 *     unode_isitem_cmp: compare handler used in searching
 *     clue_p: compare handler parameter
 *     ismatched_p:
 *       *ismatched_p == 0: matched
 *       other: not matched
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     judge if the dnode_p is matched
 *     [no called]
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_dlist_isdnmatched(bkc_dlist_node_t *dnode_p,
        bkc_dlist_unode_isitem_cmp_func_t unode_isitem_cmp,
        void *clue_p, int *ismatched_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        int ismatched = 0;
        BKC_DLIST_SPF_CKTHROW(dnode_p != NULL && unode_isitem_cmp != NULL
            && clue_p != NULL && ismatched_p != NULL, 1, BKC_DLIST_RERR);
        ismatched = unode_isitem_cmp(dnode_p->unode_p, clue_p);
        *ismatched_p = ismatched;
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_achritr
 *************************************************************************
 * parameters:
 *     dlist_p: points to the dlist
 *     nitr_p: new iterator position
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     anchor iterator in the dlist
 *****************************  Notes  *****************************
 ***********************************************************************/
int bkc_dlist_achritr(bkc_dlist_t *dlist_p, bkc_dlist_node_t *nitr_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL , 1, BKC_DLIST_RERR);
        if (nitr_p != NULL)
            dlist_p->iterator_p = nitr_p;
        else
            dlist_p->iterator_p = dlist_p->sentinel_p;
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_fdn
 *************************************************************************
 * parameters:
 *     dlist_p: points to the dlist
 *     fdnode_pp: the found dnode
 *     unode_p: points to the user node
 *     unode_isitem_cmp: compare handler used in searching
 *     clue_p: compare handler parameter
 *     pos: the position of the needed user node
 *     opcode:
 *         BKC_DLIST_OPCODE_FIND_SPEC_NODE:
 *             find the specific dnode by unode_p from the first
 *             nodes of the dlist
 *         BKC_DLIST_OPCODE_FIND_SPEC_NODE:
 *             find the specific dnode from last found node
 *         BKC_DLIST_OPCODE_FIND_HEAD_NODE:
 *             find the head dnode of he dlist
 *         BKC_DLIST_OPCODE_FIND_TAIL_NODE
 *             find the head dnode of he dlist
 *         BKC_DLIST_OPCODE_FIND_PREV_NODE
 *             find the prev dnode refering to unode_p
 *         BKC_DLIST_OPCODE_FIND_NEXT_NODE
 *             find the next dnode refering to unode_p
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     the wanted dnode will be returned through fdnode_pp
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_fdn(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t **fdnode_pp, void *unode_p, void *clue_p,
        bkc_dlist_unode_isitem_cmp_func_t unode_isitem_cmp,
        bkc_dlist_opcode_m opcode, unsigned int pos)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        bkc_dlist_node_t *fdnode_p = NULL;
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && fdnode_pp != NULL,
             1, BKC_DLIST_RERR);

        switch (opcode) {
            case BKC_DLIST_OPCODE_FIND_SPEC_NODE:
                BKC_DLIST_SPF_CKTHROW(unode_isitem_cmp != NULL,
                    2, BKC_DLIST_RERR);
                BKC_DLIST_SPF_CKRET = bkc_dlist_fdnbcmp(dlist_p,
                    &fdnode_p, dlist_p->sentinel_p->next_p,
                    dlist_p->tail_p, unode_isitem_cmp, clue_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_FIND_NSPEC_NODE:
                BKC_DLIST_SPF_CKTHROW(unode_isitem_cmp != NULL,
                    2, BKC_DLIST_RERR);
                BKC_DLIST_SPF_CKRET = bkc_dlist_fdnnspc(dlist_p, &fdnode_p,
                    unode_p, clue_p, unode_isitem_cmp);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_FIND_HEAD_NODE:
                fdnode_p = dlist_p->sentinel_p->next_p;
                break;
            case BKC_DLIST_OPCODE_FIND_TAIL_NODE:
                fdnode_p = dlist_p->tail_p;
                break;
            case BKC_DLIST_OPCODE_FIND_PREV_NODE:
            case BKC_DLIST_OPCODE_FIND_NEXT_NODE:
            case BKC_DLIST_OPCODE_FIND_NODE:
                BKC_DLIST_SPF_CKTHROW(unode_p != NULL, 2, BKC_DLIST_RERR);
                BKC_DLIST_SPF_CKRET = bkc_dlist_fdnpn(dlist_p, &fdnode_p,
                    unode_p, opcode);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
                    1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_FIND_N_NODE:
                BKC_DLIST_SPF_CKTHROW(dlist_p != NULL, 2, BKC_DLIST_RERR);
                BKC_DLIST_SPF_CKRET = bkc_dlist_fdnnode(dlist_p, &fdnode_p, pos);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
                    1, BKC_DLIST_RERR);
                break;
            default:
                BKC_DLIST_SPF_ERR("wrong find-opcode,please have a check\n");
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_FABN,
                    1, BKC_DLIST_RERR);
                break;
        }
        BKC_DLIST_SPF_RLYTHROW();

        *fdnode_pp = fdnode_p;
        BKC_DLIST_SPF_CKRET = bkc_dlist_achritr(dlist_p, fdnode_p);
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC, 1,
            BKC_DLIST_RERR);
    }

    BKC_DLIST_SPF_CATCH {
        BKC_DLIST_SPF_CASE (2) {
            *fdnode_pp = NULL;
            BKC_DLIST_SPF_BREAK;
        }
        BKC_DLIST_SPF_CASE (1) {
            BKC_DLIST_SPF_BREAK;
        }
        BKC_DLIST_SPF_DEFAULT {
            BKC_DLIST_SPF_BREAK;
        }
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_fdnpn
 *************************************************************************
 * parameters:
 *     dlist_p: points to the dlist
 *     opcode : the opration code
 *     fdnode_pp: will return the found dnode
 *     unode_p: points to the user node
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     get prev and next partner of the dnode which contained unode_p
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_fdnpn(bkc_dlist_t *dlist_p, bkc_dlist_node_t **fdnode_pp,
        void *unode_p, bkc_dlist_opcode_m opcode)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        bkc_dlist_node_t *sdnode_p = NULL;
        BKC_DLIST_SPF_CKTHROW(fdnode_pp != NULL && unode_p != NULL,
             1, BKC_DLIST_RERR);

        switch (opcode) {
            case BKC_DLIST_OPCODE_FIND_PREV_NODE:
                BKC_DLIST_SPF_CKRET = bkc_dlist_fdnbptr(dlist_p,
                    &sdnode_p, unode_p, 1);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                *fdnode_pp = (sdnode_p != NULL
                    && sdnode_p != dlist_p->sentinel_p->next_p ?
                    sdnode_p->prev_p : NULL);
                break;
             case BKC_DLIST_OPCODE_FIND_NODE:
                BKC_DLIST_SPF_CKRET = bkc_dlist_fdnbptr(dlist_p,
                    &sdnode_p, unode_p, 1);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                *fdnode_pp = sdnode_p;
                break;
            case BKC_DLIST_OPCODE_FIND_NEXT_NODE:
                BKC_DLIST_SPF_CKRET = bkc_dlist_fdnbptr(dlist_p,
                    &sdnode_p, unode_p, 1);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                *fdnode_pp = (sdnode_p != NULL ? sdnode_p->next_p : NULL);
                break;
            default:
                BKC_DLIST_SPF_ERR("wrong find-opcode,please have a check\n");
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_FABN,
                    1, BKC_DLIST_RERR);
                break;
        }
        BKC_DLIST_SPF_RLYTHROW();
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_fdnpn
 *************************************************************************
 * parameters:
 *     dlist_p: points to the dlist
 *     pos: the position of the needed dnode
 *     fdnode_pp: will return the found dnode
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     get dnode with the position of pos
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_fdnnode(bkc_dlist_t *dlist_p, bkc_dlist_node_t **fdnode_pp,
    unsigned int pos)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        unsigned int loop = 0;
        bkc_dlist_node_t *dnode_p = NULL;

        BKC_DLIST_SPF_CKTHROW(fdnode_pp != NULL && dlist_p != NULL,
             1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKTHROW((dlist_p->tinum > 0 && pos >= 0 && pos < dlist_p->tinum)
            || (dlist_p->tinum == 0 && pos == 0), 1, BKC_DLIST_RERR);

        dnode_p = dlist_p->sentinel_p->next_p;
        for (loop = 0; loop < pos && dnode_p != NULL; loop++) {
            dnode_p = dnode_p->next_p;
        }
        *fdnode_pp = dnode_p;
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_fdnnspc
 *************************************************************************
 * parameters:
 *     dlist_p: points to the dlist
 *     fdnode_pp: the found dnode
 *     unode_p: points to the user node
 *     unode_isitem_cmp: compare handler used in searching
 *     clue_p: compare handler parameter
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     get next specified dnode through clue_p and unode_isitem_cmp
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_fdnnspc(bkc_dlist_t *dlist_p,
        bkc_dlist_node_t **fdnode_pp, void *unode_p, void *clue_p,
        bkc_dlist_unode_isitem_cmp_func_t unode_isitem_cmp)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        bkc_dlist_node_t *sdnode_p = NULL;

        BKC_DLIST_SPF_CKTHROW(fdnode_pp != NULL
            && unode_isitem_cmp != NULL, 1, BKC_DLIST_RERR);

        if (unode_p == NULL) {
            BKC_DLIST_SPF_NP("sdnode_p->unode is %x,"
                "ednode_p->unode is %x\n",
                dlist_p->sentinel_p->next_p != NULL ?
                    (int)dlist_p->sentinel_p->next_p->unode_p : 0,
                dlist_p->tail_p != NULL ? (int)dlist_p->tail_p->unode_p : 0);

            BKC_DLIST_SPF_CKRET = bkc_dlist_fdnbcmp(dlist_p,
                fdnode_pp, dlist_p->sentinel_p->next_p,
                dlist_p->tail_p, unode_isitem_cmp, clue_p);
            BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
        } else {
            BKC_DLIST_SPF_CKRET = bkc_dlist_fdnpn(dlist_p, &sdnode_p,
                unode_p, BKC_DLIST_OPCODE_FIND_NEXT_NODE);
            BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == BKC_DLIST_RSUC,
                1, BKC_DLIST_RERR);
            BKC_DLIST_SPF_CKRET = bkc_dlist_fdnbcmp(dlist_p,
                fdnode_pp, sdnode_p, dlist_p->tail_p,
                unode_isitem_cmp, clue_p);
            BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
        }
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_traverse_node
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist which need to be traversed
 *     op_p: points to the operator that contains the
 *         traverse-info of the dlist
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     traverse each node in the dlist, and appied the manipulation
 *     designated by the unode_traverse handler in op_p to each
 *     user node contained in the dlist
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_traverse_node(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        unsigned int cnt = 0;
        bkc_dlist_node_t *ldnode_p = NULL;
        /*traverse ret*/
        int tvrret = 0;

        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && op_p != NULL,
            1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKTHROW(op_p->opcode ==
            BKC_DLIST_OPCODE_TRAVERSE_NODE, 1, BKC_DLIST_RERR);

        if (op_p->ophdlerset.unode_traverse != NULL) {
            for (cnt = 0, ldnode_p = dlist_p->sentinel_p->next_p;
                cnt < dlist_p->tinum && ldnode_p != NULL;
                cnt++, ldnode_p = ldnode_p->next_p) {
                tvrret = op_p->ophdlerset.unode_traverse(ldnode_p->unode_p,
                    op_p->clue_p);
                BKC_DLIST_SPF_CKTHROW(tvrret == 0, 1, BKC_DLIST_RERR);
            }
            BKC_DLIST_SPF_RLYTHROW();
        } else {
            BKC_DLIST_SPF_WARN("op_p->ophdlerset.unode_traverse is null"
                "please have a check\n");
        }
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_get_dlsize
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist which need to be get its size
 *     op_p: points to the operator that contains the
 *         getsize-info of the dlist
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     calculate the size of all the user nodes of the dlist
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_get_dlsize(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        unsigned int cnt = 0;
        bkc_dlist_node_t *ldnode_p = NULL;
        int dlsize = 0;

        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && op_p != NULL,
            1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKTHROW(op_p->opcode ==
            BKC_DLIST_OPCODE_GET_DLIST_SIZE, 1, BKC_DLIST_RERR);

        if (op_p->ophdlerset.unode_get_nsize != NULL) {
            for (cnt = 0, ldnode_p = dlist_p->sentinel_p->next_p;
                cnt < dlist_p->tinum && ldnode_p != NULL;
                cnt++, ldnode_p = ldnode_p->next_p) {
                dlsize += op_p->ophdlerset.unode_get_nsize(
                    ldnode_p->unode_p, op_p->clue_p);
            }
            BKC_DLIST_SPF_RLYTHROW();
            op_p->dlsize = dlsize;
        } else {
            op_p->dlsize = 0;
            BKC_DLIST_SPF_WARN("op_p->ophdlerset.unode_get_nsize is null"
                "please have a check\n");
        }

        BKC_DLIST_SPF_CKTHROW(op_p->dlsize >= 0, 1, BKC_DLIST_RERR);
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_prt_info
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist which need to be printed its
 *         detailed infomation
 *     op_p: points to the operator that contains the
 *         print-info of the dlist
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     print the detailed infomation of the dlist to the stand
 *     output file
 *     op_p->opcode must be BKC_DLIST_OPCODE_PRT_DLIST.
 *     op_p->ophdlerset.unode_prt can not be NULL.
 *     bkc_dlist_operate can be referred for more info
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_prt_info(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        unsigned int cnt = 0;
        bkc_dlist_node_t *ldnode_p = NULL;

        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && op_p != NULL,
            1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKTHROW(op_p->opcode ==
            BKC_DLIST_OPCODE_PRT_DLIST, 1, BKC_DLIST_RERR);

        BKC_DLIST_SPF_PRT("##################################\n");
        BKC_DLIST_SPF_PRT("dlist item is %d\n", dlist_p->tinum);

        BKC_DLIST_SPF_PRT("sdnode_p->unode is %x,"
            "ednode_p->unode is %x\n",
            dlist_p->sentinel_p->next_p != NULL ?
                (int)dlist_p->sentinel_p->next_p->unode_p : 0,
            dlist_p->tail_p != NULL ? (int)dlist_p->tail_p->unode_p : 0);

        for (cnt = 0, ldnode_p = dlist_p->sentinel_p->next_p;
            cnt < dlist_p->tinum && ldnode_p != NULL;
            cnt++, ldnode_p = ldnode_p->next_p) {

            BKC_DLIST_SPF_CKTHROW(ldnode_p->unode_p != NULL, 1,
                BKC_DLIST_RERR);

            BKC_DLIST_SPF_PRT("ldnode_p->unode_p is %x\n",
                (int)ldnode_p->unode_p);

            if (op_p->ophdlerset.unode_prt != NULL) {
                BKC_DLIST_SPF_CKRET =
                    op_p->ophdlerset.unode_prt(ldnode_p->unode_p,
                    op_p->clue_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET == 0,
                    1, BKC_DLIST_RERR);
            } else {
                BKC_DLIST_SPF_WARN("op_p->ophdlerset.unode_prt == NULL,"
                    "please have a check, its allright if we really do not"
                    "want to print the node detailed info\n");
            }
        }
        BKC_DLIST_SPF_RLYTHROW();
        BKC_DLIST_SPF_PRT("##################################\n");
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}


/*************************************************************************
 *                  bkc_dlist_get_nodenum
 *************************************************************************
 * parameters:
 *     dlist_p: the dlist which need to be printed its
 *         detailed infomation
 *     op_p: points to the operator that contains the
 *         print-info of the dlist
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     get the node nums in the dlist
 *     output file
 *     op_p->opcode must be BKC_DLIST_OPCODE_GET_NODE_NUM.
 *     bkc_dlist_operate can be referred for more info
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_get_nodenum(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && op_p != NULL,
            1, BKC_DLIST_RERR);
        BKC_DLIST_SPF_CKTHROW(op_p->opcode ==
            BKC_DLIST_OPCODE_GET_NODE_NUM, 1, BKC_DLIST_RERR);
        op_p->nodenum = dlist_p->tinum;
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_operate
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     commented in prototype declaration
 *     this is the main engine of the module
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_operate(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKTHROW(dlist_p != NULL && op_p != NULL,
            1, BKC_DLIST_RERR);

        switch (op_p->opcode) {
            case BKC_DLIST_OPCODE_ADD_HEAD_NODE:
            case BKC_DLIST_OPCODE_ADD_TAIL_NODE:
            case BKC_DLIST_OPCODE_ADD_PREV_NODE:
            case BKC_DLIST_OPCODE_ADD_NEXT_NODE:
                BKC_DLIST_SPF_CKRET = bkc_dlist_add_node(dlist_p, op_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_REMOVE_NODE:
            case BKC_DLIST_OPCODE_REMOVE_N_NODE:
            case BKC_DLIST_OPCODE_REMOVE_HEAD:
            case BKC_DLIST_OPCODE_REMOVE_TAIL:
            case BKC_DLIST_OPCODE_REMOVE_ALL:
                BKC_DLIST_SPF_CKRET =
                    bkc_dlist_remove_node(dlist_p, op_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_FIND_SPEC_NODE:
            case BKC_DLIST_OPCODE_FIND_NSPEC_NODE:
            case BKC_DLIST_OPCODE_FIND_HEAD_NODE:
            case BKC_DLIST_OPCODE_FIND_TAIL_NODE:
            case BKC_DLIST_OPCODE_FIND_PREV_NODE:
            case BKC_DLIST_OPCODE_FIND_NEXT_NODE:
            case BKC_DLIST_OPCODE_FIND_N_NODE:
            case BKC_DLIST_OPCODE_FIND_NODE:
                BKC_DLIST_SPF_CKRET = bkc_dlist_find_node(dlist_p, op_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_TRAVERSE_NODE:
                BKC_DLIST_SPF_CKRET =
                    bkc_dlist_traverse_node(dlist_p, op_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_GET_DLIST_SIZE:
                BKC_DLIST_SPF_CKRET =
                    bkc_dlist_get_dlsize(dlist_p, op_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_PRT_DLIST:
                BKC_DLIST_SPF_CKRET = bkc_dlist_prt_info(dlist_p, op_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_GET_NODE_NUM:
                BKC_DLIST_SPF_CKRET = bkc_dlist_get_nodenum(dlist_p, op_p);
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
                    BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
                break;
            case BKC_DLIST_OPCODE_EMPTY:
                BKC_DLIST_SPF_ERR("empty opcode,please have a check\n");
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_FABN,
                    1, BKC_DLIST_RERR);
                break;
            default:
                BKC_DLIST_SPF_ERR(
                    "wrong operate-opcode,please have a check\n");
                BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_FABN,
                    1, BKC_DLIST_RERR);
                break;
        }
        BKC_DLIST_SPF_RLYTHROW();
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

/*************************************************************************
 *                  bkc_dlist_watchmm
 *************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     more info
 *     this func can show the memory infomation about this module
 *     in debugging mode in which BKC_DLIST_SPF_RELEASE is not
 *     defined in bkc_dlist_spf.h. if BKC_DLIST_SPF_RELEASE is defined
 *     in bkc_dlist_spf.h, this func will always show empty messages
 *
 *     this function is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module. in this case
 *     this func can be temporarily export to bkc_dlist.h for outer
 *     refering
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_watchmm(void)
{
    BKC_DLIST_SPF_DITT(BKC_DLIST_RSUC, 0);

    BKC_DLIST_SPF_TRY {
        BKC_DLIST_SPF_CKRET = bkc_dlist_spf_watchmm();
        BKC_DLIST_SPF_CKTHROW(BKC_DLIST_SPF_CKRET ==
            BKC_DLIST_RSUC, 1, BKC_DLIST_RERR);
    }

    BKC_DLIST_SPF_CKPRET(BKC_DLIST_SPF_FNRET == BKC_DLIST_RSUC);
    return BKC_DLIST_SPF_FNRET;
}

