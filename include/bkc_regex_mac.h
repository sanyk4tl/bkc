/**************************************************************************
 *  copyleft(c)
 **************************************************************************
 *  module: bkc_regex
 *  file name: bkc_regex_mac.h
 *  description: the regex search machine component for bkc_regex module
 **************************************************************************
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
 *************************************************************************/
#ifndef BKC_REGEX_MAC_H
#define BKC_REGEX_MAC_H

/*********************included files**************************************/
#include "bkc_regex_def.h"
#include "bkc_regex_spat.h"
#include "bkc_regex_igs.h"

/*********************definition, types & constants **********************/
/*the set of cmds for the searching machine*/
typedef enum bkc_regex_mac_cmd_set_e {
    BKC_REGEX_MAC_CMD_EMPTY,
    BKC_REGEX_MAC_CMD_CHGSPAT,
    BKC_REGEX_MAC_CMD_CHGIGS,
    BKC_REGEX_MAC_CMD_STARTSCH,
    /*the limit of this set*/
    BKC_REGEX_MAC_CMD_LIMIT
} bkc_regex_mac_cmd_set_m;

/*types forward declaration*/

typedef struct bkc_regex_mac_tag_t  bkc_regex_mac_t;

/*types definition*/

/*search machine for regex searching*/
struct bkc_regex_mac_tag_t {
    bkc_regex_spat_t *spat_p;
    bkc_regex_igs_t *igs_p;
};

/*********************prototype of open functions*************************/
/**************************************************************************
 *                  bkc_regex_mac_create
 **************************************************************************
 * parameters:
 *     spat_p:
 *       points to the search pattern for this search-machine
 *     igs_p:
 *       points to the ignition string for this search-machine
 * return value:
 *     Not NULL: points to the sucessfully created search-machine
 *     NULL: abnormal return
 * description:
 *     create a searching machine with spat_p and igs_p
 *
 *     spat_p and igs_p can be NULL, if either is NULL, a warning msg
 *     will be prompted, and can be added to this machine later by calling
 *     bkc_regex_mac_cmd(mac, spat_p/igs_p, BKC_REGEX_MAC_CMD_CHGSPAT/
 *     BKC_REGEX_MAC_CMD_CHGIGS).
 ***************************** notes ***********************************
 *************************************************************************/
bkc_regex_mac_t *bkc_regex_mac_create(bkc_regex_spat_t *spat_p, char *igs_p);

/**************************************************************************
 *                  bkc_regex_mac_destroy
 **************************************************************************
 * parameters:
 *     mac_p:
 *       points to the search-machine that need to be destroyed
 * return value:
 *     BKC_REGEX_RSUC: successfull return
 *     BKC_REGEX_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     destroy the serach-machine which created by bkc_regex_mac_create
 *
 *     mac_p can not be NULL. if mac_p == NULL, return value will be
 *     BKC_REGEX_RERR, and a error msg will be prompted
 *
 *     destroy will not release the spat_p and igs_p attached to this
 *     machine when created.
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_mac_destroy(bkc_regex_mac_t *mac_p);

/**************************************************************************
 *                  bkc_regex_mac_cmd
 **************************************************************************
 * parameters:
 *     mac_p:
 *       points to the search-mac created by bkc_regex_mac_create
 *     cmd:
 *       BKC_REGEX_MAC_CMD_CHGSPAT
 *         change the search-pattern of this machine, if original
 *         search-pattern is NULL, this will add as an new one
 *       BKC_REGEX_MAC_CMD_CHGIGS
 *         change the ignition-string of this machine, if original
 *         ignition-string is NULL, this will add as an new one
 *       BKC_REGEX_MAC_CMD_STARTSCH
 *         start the machine, then it will begin searching.
 *         a searching will automatically stop when searching
 *         finish or abnormality occurs
 *     args_p:
 *       points to the arguments passed in for each cmd:
 *       BKC_REGEX_MAC_CMD_CHGSPAT:
 *         points to the search-pattern
 *       BKC_REGEX_MAC_CMD_CHGIGS
 *         points to the ignition-string
 *       BKC_REGEX_MAC_CMD_STARTSCH
 *         must be NULL, other value is undefined
 * return value:
 *     BKC_REGEX_RSUC: successfull return
 *     BKC_REGEX_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     pass a cmd to the search-machine
 *
 *     if want to know the search-pattern is matched or not when
 *     cmd == BKC_REGEX_MAC_CMD_STARTSCH and ret == BKC_REGEX_RSUC
 *     bkc_regex_mac_gmstatus(mac_p). if ret == BKC_REGEX_RMATCHED,
 *     and want to get the matched string further, bkc_regex_spat_gmret()
 *     can be called under that circumstance
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_mac_cmd(bkc_regex_mac_t *mac_p, bkc_regex_mac_cmd_set_m cmd,
        void *args_p);

/**************************************************************************
 *                  bkc_regex_mac_gmstatus
 **************************************************************************
 * parameters:
 *     mac_p:
 *       points to the search-machine that need to be destroyed
 * return value:
 *     BKC_REGEX_RMATCHED:
 *       search-pattern has been matched in the mac_p.
 *     BKC_REGEX_RNOMATCHED:
 *       search-pattern has not been matched in the mac_p.
 *     (other): abnormal return
 * description:
 *     get the match-status of current searching in mac_p.
 *     this func can be called after bkc_regex_mac_cmd(mac_p,
 *     BKC_REGEX_MAC_CMD_STARTSCH, NULL) to get the match-status
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_mac_gmstatus(bkc_regex_mac_t *mac_p);

#endif


