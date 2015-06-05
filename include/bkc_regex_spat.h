/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_regex
 *  file name: bkc_regex_spat.h
 *  description: the search pattern component for bkc_regex module
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
#ifndef BKC_REGEX_SPAT_H
#define BKC_REGEX_SPAT_H

/*********************included files*************************************/
#include "bkc_regex_def.h"
#include "bkc_string.h"
#include "bkc_dlist.h"

/*********************definition, types & constants ***********************/
/*the set of meta-word mean-unit*/
typedef enum bkc_regex_spat_metawd_set_e {
    /*the "." symbol*/
    BKC_REGEX_SPAT_METAWD_DOT
} bkc_regex_spat_metawd_set_m;

/*the set of multi-word mean-unit*/
typedef enum bkc_regex_spat_multiwd_set_e {
    /*the alpha and number symbol,that is,
      "a-z", "A-Z", "0-9"*/
    BKC_REGEX_SPAT_MULTIWD_SET_AN,
    /*the alpha symbol,that is,
      "a-z", "A-Z"*/
    BKC_REGEX_SPAT_MULTIWD_SET_ALPHA,
    /*the num symbol,that is,
      "0-9"*/
    BKC_REGEX_SPAT_MULTIWD_SET_NUM
} bkc_regex_spat_multiwd_set_m;

/*the mean-unit type*/
typedef enum bkc_regex_spat_mutype_e {
    BKC_REGEX_SPAT_MUTYPE_PLAIN_TEXT,
    BKC_REGEX_SPAT_MUTYPE_META_WORD,
    BKC_REGEX_SPAT_MUTYPE_MULTI_WORD
} bkc_regex_spat_mutype_m;

/*types forward declaration in bkc_regex_spat module*/

typedef struct bkc_regex_spat_tag_t  bkc_regex_spat_t;
typedef struct bkc_regex_spat_mgroup_tag_t  bkc_regex_spat_mgroup_t;
typedef struct bkc_regex_spat_munit_tag_t  bkc_regex_spat_munit_t;
/*munit content struct*/
typedef struct bkc_regex_spat_plain_text_tag_t  bkc_regex_spat_plain_text_t;
typedef struct bkc_regex_spat_meta_word_tag_t  bkc_regex_spat_meta_word_t;
typedef struct bkc_regex_spat_multi_word_tag_t  bkc_regex_spat_multi_word_t;

/*types definition*/

/*the content struct of plain-text munit*/
struct bkc_regex_spat_plain_text_tag_t {
    /*the plain text which need to be matched*/
    bkc_string_t *text_p;
    /*the next position for matching in text_p */
    unsigned int pos;
};

/*the content struct of meta-word munit*/
struct bkc_regex_spat_meta_word_tag_t {
    /*the index of this meta mean-unit*/
    bkc_regex_spat_metawd_set_m name_index;
    /*the specific char set of the meta word*/
    bkc_string_t *set_p;
};

/*the content struct of multi-word munit*/
struct bkc_regex_spat_multi_word_tag_t {
    /*the index of this multi mean-unit*/
    bkc_regex_spat_multiwd_set_m name_index;
};

/*mean-group postfix connect type*/
typedef enum bkc_regex_spat_mgpctype_e{
    BKC_REGEX_SPAT_MGPCTYPE_EMPTY,
    BKC_REGEX_SPAT_MGPCTYPE_AND,
    BKC_REGEX_SPAT_MGPCTYPE_OR
} bkc_regex_spat_mgpctype_m;

/*mean-group postfix decoration type*/
typedef enum bkc_regex_spat_mgpdtype_e{
    BKC_REGEX_SPAT_MGPDTYPE_EMPTY,
    BKC_REGEX_SPAT_MGPDTYPE_ASTERISK,
    BKC_REGEX_SPAT_MGPDTYPE_PLUS
} bkc_regex_spat_mgpdtype_m;

/*mean-group subsidiary set type*/
typedef enum bkc_regex_spat_mgsdtype_e{
    BKC_REGEX_SPAT_MGSDTYPE_EMPTY,
    BKC_REGEX_SPAT_MGSDTYPE_AND,
    BKC_REGEX_SPAT_MGSDTYPE_OR
} bkc_regex_spat_mgsdtype_m;

/*mean-group node type*/
typedef enum bkc_regex_spat_mgntype_e{
    BKC_REGEX_SPAT_MGNTYPE_EMPTY,
    /*single node*/
    BKC_REGEX_SPAT_MGNTYPE_NODE,
    /*set which means that this mean-group node
      describe a set of mean-group nodes, but not
      just a single node*/
    BKC_REGEX_SPAT_MGNTYPE_SET
} bkc_regex_spat_mgntype_m;

