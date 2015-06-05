/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_dlist
 *  file name: bkc_dlist.h
 *  description: N/A
 *************************************************************************
 *  keys:
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
#ifndef BKC_DLIST_H
#define BKC_DLIST_H
/*********************included files*************************************/

/*********************definition, types & constants ***********************/
/*ret value of funcs in bkc_dlist*/
#define BKC_DLIST_RSUC  (0)
#define BKC_DLIST_RERR  (-1)

/*operation code*/
typedef enum bkc_dlist_opcode_e {
    BKC_DLIST_OPCODE_EMPTY,
    /*create and destroy dlist*/
    BKC_DLIST_OPCODE_CREATE_LIST,
    BKC_DLIST_OPCODE_DESTROY_LIST,
    /*add node to the dlist*/
    BKC_DLIST_OPCODE_ADD_HEAD_NODE,
    BKC_DLIST_OPCODE_ADD_TAIL_NODE,
    BKC_DLIST_OPCODE_ADD_PREV_NODE,
    BKC_DLIST_OPCODE_ADD_NEXT_NODE,
    /*remove node from the dlist*/
    BKC_DLIST_OPCODE_REMOVE_NODE,
    BKC_DLIST_OPCODE_REMOVE_N_NODE,
    BKC_DLIST_OPCODE_REMOVE_HEAD,
    BKC_DLIST_OPCODE_REMOVE_TAIL,
    BKC_DLIST_OPCODE_REMOVE_ALL,
    /*find node in the dlist*/
    BKC_DLIST_OPCODE_FIND_NODE,
    BKC_DLIST_OPCODE_FIND_SPEC_NODE,
    BKC_DLIST_OPCODE_FIND_NSPEC_NODE,
    BKC_DLIST_OPCODE_FIND_HEAD_NODE,
    BKC_DLIST_OPCODE_FIND_TAIL_NODE,
    BKC_DLIST_OPCODE_FIND_PREV_NODE,
    BKC_DLIST_OPCODE_FIND_NEXT_NODE,
    /*find the node on the n position in the dlist*/
    BKC_DLIST_OPCODE_FIND_N_NODE,
    /*traverse all nodes in the dlist*/
    BKC_DLIST_OPCODE_TRAVERSE_NODE,
    /*get the node numbers in the dlist*/
    BKC_DLIST_OPCODE_GET_NODE_NUM,
    /*get the dlist size in bytes*/
    BKC_DLIST_OPCODE_GET_DLIST_SIZE,
    /*print the information of the dlist*/
    BKC_DLIST_OPCODE_PRT_DLIST
} bkc_dlist_opcode_m;

/*destroy mode of a dlist*/
typedef enum bkc_dlist_dsymode_e {
    BKC_DLIST_DM_EMPTY,
    BKC_DLIST_DM_DELUSERDATA,
    BKC_DLIST_DM_NOT_DELUSERDATA
} bkc_dlist_dsymode_m;

/*remove mode of a dlist*/
typedef enum bkc_dlist_rmvmode_e {
    BKC_DLIST_RM_EMPTY,
    BKC_DLIST_RM_DELUSERDATA,
    BKC_DLIST_RM_NOT_DELUSERDATA
} bkc_dlist_rmvmode_m;

/*handlers of a dlist*/
/*return value:
  0 means success,
  other means error*/
typedef int (*bkc_dlist_unode_destroy_func_t)(void *unode_p, void *param_p);
/*return value:
  0 means unode_p is matched by cmp_clue_p,
  other means unmatched*/
typedef int (*bkc_dlist_unode_isitem_cmp_func_t)(void *unode_p,
                void *cmp_clue_p);
/*return value:
  the size of the user node measured in bytes*/
typedef unsigned int (*bkc_dlist_unode_get_nsize_func_t)(void *unode_p,
    void *param_p);

/*manipulation applied on the user node when traversing
  over all the nodes in dlist.
  return value:
  0 means success,
  other means error*/
