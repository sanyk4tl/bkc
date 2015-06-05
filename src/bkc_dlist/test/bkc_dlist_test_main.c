/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_dlist
 *  file name: bkc_dlist_test.c
 *  description:  facilities for testing the bkc_dlist mod
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
#include <stdio.h>
#include <stdlib.h>
#include "bkc_dlist_tspf.h"
#include "bkc_dlist.h"
/*********************definition, types & constants ***********************/
#define BKC_DLIST_T_WATCHM

typedef struct bkc_dlist_t_unode_s {
    int fm;
    int sm;
    int tm;
} bkc_dlist_t_unode_t;

int bkc_dlist_t_unode_destroy_func(void *unode_p, void *param_p);

int bkc_dlist_t_unode_prt_func(void *unode_p, void *param_p);

int bkc_dlist_t_unode_isitem_cmp_func(void *unode_p,
                void *cmp_clue_p);

int bkc_dlist_t_unode_traverse_func(void *unode_p, void *param_p);

unsigned int bkc_dlist_t_unode_getsize_func(void *unode_p, void *param_p);
/*********************variables******************************************/

/*********************prototype of local functions***********************/
void bkc_dlist_t_cd(void);
void bkc_dlist_t_cdat(void);
void bkc_dlist_t_cdatnr(void);
void bkc_dlist_t_cdah(void);
void bkc_dlist_t_cdap(void);
void bkc_dlist_t_cdan(void);
void bkc_dlist_t_cdarmv(void);
void bkc_dlist_t_cdarmvmul(void);
void bkc_dlist_t_cdfs(void);
void bkc_dlist_t_cdfns(void);
void bkc_dlist_t_cdfht(void);
void bkc_dlist_t_cdfpn(void);
void bkc_dlist_t_cdtraverse(void);
void bkc_dlist_t_cdgsz(void);
void bkc_dlist_t_cdpt(void);
void bkc_dlist_t_cdfnnode(void);
void bkc_dlist_t_cdgnnum(void);

/*********************implementation of open and local functions*********/
/*************************************************************************
 *                  bkc_dlist_t_unode_prt_func
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     print handler for test
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_t_unode_prt_func(void *unode_p, void *param_p)
{
    bkc_dlist_t_unode_t *iunode_p = NULL;

    if (unode_p != NULL) {
        iunode_p = (bkc_dlist_t_unode_t *)unode_p;
        printf("iunode_p is %p\n", iunode_p);
        printf("iunode_p->fm is %d\n", iunode_p->fm);
        printf("iunode_p->sm is %d\n", iunode_p->sm);
        printf("iunode_p->tm is %d\n", iunode_p->tm);
        return 0;
    } else {
        return -1;
    }
}

/*************************************************************************
 *                  bkc_dlist_t_unode_isitem_cmp_func
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     compare handler for test
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_t_unode_isitem_cmp_func(void *unode_p, void *cmp_clue_p)
{
    bkc_dlist_t_unode_t *iunode_p = NULL;
    int *num_p = NULL;
    int iscmp = -1;

    printf("bkc_dlist_t_unode_isitem_cmp_func is called\n");
    if (unode_p != NULL && cmp_clue_p != NULL) {
        iunode_p = (bkc_dlist_t_unode_t *)unode_p;
        num_p = (int *)cmp_clue_p;
        if (*num_p ==  iunode_p->fm && *num_p ==  iunode_p->sm
            && *num_p == iunode_p->tm) {
            iscmp = 0;
        }
    }

    return iscmp;
}

/*************************************************************************
 *                  bkc_dlist_t_unode_destroy_func
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     destroy handler for test
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_t_unode_destroy_func(void *unode_p, void *param_p)
{
    bkc_dlist_t_unode_t *iunode_p = NULL;

    if (unode_p != NULL) {
        iunode_p = (bkc_dlist_t_unode_t *)unode_p;
        iunode_p->fm = 0;
        iunode_p->sm = 0;
        iunode_p->tm = 0;
        printf("unode %p is freed\n", unode_p);

        free(iunode_p);
        return 0;
    } else {
        return -1;
    }
}

/*************************************************************************
 *                  bkc_dlist_t_unode_traverse_func
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     traverse handler for test
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_t_unode_traverse_func(void *unode_p, void *param_p)
{
    bkc_dlist_t_unode_t *iunode_p = NULL;

    if (unode_p != NULL) {
        iunode_p = (bkc_dlist_t_unode_t *)unode_p;
        iunode_p->fm = 99;
        iunode_p->sm = 99;
        iunode_p->tm = 99;
        printf("unodes is traversed\n");

        return 0;
    } else {
        return -1;
    }
}

/*************************************************************************
 *                  bkc_dlist_t_unode_getsize_func
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     get size handler for test
 *****************************  Notes  *****************************
 ************************************************************************/
