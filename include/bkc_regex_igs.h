/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_regex
 *  file name: bkc_regex_igs.h
 *  description: the ignition string component in bkc_regex module
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
#ifndef BKC_REGEX_IGS_H
#define BKC_REGEX_IGS_H

/*********************included files*************************************/
#include "bkc_regex_def.h"
#include "bkc_string.h"

/*********************definition, types & constants ***********************/
/*types forward declaration*/

typedef struct bkc_regex_igs_tag_t  bkc_regex_igs_t;

/*types definition*/

/*ignition string for regex searching*/
struct bkc_regex_igs_tag_t {
    /*the ignition string which need to be searched*/
    bkc_string_t *str_p;
    /*start search-position of this string*/
    int start_pos;
    /*current search-postion on this string*/
    int cur_pos;
};

/*********************prototype of open functions************************/
/**************************************************************************
 *                  bkc_regex_igs_create
 **************************************************************************
 * parameters:
 *     igs_p:
 *       points to the text in ascii for the ignition string
 * return value:
 *     Not NULL: points to the sucessfully created ignition string
 *     NULL: abnormal return
 * description:
 *     create a ignition string with the text content designated by igs_p.
 *     a ignition string is the feeding source for a searching process.
 *     str_p can be NULL, and a warning msg will be prompted.
 ***************************** notes ***********************************
 *************************************************************************/
bkc_regex_igs_t *bkc_regex_igs_create(char *str_p);

/**************************************************************************
 *                  bkc_regex_igs_destroy
 **************************************************************************
 * parameters:
 *     igs_p:
 *       points to the ignition string that need to be destroyed
 * return value:
 *     BKC_REGEX_RSUC: successfull return
 *     BKC_REGEX_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     destroy the ignition string which created by bkc_regex_igs_create
 *
 *     igs_p can not be NULL. if igs_p == NULL, return value will be
 *     BKC_REGEX_RERR, and a error msg will be prompted
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_igs_destroy(bkc_regex_igs_t *igs_p);

#endif