typedef int (*bkc_dlist_unode_traverse_func_t)(void *unode_p,
    void *param_p);

/*print handler of each unode, and can be used to print the detailed
  information of each node in dlist.
  return value:
  0 means success,
  other means error*/
typedef int (*bkc_dlist_unode_prt_func_t)(void *unode_p, void *param_p);

/*types forward declaration in bkc_dlist module*/
typedef struct bkc_dlist_ophdlerset_tag_t bkc_dlist_ophdlerset_t;
typedef struct bkc_dlist_operator_tag_t bkc_dlist_operator_t;
typedef struct bkc_dlist_tag_t bkc_dlist_t;

/*operation-handler set of a dlist*/
struct bkc_dlist_ophdlerset_tag_t {
    /*for destroying user node*/
    bkc_dlist_unode_destroy_func_t unode_destroy;
    /*for comparing the user node by the cmp_clue_p*/
    bkc_dlist_unode_isitem_cmp_func_t unode_isitem_cmp;
    /*for geting user node size*/
    bkc_dlist_unode_get_nsize_func_t unode_get_nsize;
    /*for manipulation applied to the unode when
      tarversing all nodes in dlist*/
    bkc_dlist_unode_traverse_func_t unode_traverse;
    /*for printing the detailed information of
      each user node in dlist*/
    bkc_dlist_unode_prt_func_t unode_prt;
};

/*operator of a dlist*/
struct bkc_dlist_operator_tag_t {
    /*the type of the operation applied to the dlist*/
    bkc_dlist_opcode_m opcode;
    /*target user node for add ,find, remove*/
    void *tunode_p;
    /*beacon user node for add*/
    void *bunode_p;
    /*result unode for find*/
    void *runode_p;
    /*point to clue data for handler func,such as unode_istem_cmp*/
    void *clue_p;
    /*size in bytes of all user nodes of this dlist*/
    unsigned int dlsize;
    /*handler-set of the dlist*/
    bkc_dlist_ophdlerset_t ophdlerset;
    /*for the destroy mode:
      BKC_DLIST_DM_DELUSERDATA: delete the user node data by
          unode_destroy handler in ophdlerset
      BKC_DLIST_DM_DELUSERDATA: will not del user node data*/
    bkc_dlist_dsymode_m dsymode;
    /*for the remove mode:
      BKC_DLIST_RM_DELUSERDATA: remove the user node designated by
          unode_p, and destroy it's data by unode_destroy in ophdlerset
      BKC_DLIST_RM_NOT_DELUSERDATA: just remove the user node from dlist,
          wont destroy user node data*/
    bkc_dlist_rmvmode_m rmvmode;

    /*for designating the position of the user node. used in find and remove
      with the the op_code == BKC_DLIST_OPCODE_FIND_N_NODE and
      BKC_DLIST_OPCODE_REMOVE_N_NODE*/
    unsigned int pos;

    /*for returning the node nums in the dlist when
      op_code == BKC_DLIST_OPCODE_GET_NODE_NUM*/
    unsigned int nodenum;
};

/*********************prototype of open functions************************/
/*************************************************************************
 *                  bkc_dlist_create
 *************************************************************************
 * parameters:
 *     op_p: points to the operator that contains the
 *          create-info of the dlist
 * return value:
 *     Not NULL: points to the sucessfully created dlist
 *     NULL: abnormal return
 * description:
 *     caller will passes the create-info to this function
 *     through filling in the input-attributes in op_p.
 *     the create operation have no output-attributes in op_p
 *
 *     corresponding input and output attributes in op_p for
 *     bkc_dlist_create are as below:
 *     input-attributes:
 *         op_p->opcode:
 *             must be BKC_DLIST_OPCODE_CREATE_LIST
 *             if not, the return value will be NULL,
 *             and will get an error message
 *     output-attributes: N/A
 *
 *     if creating failed, the return value will be NULL,
 *     and will get an error message
 *
 *     a general usecase looks like:
 *
 *     bkc_dlist_t *dlist_p = NULL;
 *     bkc_dlist_operator_t op;
 *     bkc_dlist_definit_op(&op);
 *     op.opcode = BKC_DLIST_OPCODE_CREATE_LIST;
 *     dlist_p = bkc_dlist_create(&op);
 *
 *     for detailed usage information, can refers to bkc_dlist_t_cd()
 *     in /test/bkc_dlist_test_main.c
 *****************************  Notes  *****************************
 ************************************************************************/