unsigned int bkc_dlist_t_unode_getsize_func(void *unode_p, void *clue_p)
{
    bkc_dlist_t_unode_t *iunode_p = NULL;
    unsigned int size = 0;

    if (unode_p != NULL) {
        iunode_p = (bkc_dlist_t_unode_t *)unode_p;
        size = sizeof(*iunode_p);
        printf("unode size is calculated\n");
    }

    return size;
}

/*************************************************************************
 *                  bkc_dlist_t_cl
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create and destroy
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cd(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

#if 1
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_ADD_HEAD_NODE;
    op.tunode_p = 0xFFFFFFFF;
    bkc_dlist_operate(dlist_p, &op);
#endif

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    bkc_dlist_operate(dlist_p, &op);

    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_NOT_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = NULL;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif

    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdah
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add head and destroy
 *     ok
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cdah(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    int loop = 4;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_HEAD_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.clue_p = NULL;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdat
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add tail and destroy
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cdat(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    int loop = 4;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdatnr
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add tail and destroy, but not release
 *     the user node data
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cdatnr(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    int loop = 4;

    bkc_dlist_tspf_memdgn((bkc_dlist_tspf_malloc_func_t)malloc,
        (bkc_dlist_tspf_free_func_t)free);

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_NOT_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    bkc_dlist_destroy(dlist_p, &op);

    bkc_dlist_tspf_memdgn((bkc_dlist_tspf_malloc_func_t)malloc,
        (bkc_dlist_tspf_free_func_t)free);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdap
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add prev and destroy
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cdap(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    bkc_dlist_t_unode_t *fnode_p = NULL;
    int loop = 4;
    int clue = 0;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*find a user node*/
    clue = 3;
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_SPEC_NODE;
    op.clue_p = &clue;
    op.ophdlerset.unode_isitem_cmp = bkc_dlist_t_unode_isitem_cmp_func;
    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;
    /*new and add a user node*/
    unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
    unode_p->fm = unode_p->sm = unode_p->tm = 9;
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_ADD_PREV_NODE;
    op.tunode_p = unode_p;
    op.bunode_p = fnode_p;
    bkc_dlist_operate(dlist_p, &op);
    /*printf the dlist*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdan
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add next and destroy
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cdan(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    bkc_dlist_t_unode_t *fnode_p = NULL;
    int loop = 4;
    int clue = 0;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*find a user node*/
    clue = 2;
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_SPEC_NODE;
    op.clue_p = &clue;
    op.ophdlerset.unode_isitem_cmp = bkc_dlist_t_unode_isitem_cmp_func;
    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;
    /*new and add a user node*/
    unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
    unode_p->fm = unode_p->sm = unode_p->tm = 9;
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_ADD_NEXT_NODE;
    op.tunode_p = unode_p;
    op.bunode_p = fnode_p;
    bkc_dlist_operate(dlist_p, &op);
    /*printf the dlist*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdarmv
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add  and remove unode
 *****************************  Notes  *****************************
 ************************************************************************/
