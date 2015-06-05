/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_stack
 *  file name: bkc_stack_test.c
 *  description:  facilities for testing the bkc_stack mod
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
#include "bkc_stack_tspf.h"
#include "bkc_stack.h"
/*********************definition, types & constants ***********************/
/*#define BKC_STACK_T_WATCHM*/
/*********************variables******************************************/

/*********************prototype of local functions***********************/
static void bkc_stack_t_cppd(void);
static int bkc_stack_unode_prt_func(void *unode_p, void *param_p);
static int bkc_stack_unode_destroy_func(void *unode_p, void *param_p);

/*********************implementation of open and local functions*********/
/*************************************************************************
 *                  bkc_stack_unode_prt_func
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_stack_unode_prt_func(void *unode_p, void *param_p)
{
    /*we print nothing*/
    return 0;
}

/*************************************************************************
 *                  bkc_stack_unode_destroy_func
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_stack_unode_destroy_func(void *unode_p, void *param_p)
{
    printf("in the destroy func\n");
    printf("unode_p is %p, param_p is %p\n", unode_p, param_p);
    return 0;
}

/*************************************************************************
 *                  bkc_stack_t_cppd
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     test create, push, pop and destroy
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_stack_t_cppd(void)
{
    bkc_stack_t *stack_p = NULL;
    void *poped_node_p = NULL;
    int num = -1;
    stack_p = bkc_stack_create();

    bkc_stack_push(stack_p, 0xFFFFFFFF);
    bkc_stack_push(stack_p, 0xFFFFFFFE);
    bkc_stack_push(stack_p, 0xFFFFFFFD);
    bkc_stack_push(stack_p, 0xFFFFFFFC);
    bkc_stack_push(stack_p, 0xFFFFFFFB);
    bkc_stack_push(stack_p, 0xFFFFFFFA);
    bkc_stack_print(stack_p, bkc_stack_unode_prt_func, NULL);

#if 0
    bkc_stack_reset(stack_p, bkc_stack_unode_destroy_func, NULL);
    bkc_stack_print(stack_p, bkc_stack_unode_prt_func, NULL);
    getchar();
#endif
    bkc_stack_getnnum(stack_p, &num);
    printf("num is %d\n", num);

    bkc_stack_pop(stack_p, &poped_node_p);
    printf("first pop is %x\n", poped_node_p);
    bkc_stack_print(stack_p, bkc_stack_unode_prt_func, NULL);

    bkc_stack_pop(stack_p, &poped_node_p);
    printf("second pop is %x\n", poped_node_p);
    bkc_stack_print(stack_p, bkc_stack_unode_prt_func, NULL);

    bkc_stack_pop(stack_p, &poped_node_p);
    printf("third pop is %x\n", poped_node_p);
    bkc_stack_print(stack_p, bkc_stack_unode_prt_func, NULL);

    bkc_stack_pop(stack_p, &poped_node_p);
    printf("fourth pop is %x\n", poped_node_p);
    bkc_stack_print(stack_p, bkc_stack_unode_prt_func, NULL);

    bkc_stack_pop(stack_p, &poped_node_p);
    printf("fifth pop is %x\n", poped_node_p);
    bkc_stack_print(stack_p, bkc_stack_unode_prt_func, NULL);

    bkc_stack_pop(stack_p, &poped_node_p);
    printf("sixth pop is %x\n", poped_node_p);
    bkc_stack_print(stack_p, bkc_stack_unode_prt_func, NULL);

    poped_node_p = NULL;
    bkc_stack_pop(stack_p, &poped_node_p);
    printf("seven pop is %x\n", poped_node_p);
    bkc_stack_print(stack_p, bkc_stack_unode_prt_func, NULL);

    bkc_stack_destroy(stack_p, bkc_stack_unode_destroy_func, NULL);

    bkc_stack_watchmm();
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
    bkc_stack_t_cppd();
    return 0;
}