bkc_dlist_t *bkc_dlist_create(bkc_dlist_operator_t *op_p);

/*************************************************************************
 *                  bkc_dlist_destroy
 *************************************************************************
 * parameters:
 *     dlist_p: points to the dlist that need to be destroyed
 *     op_p: points to the operator that contains the
 *          destroy-info of the dlist
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     the dlist is created by bkc_dlist_create()
 *
 *     caller will passes the destroy-info through the
 *     input-attributes in op_p. moreover, the caller
 *     can get the wanted result from the output-attributes in op_p.
 *
 *     corresponding input and output attributes of bkc_dlist_destroy
 *     in op_p are as below:
 *     input-attributes:
 *         op_p->opcode:
 *             must be BKC_DLIST_OPCODE_DESTROY_LIST
 *             if not, this func will return erroneously and
 *             get an error message
 *         op_p->dsymode:
 *             BKC_DLIST_DM_DELUSERDATA: destroy the user node
 *                 data by unode_destroy in op_p->ophdlerset
 *             BKC_DLIST_DM_NOT_DELUSERDATA: will not destroy user node
 *                 data, and all the user data will be left untouched
 *         op_p->clue_p:
 *             the second param for calling the destroy func.
 *             this can be NULL if the second param is not needed
 *         op_p->ophdlerset.unode_destroy:
 *             the user node destroy handler,
 *             can not be NULL when op_p->dsymode ==
 *             BKC_DLIST_DM_DELUSERDATA
 *             if NULL, this func will return erroneously and
 *             get an error message
 *     output-attributes: N/A
 *
 *     if destroying failed, the return value will not be
 *     BKC_DLIST_RSUC, and will get an error message
 *
 *     a general usecase looks like:
 *
 *     bkc_dlist_definit_op(&op);
 *     op.dsymode = BKC_DLIST_DM_DELUSERDATA;
 *     op.opcode = BKC_DLIST_OPCODE_DESTROY_LIST;
 *     op.clue_p = NULL;//can be any param for 
 *         //bkc_dlist_t_unode_destroy_func
 *     op.ophdlerset.unode_destroy = bkc_dlist_t_unode_destroy_func;
 *     bkc_dlist_destroy(dlist_p, &op);
 *
 *     for detailed usage information, can refers to bkc_dlist_t_cd()
 *     in /test/bkc_dlist_test_main.c
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_destroy(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p);

/*************************************************************************
 *                  bkc_dlist_operate
 *************************************************************************
 * parameters:
 *     dlist_p: points to the dlist which will be manipulated according
 *         to op_p
 *     op_p: points to the bkc_dlist_operator_t that desigates the
 *         specific manipulation applied to this dlist
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     dlist_p is created by bkc_dlist_create, and can be destroyed by
 *     bkc_dlist_destroy.
 *
 *     specific operation code and corresponding input and output
 *     attributes in op_p are listed as below upon the return value
 *     is BKC_DLIST_RSUC, otherwise they are not defined:
 *
 *     opcode: BKC_DLIST_OPCODE_ADD_TAIL_NODE
 *     doing: add a user node to the tail of the list
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_ADD_TAIL_NODE
 *         op_p->tunode_p:
 *             points to the user node which need be added to the dlist
 *             can not be NULL
 *             if NULL, no operation will be applied , and this func
 *             will return erroneously with an error message
 *     output-attributes: N/A
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdat(void),void bkc_dlist_t_cdatnr(void)
 *         in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_ADD_HEAD_NODE
 *     doing: add a user node to the head of the list
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_ADD_HEAD_NODE
 *         op_p->tunode_p:
 *             points to the user node which need be added to the dlist
 *             can not be NULL
 *             if NULL, no operation will be applied , and this func
 *             will return erroneously with an error message
 *     output-attributes: N/A
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdah(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_ADD_PREV_NODE
 *     doing: add the user node to dlist as the previous partner of the
 *         user node designated by op_p->bunode_p in op_p.
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_ADD_PREV_NODE
 *         op_p->tunode_p:
 *             points to the user node which need to be
 *             added to the dlist
 *             can not be NULL
 *             if NULL, no operation will be applied , and this func
 *             will return erroneously with an error message
 *         op_p->bunode_p:
 *             points to the beacon user node used for adding
 *     output-attributes: N/A
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdap(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_ADD_NEXT_NODE
 *     doing: add the user node to dlist as the next partner of the
 *         user node designated by op_p->bunode_p in op_p.
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_ADD_NEXT_NODE
 *         op_p->tunode_p:
 *             points to the user node which need to be added
 *             to the dlist
 *             can not be NULL
 *             if NULL, no operation will be applied , and this func
 *             will return erroneously with an error message
 *         op_p->bunode_p:
 *             points to the beacon user node used for adding
 *             can not be NULL
 *             if NULL, no operation will be applied , and this func
 *             will return erroneously with an error message
 *     output-attributes: N/A
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdan(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_REMOVE_NODE
 *     doing: remove the user node from dlist
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_REMOVE_NODE
 *         op_p->tunode_p:
 *             points to the user node which need to be
 *             removed from the dlist
 *         op_p->clue_p:
 *             the second param for calling the destroy func.
 *             this can be NULL if the second param is not needed
 *         op_p->rmvmode:
 *             BKC_DLIST_RM_DELUSERDATA: remove the user node
 *                 designated by tunode_p, and destroy it's data by
 *                 op_p->ophdlerset.unode_destroy
 *             BKC_DLIST_RM_NOT_DELUSERDATA: just remove the user node
 *                 from the dlist, and wont destroy user node data
 *         op_p->ophdlerset.unode_destroy:
 *             the user node destroy handler,
 *             can not be NULL when op_p->rmvmode ==
 *             BKC_DLIST_RM_DELUSERDATA
 *             if NULL, this func will return erroneously and
 *             get an error message
 *     output-attributes: N/A
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdarmv(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_REMOVE_N_NODE
 *     doing: remove the num-th user node designated by
 *             op_p->pos from the dlist
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_REMOVE_NODE
 *         op_p->pos:
 *             designated which node need to be removed
 *         op_p->clue_p:
 *             the second param for calling the destroy func.
 *             this can be NULL if the second param is not needed
 *         op_p->rmvmode:
 *             BKC_DLIST_RM_DELUSERDATA: remove the user node
 *                 designated by tunode_p, and destroy it's data by
 *                 op_p->ophdlerset.unode_destroy
 *             BKC_DLIST_RM_NOT_DELUSERDATA: just remove the user node
 *                 from the dlist, and wont destroy user node data
 *         op_p->ophdlerset.unode_destroy:
 *             the user node destroy handler,
 *             can not be NULL when op_p->rmvmode ==
 *             BKC_DLIST_RM_DELUSERDATA
 *             if NULL, this func will return erroneously and
 *             get an error message
 *     output-attributes:
 *         op_p->runode_p will return the designated node which need to
 *         be removed if op_p->rmvmode == BKC_DLIST_RM_NOT_DELUSERDATA.
 *         but if op_p->rmvmode == BKC_DLIST_RM_DELUSERDATA,
 *         op_p->runode_p will be NULL after removing
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdarmvmul(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_REMOVE_HEAD
 *     doing:  remove the head node from the dlist
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_REMOVE_HEAD
 *         op_p->rmvmode:
 *             BKC_DLIST_RM_DELUSERDATA: remove the user node
 *                 designated by tunode_p, and destroy it's data by
 *                 op_p->ophdlerset.unode_destroy
 *             BKC_DLIST_RM_NOT_DELUSERDATA: just remove the user node
 *                 from the dlist, and wont destroy user node data
 *         op_p->clue_p:
 *             the second param for calling the destroy func.
 *             this can be NULL if the second param is not needed
 *         op_p->ophdlerset.unode_destroy:
 *             the user node destroy handler,
 *             can not be NULL when op_p->rmvmode ==
 *             BKC_DLIST_RM_DELUSERDATA, if NULL, this func will
 *             return erroneously and get an error message
 *     output-attributes:
 *         op_p->runode_p will return the designated node which need to
 *         be removed if op_p->rmvmode == BKC_DLIST_RM_NOT_DELUSERDATA.
 *         but if op_p->rmvmode == BKC_DLIST_RM_DELUSERDATA,
 *         op_p->runode_p will be NULL after removing
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdarmvmul(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_REMOVE_TAIL
 *     doing:  remove the tail node from the dlist
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_REMOVE_TAIL
 *         op_p->rmvmode:
 *             BKC_DLIST_RM_DELUSERDATA: remove the user node
 *                 designated by tunode_p, and destroy it's data by
 *                 op_p->ophdlerset.unode_destroy
 *             BKC_DLIST_RM_NOT_DELUSERDATA: just remove the user node
 *                 from the dlist, and wont destroy user node data
 *         op_p->clue_p:
 *             the second param for calling the destroy func.
 *             this can be NULL if the second param is not needed
 *         op_p->ophdlerset.unode_destroy:
 *             the user node destroy handler,
 *             can not be NULL when op_p->rmvmode ==
 *             BKC_DLIST_RM_DELUSERDATA
 *             if NULL, this func will return erroneously and
 *             get an error message
 *     output-attributes:
 *         op_p->runode_p will return the designated node which need to
 *         be removed if op_p->rmvmode == BKC_DLIST_RM_NOT_DELUSERDATA.
 *         but if op_p->rmvmode == BKC_DLIST_RM_DELUSERDATA,
 *         op_p->runode_p will be NULL after removing
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdarmvmul(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_REMOVE_ALL
 *     doing:  remove all the nodes from the dlist
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_REMOVE_ALL
 *         op_p->rmvmode:
 *             BKC_DLIST_RM_DELUSERDATA: remove the user node
 *                 designated by tunode_p, and destroy it's data by
 *                 op_p->ophdlerset.unode_destroy
 *             BKC_DLIST_RM_NOT_DELUSERDATA: just remove the user node
 *                 from the dlist, and wont destroy user node data
 *         op_p->clue_p:
 *             the second param for calling the destroy func.
 *             this can be NULL if the second param is not needed
 *         op_p->ophdlerset.unode_destroy:
 *             the user node destroy handler,
 *             can not be NULL when op_p->rmvmode ==
 *             BKC_DLIST_RM_DELUSERDATA
 *             if NULL, this func will return erroneously and
 *             get an error message
 *     output-attributes: N/A
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdarmvmul(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_FIND_SPEC_NODE
 *     doing: find a specific user node by the clue_p given in op_p
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_FIND_SPEC_NODE
 *         op_p->clue_p:
 *             point to clue data for
 *             op_p->ophdlerset.unode_istem_cmp.
 *             can not be NULL
 *             if NULL, this func will return erroneously and
 *             get an error message
 *         op_p->ophdlerset.unode_istem_cmp:
 *             the user node compare handler.
 *             can not be NULL
 *             if NULL, this func will return erroneously and
 *             get an error message
 *     output-attributes:
 *         op_p->runode_p:
 *             Not NULL: points to the found user node
 *             NULL: not found the specific user node by the given
 *                 op_p->clue_p
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdfs(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_FIND_NSPEC_NODE
 *     doing: find a next specific user node by the clue_p given in op_p
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_FIND_NSPEC_NODE
 *         op_p->clue_p:
 *             point to clue data for
 *             op_p->ophdlerset.unode_istem_cmp.
 *             can not be NULL
 *             if NULL, this func will return erroneously and
 *             get an error message
 *         op_p->ophdlerset.unode_istem_cmp:
 *             the user node compare handler.
 *             can not be NULL.
 *             if NULL, this func will return erroneously and
 *             get an error message
 *         op_p->tunode_p:
 *             when first searching, can be manually set NULL,
 *             in the following searching for the same target node,
 *             this attribute need not be reset or changed manually.
 *             in fact, it will automatically be set as the value of
 *             op_p->runode_p which is gotten from last searching, and
 *             need not be reset or changed again.
 *     output-attributes:
 *         op_p->runode_p:
 *             Not NULL: points to the found user node
 *             NULL: not found the specific user node by the given
 *                 op_p->clue_p
 *     more info:
 *         this is usefull for find all the designated
 *         nodes in the dlist
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdfns(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_FIND_HEAD_NODE
 *     doing: find the head user node in dlist
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_FIND_HEAD_NODE
 *     output-attributes:
 *         op_p->runode_p:
 *             Not NULL: points to the found user node
 *             NULL: not found the specific user node
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdfht(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_FIND_TAIL_NODE
 *     doing: find the tail user node in dlist
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_FIND_TAIL_NODE
 *     output-attributes:
 *         op_p->runode_p:
 *             Not NULL: points to the found user node
 *             NULL: not found the specific user node
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdfht(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_FIND_PREV_NODE
 *     doing: find the previous partner of the user node designated
 *         by the op_p->tunode_p
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_FIND_PREV_NODE
 *         op_p->tunode_p:
 *             points to the user node of which the previous partner
 *                 will be returned
 *             can not be NULL
 *             if NULL, the op_p->runode_p will be NULL, and this
 *             func will return erroneously with an error message
 *     output-attributes:
 *         op_p->runode_p:
 *             Not NULL: points to the found user node
 *             NULL: not found the specific user node by the given
 *                 op_p->clue_p
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdfpn(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_FIND_NEXT_NODE
 *     doing: find the next partner of the user node designated
 *         by the op_p->tunode_p
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_FIND_NEXT_NODE
 *         op_p->tunode_p:
 *             points to the user node of which the next partner
 *                 will be returned
 *             can not be NULL
 *             if NULL, the op_p->runode_p will be NULL, and this
 *             func will return erroneously with an error message
 *     output-attributes:
 *         op_p->runode_p:
 *             Not NULL: points to the found user node
 *             NULL: not found the specific user node by the given
 *                 op_p->clue_p
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdfpn(void) in /test/bkc_dlist_test_main.c
 * 
 *     opcode: BKC_DLIST_OPCODE_FIND_NODE
 *     doing: find the designated user node in this list, if the
 *         it exists, it will be returned in by the op_p->tunode_p
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_FIND_NODE
 *         op_p->tunode_p:
 *             points to the user node 
 *             can not be NULL
 *             if NULL, the op_p->runode_p will be NULL, and this
 *             func will return erroneously with an error message
 *     output-attributes:
 *         op_p->runode_p:
 *             Not NULL: points to the found user node
 *             NULL: not found the specific user node 
 *     more info:
 *
 *     opcode: BKC_DLIST_OPCODE_FIND_N_NODE
 *     doing: find the node on the designated position of the dlist
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_FIND_N_NODE
 *         op_p->pos:
 *             the position of the node which will be found in
 *             the dlist
 *     output-attributes:
 *         op_p->runode_p:
 *             Not NULL: points to the found user node
 *             NULL: not found the specific user node by the given
 *                 op_p->clue_p
 *     more info:
 *         for detailed usage information, can refers to the following funcs
 *         in /test/bkc_dlist_test_main.c:
 *         void bkc_dlist_t_cdfnnode(void)
 *
 *     opcode: BKC_DLIST_OPCODE_TRAVERSE_NODE
 *     doing: traverse all the user nodes in the dlist, and apply
 *         the op_p->ophdlerset.unode_traverse manipulation to each
 *         user node in the dlist
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_TRAVERSE_NODE
 *         op_p->clue_p:
 *             the second param for calling the printing func.
 *             this can be NULL if the second param is not needed
 *         op_p->ophdlerset.unode_traverse:
 *             the traverse handler which will be applied to each
 *             user node.
 *             if NULL no operation will be applied and
 *             will get a warning
 *     output-attributes: N/A
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdtraverse(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_GET_DLIST_SIZE
 *     doing: get the size in bytes of all the user node in dlist
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_GET_DLIST_SIZE
 *         op_p->ophdlerset.unode_get_nsize:
 *             the handler which will be used to get size in bytes of
 *             each user node in dlist
 *             if NULL no operation will be applied and
 *             will get a warning, op_p->dlsize will be 0
 *     output-attributes:
 *         op_p->dlsize:
 *             the size in bytes of all the user nodes contained in
 *             this dlist
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdgsz(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_PRT_DLIST
 *     doing: print the information of the dlist and its nodes
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_PRT_DLIST
  *        op_p->clue_p:
 *             the second param for calling the traversing func.
 *             this can be NULL if the second param is not needed
 *         op_p->ophdlerset.unode_prt:
 *             the handler which will be used to print the node-info
 *             if NULL no operation will be applied and
 *             will get a warning
 *     output-attributes: N/A
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdpt(void) in /test/bkc_dlist_test_main.c
 *
 *     opcode: BKC_DLIST_OPCODE_GET_NODE_NUM
 *     doing: get the nodes num in the dlist
 *     input-attributes:
 *         op_p->opcode:
 *             BKC_DLIST_OPCODE_GET_NODE_NUM
 *     output-attributes:
 *         op_p->nodenum:
 *             the number of the nodes in the dlist
 *     more info:
 *         for detailed usage information, can refers to
 *         void bkc_dlist_t_cdpt(void) in /test/bkc_dlist_test_main.c.
 *         if operating failed, the return value will not be
 *         BKC_DLIST_RSUC, and will get an error message
 *
 *     a general usecase for all operations looks like:
 *
 *     bkc_dlist_definit_op(&op);
 *     op.opcode = BKC_DLIST_OPCODE_PRT_DLIST;
 *     op.ophdlerset.unode_prt = bkc_dlist_t_unode_prt_func;
 *     bkc_dlist_operate(dlist_p, &op);
 *
 *     for detailed usage information, can refers to
 *     /test/bkc_dlist_test_main.c
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_operate(bkc_dlist_t *dlist_p, bkc_dlist_operator_t *op_p);

/*************************************************************************
 *                  bkc_dlist_definit_op
 *************************************************************************
 * parameters:
 *     op_p: points to the operator which need to be defaultly inited
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     defaultly init the given operator.
 *     this func need to be called before a operator is applied to
 *     an dlist operation. and all the members in bkc_dlist_operator_t
 *     will be assigned a default value
 *
 *     more, when a bkc_dlist_operator_t has already been
 *     used for one operation designated by op_p->opcode,
 *     and will be used for another operation sequentially, this func
 *     can also be called to reinit the bkc_dlist_operator_t for the
 *     new operation
 *
 *     if definit failed, the return value will not be BKC_DLIST_RSUC,
 *     and will get an error message
 *
 *     a general usecase looks like:
 *
 *     bkc_dlist_operator_t op;
 *     bkc_dlist_definit_op(&op);
 *
 *     for detailed information, can refers to bkc_dlist_t_cd()
 *     in /test/bkc_dlist_test_main.c
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_definit_op(bkc_dlist_operator_t *op_p);

/*************************************************************************
 *                  bkc_dlist_watchmm
 *************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_DLIST_RSUC: sucessfull return
 *     BKC_DLIST_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_DLIST_SPF_RELEASE is not
 *     defined in bkc_dlist_spf.h) this func will always show empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_dlist_watchmm(void);
#endif