#define BKC_DLIST_T_CDARMV_DELUSERDATA
/*#define BKC_DLIST_T_CDARMV_NDELUSERDATA*/
void bkc_dlist_t_cdarmv(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    bkc_dlist_t_unode_t *fnode_p = NULL;
    int loop = 4;
    int clue = 0;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*find a user node*/
    clue = 2;
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_SPEC_NODE;
    op.clue_p = &clue;
    op.ophdlerset.unode_isitem_cmp = bkc_dlist_t_unode_isitem_cmp_func;
    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;

    /*remove the user node*/
#if defined(BKC_DLIST_T_CDARMV_DELUSERDATA)
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_REMOVE_NODE;
    op.tunode_p = fnode_p;
    op.rmvmode = BKC_DLIST_RM_DELUSERDATA;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_operate(dlist_p, &op);
#elif defined(BKC_DLIST_T_CDARMV_NDELUSERDATA)
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_REMOVE_NODE;
    op.tunode_p = fnode_p;
    op.rmvmode = BKC_DLIST_RM_NOT_DELUSERDATA;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_operate(dlist_p, &op);
#else
    printf("op.rmvmode is not set, please have a check\n");
#endif

    /*print the list*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*destroy*/
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdarmvmul
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add, remove n node , remove head , remove
 *     tail, and remove all nodes
 *****************************  Notes  *****************************
 ************************************************************************/
/*#define BKC_DLIST_T_CDARMVMUL_DELUSERDATA*/
#define BKC_DLIST_T_CDARMVMUL_NDELUSERDATA
void bkc_dlist_t_cdarmvmul(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    bkc_dlist_t_unode_t *fnode_p = NULL;
    int loop = 4;
    int clue = 0;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*find a user node*/
    clue = 2;
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_SPEC_NODE;
    op.clue_p = &clue;
    op.ophdlerset.unode_isitem_cmp = bkc_dlist_t_unode_isitem_cmp_func;
    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;

    /*remove the user node*/
    /*BKC_DLIST_OPCODE_REMOVE_N_NODE*/
#if 1
#if defined(BKC_DLIST_T_CDARMVMUL_DELUSERDATA)
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_REMOVE_N_NODE;
    op.pos = 0;
    op.rmvmode = BKC_DLIST_RM_DELUSERDATA;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_operate(dlist_p, &op);
#elif defined(BKC_DLIST_T_CDARMVMUL_NDELUSERDATA)
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_REMOVE_N_NODE;
    op.pos = 0;
    op.rmvmode = BKC_DLIST_RM_NOT_DELUSERDATA;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_operate(dlist_p, &op);
#else
    printf("op.rmvmode is not set, please have a check\n");
#endif
    printf("the returned user node is %p\n", op.runode_p);
#endif

    /*BKC_DLIST_OPCODE_REMOVE_HEAD*/
#if 0
#if defined(BKC_DLIST_T_CDARMVMUL_DELUSERDATA)
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_REMOVE_HEAD;
    op.rmvmode = BKC_DLIST_RM_DELUSERDATA;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_operate(dlist_p, &op);
#elif defined(BKC_DLIST_T_CDARMVMUL_NDELUSERDATA)
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_REMOVE_HEAD;
    op.rmvmode = BKC_DLIST_RM_NOT_DELUSERDATA;
    op.ophdlerset.unode_destroy = NULL;
    bkc_dlist_operate(dlist_p, &op);
#else
    printf("op.rmvmode is not set, please have a check\n");
#endif
    printf("the returned user node is %p\n", op.runode_p);
#endif

    /*BKC_DLIST_OPCODE_REMOVE_TAIL*/
#if 0
#if defined(BKC_DLIST_T_CDARMVMUL_DELUSERDATA)
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_REMOVE_TAIL;
    op.rmvmode = BKC_DLIST_RM_DELUSERDATA;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_operate(dlist_p, &op);
#elif defined(BKC_DLIST_T_CDARMVMUL_NDELUSERDATA)
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_REMOVE_TAIL;
    op.rmvmode = BKC_DLIST_RM_NOT_DELUSERDATA;
    op.ophdlerset.unode_destroy = NULL;
    bkc_dlist_operate(dlist_p, &op);
#else
    printf("op.rmvmode is not set, please have a check\n");
#endif

   printf("the returned user node is %p\n", op.runode_p);

#endif

    /*BKC_DLIST_OPCODE_REMOVE_ALL*/
#if 0
#if defined(BKC_DLIST_T_CDARMVMUL_DELUSERDATA)
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_REMOVE_ALL;
    op.rmvmode = BKC_DLIST_RM_DELUSERDATA;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_operate(dlist_p, &op);
#elif defined(BKC_DLIST_T_CDARMVMUL_NDELUSERDATA)
    printf("BKC_DLIST_T_CDARMVMUL_NDELUSERDATA\n");
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_REMOVE_ALL;
    op.rmvmode = BKC_DLIST_RM_NOT_DELUSERDATA;
    op.ophdlerset.unode_destroy = NULL;
    bkc_dlist_operate(dlist_p, &op);
#else
    printf("op.rmvmode is not set, please have a check\n");
#endif
#endif

    /*print the list*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*destroy*/
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdfs
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add  and find specific unode
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cdfs(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    bkc_dlist_t_unode_t *fnode_p = NULL;
    int loop = 4;
    int clue = 0;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    /*print the list*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*find a user node*/
    clue = 2;
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_SPEC_NODE;
    op.clue_p = &clue;
    op.ophdlerset.unode_isitem_cmp = bkc_dlist_t_unode_isitem_cmp_func;
    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;

    /*print the found node*/
    bkc_dlist_t_unode_prt_func(fnode_p, NULL);

    /*destroy*/
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdfns
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add  and find specific unode serially
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cdfns(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    bkc_dlist_t_unode_t *fnode_p = NULL;
    int loop = 4;
    int clue = 0;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    /*add the 5th node*/
    unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
    unode_p->fm = unode_p->sm = unode_p->tm = 2;
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
    op.tunode_p = unode_p;
    bkc_dlist_operate(dlist_p, &op);

    /*print the list*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*find a user node*/
    clue = 2;
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_NSPEC_NODE;
    op.clue_p = &clue;
    op.tunode_p = NULL;
    op.ophdlerset.unode_isitem_cmp = bkc_dlist_t_unode_isitem_cmp_func;

    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;
    while (fnode_p != NULL) {
        /*print the found node*/
        bkc_dlist_t_unode_prt_func(fnode_p, NULL);
        /*find the next specific node in this dlist*/
        bkc_dlist_operate(dlist_p, &op);
        fnode_p = op.runode_p;
    }

    /*destroy*/
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdfnnode
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add  and find the nth unode
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cdfnnode(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    bkc_dlist_t_unode_t *fnode_p = NULL;
    int loop = 4;
    int clue = 0;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    /*add the 5th node*/
    unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
    unode_p->fm = unode_p->sm = unode_p->tm = 2;
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
    op.tunode_p = unode_p;
    bkc_dlist_operate(dlist_p, &op);

    /*print the list*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*find a user node*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_N_NODE;
    op.pos = 4;
    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;

    if (fnode_p != NULL){
        printf("ok ,the found node is as follows\n");
        bkc_dlist_t_unode_prt_func(fnode_p, NULL);
    } else {
        printf("ok ,not found node ");
    }

    /*destroy*/
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdgnodenum
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add  and get the node num in dlist
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cdgnnum(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    int loop = 4;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_GET_NODE_NUM;
    bkc_dlist_operate(dlist_p, &op);

    printf("ok , node num in dlist is %d\n", op.nodenum);

    /*destroy*/
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdfh
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create, add, find head and tail unode
 *****************************  Notes  *****************************
 ************************************************************************/
/*#define BKC_DLIST_T_CDFHT_HEAD*/
#define BKC_DLIST_T_CDFHT_TAIL
void bkc_dlist_t_cdfht(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    bkc_dlist_t_unode_t *fnode_p = NULL;
    int loop = 4;
    int clue = 0;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    /*print the list*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

#if defined(BKC_DLIST_T_CDFHT_HEAD)
    /*find a head node*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_HEAD_NODE;
    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;

    if (fnode_p != NULL) {
        /*print the found node*/
        printf("ok, head node is found\n");
        bkc_dlist_t_unode_prt_func(fnode_p);
    }
#elif defined(BKC_DLIST_T_CDFHT_TAIL)
    /*find a tail node*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_TAIL_NODE;
    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;

    if (fnode_p != NULL) {
        /*print the found node*/
        printf("ok, tail node is found\n");
        bkc_dlist_t_unode_prt_func(fnode_p, NULL);
    }
#else
    printf("op.opcode is not set, please have a check\n");
#endif

    /*destroy*/
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdfpn
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add  and find prev and next unode
 *****************************  Notes  *****************************
 ************************************************************************/
#define BKC_DLIST_T_CDFPN_PREV
/*#define BKC_DLIST_T_CDFPN_NEXT*/
void bkc_dlist_t_cdfpn(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    bkc_dlist_t_unode_t *fnode_p = NULL;
    int loop = 4;
    int clue = 0;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    /*print the list*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*find a user node*/
    clue = 2;
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_NSPEC_NODE;
    op.clue_p = &clue;
    op.tunode_p = NULL;
    op.ophdlerset.unode_isitem_cmp = bkc_dlist_t_unode_isitem_cmp_func;
    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;

#if defined(BKC_DLIST_T_CDFPN_PREV)
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_PREV_NODE;
    op.tunode_p = fnode_p;
    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;

    if (fnode_p != NULL) {
        /*print the found node*/
        printf("ok, prev node is found\n");
        bkc_dlist_t_unode_prt_func(fnode_p, NULL);
    }
#elif defined(BKC_DLIST_T_CDFPN_NEXT)
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_FIND_NEXT_NODE;
    op.tunode_p = fnode_p;
    bkc_dlist_operate(dlist_p, &op);
    fnode_p = op.runode_p;

    if (fnode_p != NULL) {
        /*print the found node*/
        printf("ok, next node is found\n");
        bkc_dlist_t_unode_prt_func(fnode_p);
    }
#else
    printf("op.opcode is not set, please have a check\n");
#endif

    /*destroy*/
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdtraverse
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create , add  and traverse unodes
 *****************************  Notes  *****************************
 ************************************************************************/

void bkc_dlist_t_cdtraverse(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    int loop = 4;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    /*print the list*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*traverse all the nodes*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_TRAVERSE_NODE;
    op.ophdlerset.unode_traverse = bkc_dlist_t_unode_traverse_func;
    bkc_dlist_operate(dlist_p, &op);

    /*print the list*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*destroy*/
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdgsz
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create, add and get size of unodes in the list .
 *     and then, destroy the dlist
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cdgsz(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    int loop = 4;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    /*print the list*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*get size of all the nodes*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_GET_DLIST_SIZE;
    op.ophdlerset.unode_get_nsize = bkc_dlist_t_unode_getsize_func;
    bkc_dlist_operate(dlist_p, &op);

    /*print the result*/
    printf("the sizeof the dlist is %d\n", op.dlsize);

    /*destroy*/
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

/*************************************************************************
 *                  bkc_dlist_t_cdpt
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create, add, print and destroy
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_dlist_t_cdpt(void)
{
    bkc_dlist_t *dlist_p = NULL;
    bkc_dlist_operator_t op;
    bkc_dlist_t_unode_t *unode_p = NULL;
    int loop = 4;

    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
    dlist_p = bkc_dlist_create(&op);

    /*add 4 user nodes to dlist*/
    for (loop = 0; loop < 4; loop++) {
        unode_p = malloc(sizeof(bkc_dlist_t_unode_t));
        unode_p->fm = unode_p->sm = unode_p->tm = loop;
        bkc_dlist_definit_op(&op);
        op.opcode = BKC_DLIST_OPCODE_ADD_TAIL_NODE;
        op.tunode_p = unode_p;
        bkc_dlist_operate(dlist_p, &op);
    }

    /*print the list*/
    bkc_dlist_definit_op(&op);
    op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
    op.clue_p = NULL;
    op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
    bkc_dlist_operate(dlist_p, &op);

    /*destroy*/
    bkc_dlist_definit_op(&op);
    op.dsymode = BKC_DLIST_DM_DELUSERDATA;
    op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
    op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
    bkc_dlist_destroy(dlist_p, &op);

#ifdef BKC_DLIST_T_WATCHM
    bkc_dlist_watchmm();
#endif
    return ;
}

int main(int argc, char*argv[])
{
#if 1
    printf("test bkc_dlist_t_cd\n");
    bkc_dlist_t_cd();
#endif

#if 0
    printf("test bkc_dlist_t_cda\n");
    bkc_dlist_t_cdah();
#endif

#if 0
    printf("test bkc_dlist_t_cdat\n");
    bkc_dlist_t_cdat();
#endif

#if 0
    printf("test bkc_dlist_t_cdap\n");
    bkc_dlist_t_cdap();
#endif

#if 0
    printf("test bkc_dlist_t_cdan\n");
    bkc_dlist_t_cdan();
#endif

#if 0
    printf("test bkc_dlist_t_cdarmv\n");
    bkc_dlist_t_cdarmv();
#endif

#if 0
    printf("test bkc_dlist_t_cdfs\n");
    bkc_dlist_t_cdfs();
#endif

#if 0
    printf("test bkc_dlist_t_cdfns\n");
    bkc_dlist_t_cdfns();
#endif

#if 0
    printf("test bkc_dlist_t_cdfht\n");
    bkc_dlist_t_cdfht();
#endif

#if 0
    printf("test bkc_dlist_t_cdfpn\n");
    bkc_dlist_t_cdfpn();
#endif

#if 0
    printf("test bkc_dlist_t_cdtraverse\n");
    bkc_dlist_t_cdtraverse();
#endif

#if 0
    printf("test bkc_dlist_t_cdgsz\n");
    bkc_dlist_t_cdgsz();
#endif

#if 0
    printf("test bkc_dlist_t_cdpt\n");
    bkc_dlist_t_cdpt();
#endif

#if 0
    printf("test bkc_dlist_t_cdatnr\n");
    bkc_dlist_t_cdatnr();
#endif

#if 0
    printf("test bkc_dlist_t_cdfnnode\n");
    bkc_dlist_t_cdfnnode();
#endif

#if 0
    printf("test bkc_dlist_t_cdgnnum\n");
    bkc_dlist_t_cdgnnum();
#endif

#if 0
    printf("test bkc_dlist_t_cdarmvmul\n");
    bkc_dlist_t_cdarmvmul();
#endif

    return 0;
}