/*mean unit used in regex search*/
struct bkc_regex_spat_munit_tag_t {
    bkc_regex_spat_mutype_m unit_type;
    union {
        bkc_regex_spat_plain_text_t plain_text;
        bkc_regex_spat_meta_word_t  meta_word;
        bkc_regex_spat_multi_word_t multi_word;
    } munit_content;
};

/*mean group used in regex search*/
struct bkc_regex_spat_mgroup_tag_t {
    /*the index which the user designated,
      valid value is [0, +INT_MAX)*/
    int index;
    /*the type of this mean-group node*/
    bkc_regex_spat_mgntype_m node_type;
    /*mean unit for this mean-group*/
    bkc_regex_spat_munit_t munit;

    /*postfix connect type*/
    bkc_regex_spat_mgpctype_m conn_type;
    /*postfix decoration type*/
    bkc_regex_spat_mgpdtype_m deco_type;

    /*subsidiary mean-groups type for this mgroup*/
    bkc_regex_spat_mgsdtype_m sub_type;
    /*subsidiary mean-groups for this mgroup,
      the type of each node will be
      bkc_regex_spat_mgroup_t*/
    bkc_dlist_t *sub_groups_list_p;

    /*next neighbor mgroup for this mgroup*/
    bkc_dlist_t *next_group_p;

    /*result matched string list, type of each node
      will be bkc_string_t*/
    bkc_dlist_t *str_list_p;
};

/*search pattern for regex searching*/
#if 0
/*the perfect evolving version*/
struct bkc_regex_spat_tag_t {
    /*the current mean_group which is being matched*/
    unsigned int cur_pos;

    /*the mean-group number in this search pattern*/
    unsigned int mgroup_num;
    /*mean-group list*/
    bkc_regex_spat_mgroup_t *mgroup_p;

    /*flags about if all mean groups has been matched
      0: not all mean groups been matched yet
      1: all are matched*/
    unsigned int is_agroups_matched;
};
#endif
/*the fast version*/
struct bkc_regex_spat_tag_t {
    /*the current mean_group which is being matched*/
    unsigned int cur_pos;

    /*mean-group list*/

    bkc_string_t *pat_str_p;
    bkc_string_t *mat_ret_p;

    /*flags about if the pattern has been matched
      0: not matched yet
      1:  matched*/
    unsigned int is_pat_matched;
};

/*********************prototype of open functions************************/
/**************************************************************************
 *                  bkc_regex_spat_create
 **************************************************************************
 * parameters:
 *     pat_str_p:
 *       points to the text in ascii for the search pattern
 * return value:
 *     Not NULL: points to the sucessfully created search pattern
 *     NULL: abnormal return
 * description:
 *     create a search pattern with the text content designated by
 *     pat_str_p.
 *     str_p can be NULL, and a warning msg will be prompted
 ***************************** notes ***********************************
 *************************************************************************/
bkc_regex_spat_t *bkc_regex_spat_create(char *pat_str_p);

/**************************************************************************
 *                  bkc_regex_spat_destroy
 **************************************************************************
 * parameters:
 *     spat_p:
 *       points to the search pattern that need to be destroyed
 * return value:
 *     BKC_REGEX_RSUC: successfull return
 *     BKC_REGEX_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     destroy the search pattern which created by bkc_regex_spat_create
 *
 *     spat_p can not be NULL. if spat_p == NULL, return value will be
 *     BKC_REGEX_RERR, and a error msg will be prompted
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_spat_destroy(bkc_regex_spat_t *spat_p);

/**************************************************************************
 *                  bkc_regex_spat_gmret
 **************************************************************************
 * parameters:
 *     spat_p:
 *       points to the search pattern that gives the matched result string
 *     rstr_p:
 *       will be stuffed with the matched result
 * return value:
 *     BKC_REGEX_RSUC: successfull return
 *     BKC_REGEX_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     get the matched result string
 *
 *     spat_p and rstr_p can not be NULL. if either is NULL, return value
 *     will be BKC_REGEX_RERR, and a error msg will be prompted
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_spat_gmret(bkc_regex_spat_t *spat_p, bkc_string_t *rstr_p);

/**************************************************************************
 *                  bkc_regex_spat_definit
 **************************************************************************
 * parameters:
 *     spat_p:
 *       points to the search pattern that gives the matched result string
 * return value:
 *     BKC_REGEX_RSUC: successfull return
 *     BKC_REGEX_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     default init the spat_p, can be used when we want to clear the
 *     spat_p for a new searching for the same search pattern.
 *     it will releasing the matching result and zero the start_pos of
 *     the pattern. it is used in bkc_regex_mac_startsch().
 *
 *     spat_p can not be NULL. if NULL, return value
 *     will be BKC_REGEX_RERR, and a error msg will be prompted
 ***************************** notes ***********************************
 *************************************************************************/
int bkc_regex_spat_definit(bkc_regex_spat_t *spat_p);

#endif


