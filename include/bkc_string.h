/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_string
 *  file name: bkc_string.h
 *  description: the basic string facility supplied in bkc
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
#ifndef BKC_STRING_H
#define BKC_STRING_H

/*********************included files*************************************/

/*********************definition, types & constants ***********************/
/*ret value of funcs in bkc_dlist*/
#define BKC_STRING_RET_SUC  (0)
#define BKC_STRING_RET_ERR  (-1)

/*types forward declaration in bkc_string module*/
typedef struct bkc_string_tag_t bkc_string_t;

/*trim mode of a string*/
typedef enum bkc_string_trimmode_e {
    /*trim the left spaces in the string*/
    BKC_STRING_TRIMLSPACE,
    /*trim the right spaces in the string*/
    BKC_STRING_TRIMRSPACE,
    /*trim the left and right spaces in the string*/
    BKC_STRING_TRIMLRSPACE,
    /*trim the continuous spaces in the string*/
    BKC_STRING_TRIMCSPACE,
    /*trim the string with length*/
    BKC_STRING_TRIMLEN
} bkc_string_trimmode_m;

/*cpy mode of a string
  used in bkc_string_cpyspec() */
typedef enum bkc_string_cpy_mode_e {
    BKC_STRING_INSERT,
    BKC_STRING_OVERRIDE,
    /*override with the ending-segment style*/
    BKC_STRING_OVERRIDE_ESEG
} bkc_string_cpy_mode_m;

/*the malloc function for bkc_string_gettext()*/
typedef void *(*bkc_string_gtextmfunc_t)(unsigned int size);

/*********************prototype of open functions************************/
/*************************************************************************
 *                  bkc_string_create
 *************************************************************************
 * parameters:
 *     content_p:
 *       points to the initial text in ascii chars for the content
 *       of the string
 * return value:
 *     Not NULL: points to the sucessfully created string
 *     NULL: abnormal return
 * description:
 *     create a string with the text content designated by content_p
 *     content_p can be null if need not the initial text, and
 *     the lenth of the created string will be 0
 *****************************  notes  *****************************
 ************************************************************************/
bkc_string_t *bkc_string_create(char *content_p);

/*************************************************************************
 *                  bkc_string_destroy
 *************************************************************************
 * parameters:
 *     str_p:
 *       points to the string that need to be destroyed
 * return value:
 *     BKC_STRING_RET_SUC: successfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     destroy the string which created by bkc_string_create
 *
 *     str_p can not be NULL. if str_p == NULL, return value will be
 *     BKC_STRING_RET_ERR, and an error msg will be prompted
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_destroy(bkc_string_t *str_p);

/*************************************************************************
 *                  bkc_string_appdchar
 *************************************************************************
 * parameters:
 *     dstr_p:
 *       points to destination src which need to be concated by ch
 *     ch:
 *       the single character in ASCII which will be appended to dstr_p
 * return value:
 *     BKC_STRING_RET_SUC: successfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     append the ch to the tail of the text content of dstr_p
 *
 *     str_p can not be NULL. if str_p == NULL, return value will be
 *     BKC_STRING_RET_ERR, and an error msg will be prompted
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_appdchar(bkc_string_t *dstr_p, char ch);

/*************************************************************************
 *                  bkc_string_insnchar
 *************************************************************************
 * parameters:
 *     dstr_p:
 *       points to string created by bkc_string_create
 *     pos:
 *       the position of the char which will be inserted to
 * return value:
 *     BKC_STRING_RET_SUC: successfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     insert ch to the position of pos in the str_p
 *
 *     str_p can not be NULL and pos can not large than the
 *     bkc_string_getlen(str_p) -1, or lese, return value will be
 *     BKC_STRING_RET_ERR, and an error msg will be prompted
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_insnchar(bkc_string_t *str_p, int pos, char ch);

/*************************************************************************
 *                  bkc_string_getnchar
 *************************************************************************
 * parameters:
 *     dstr_p:
 *       points to string created by bkc_string_create
 *     pos:
 *       the position of the char which will be returned
 * return value:
 *     the n(th) char in the str_p
 *     (less than 0): erroneous return
 * description:
 *     get the nth char in the str_p, if the return value is
 *     less than 0, mean an error occurred, and an error msg
 *     will be prompted
 *
 *     str_p can not be NULL. if str_p == NULL, return value will be
 *     BKC_STRING_RET_ERR, and an error msg will be prompted
 *****************************  notes  *****************************
 ************************************************************************/
char bkc_string_getnchar(bkc_string_t *dstr_p, int pos);

/*************************************************************************
 *                  bkc_string_appdint
 *************************************************************************
 * parameters:
 *     dstr_p:
 *       points to destination src which need to be appended by num
 *     num:
 *       the number which will be translated to a text
 *       and then be appended to dstr_p
 * return value:
 *     BKC_STRING_RET_SUC: successfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     append the num to the tail of dstr_p as a string
 *
 *     dstr_p can not be NULL. if str_p == NULL, return value will be
 *     BKC_STRING_RET_ERR, and an error msg will be prompted
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_appdint(bkc_string_t *dstr_p, int num);

/*************************************************************************
 *                  bkc_string_appd
 *************************************************************************
 * parameters:
 *     dstr_p:
 *       points to destination src which need to be appended by
 *       sstr_p, stext_p
 *     sstr_p:
 *       points to the src string which will be concated to dstr_p
 *     stext_p:
 *       points to the src text which will be concated to dstr_p
 *     ch:
 *       the character which will be concated to dstr_p
 * return value:
 *     BKC_STRING_RET_SUC: successfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     sstr_p, stext_p is recommended to be set to NULL, and ch to -1,
 *     if any of them are not needed.
 *     the result order in dstr_p will be:
 *     original text in dstr_p ,text in sstr_p, text in stext_p and ch
 *
 *     str_p can not be NULL. if str_p == NULL, return value will be
 *     BKC_STRING_RET_ERR, and an error msg will be prompted
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_appd(bkc_string_t *dstr_p, bkc_string_t *sstr_p,
        char *stext_p, char ch);

/*************************************************************************
 *                  bkc_string_trim
 *************************************************************************
 * parameters:
 *     str_p:
 *         points to the string which need to be truncated
 *     newlen:
 *         the designated length of the text content
 *         BKC_STRING_TRIMLSPACE:
 *           trim the left spaces in the string
 *         BKC_STRING_TRIMRSPACE:
 *           trim the right spaces in the string
 *         BKC_STRING_TRIMLRSPACE:
 *           trim the left and right spaces in the string
 *         BKC_STRING_TRIMCSPACE:
 *           trim the continuous spaces in the string, and
 *           all the continuous spaces will be replaced by
 *           a single space
 *         BKC_STRING_TRIMLEN:
 *           trim the string to be the length of newlen
 * return value:
 *     BKC_STRING_RET_SUC: sucessfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     trim with mode == BKC_STRING_TRIMLEN,
 *     have two impact:
 *     if newlen is large than the lenth of the content text in the
 *     string, then nothing  will be applied to the string.
 *     in BKC_STRING_TRIMLSPACE, BKC_STRING_TRIMRSPACE,
 *     BKC_STRING_TRIMLRSPACE and BKC_STRING_TRIMCSPACE mode,
 *     the newlen is not used
 *
 *     str_p can not be NULL. if str_p == NULL, return value is
 *     BKC_STRING_RET_ERR, and an error msg will be prompted
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_trim(bkc_string_t *str_p, bkc_string_trimmode_m mode,
        unsigned int newlen);

/*************************************************************************
 *                  bkc_string_dup
 *************************************************************************
 * parameters:
 *     str_p:
 *         points to the string which will be duplicated
 * return value:
 *     Not NULL: points to the sucessfully duplicated string
 *     NULL: abnormal return
 * description:
 *     create a new string of which the conten is  the same
 *     as str_p.
 *     str_p can not be NULL. if str_p == NULL, return value
 *     will be NULL and will get an error msg.
 *     its a convenient way of calling bkc_string_dupspec(str_p, 0, -1)
 *****************************  notes  *****************************
 ************************************************************************/
bkc_string_t *bkc_string_dup(bkc_string_t *str_p);

/*************************************************************************
 *                  bkc_string_dupspec
 *************************************************************************
 * parameters:
 *     str_p:
 *         points to the string which will be duplicated
 *     spos:
 *         the start position in str_p for the string that will
 *         be duplicated and returned
 *     len_p:
 *         this is a value-result argument first be inputed as the the len
 *         of the string which will be duplicated in sstr_p, and will be
 *         finally stuffed with length thar really been duplicated
 *
 *         if first initialed with -1, then the real length covers the
 *         range of [spos, bkc_string_getlen(str_p)-1]
 * return value:
 *     Not NULL: points to the sucessfully duplicated string
 *     NULL: abnormal return
 * description:
 *     create a new string of which the conten designated by
 *     the start points of spos refered to the content in str_p
 *     and  the length of len.
 *     str_p can not be NULL. if str_p == NULL, return value
 *     will be NULL and will get an error msg
 *****************************  notes  *****************************
 ************************************************************************/
bkc_string_t *bkc_string_dupspec(bkc_string_t *str_p, int spos, int *len_p);

/*************************************************************************
 *                  bkc_string_speccpy
 *************************************************************************
 * parameters:
 *     dstr_p:
 *         points to the string which sstr_p will be copied to
 *     dspos:
 *         the start position in dstr_p for the string that will be stuffed
 *         to dstr_p from sstr_p
 *     sstr_p:
 *         points to the string of which the text content needed
 *         to be copied to dstr_p
 *     sspos:
 *         start position of the string which will be copied in sstr_p
 *     len_p:
 *         this is a value-result argument first be inputed as the the len
 *         of the string which will be copied in sstr_p, and will be
 *         finally stuffed with length thar really been copied
 *
 *         if first initialed with -1, then the real length covers the
 *         range of [sspos, bkc_string_getlen(sstr_p)-1]
 *     mode:
 *         BKC_STRING_INSERT:
 *           the original content in dstr_p from dpos will be right-moved
 *           in sequence
 *         BKC_STRING_OVERRIDE:
 *           the original content in dstr_p from dpos will be overrided
 *           in sequence
 *         BKC_STRING_OVERRIDE_ESEG:
 *           same as BKC_STRING_OVERRIDE, and a more operation:
 *           the designated source string will be copied to the destination
 *           as the last parts of the destination , which means that
 *           the destination will be ended by the new coming part
 * return value:
 *     BKC_STRING_RET_SUC: sucessfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     neither dstr_p nor sstr_p can be NULL. if either is NULL,
 *     return value is BKC_STRING_RET_ERR, and an error msg
 *     will be prompted
 *
 *     if the dstr_p is shorter than the sstr_p with the designated
 *     length, dstr_p will expanded automatically.
 *
 *     if dpos is large than bkc_string_getlen(dstr_p)-1 or spos
 *     is large than bkc_string_getlen(sstr_p)-1, return value is
 *     BKC_STRING_RET_ERR, and an error msg will be prompted
 *
 *     if the length from len_p is too larger than the available
 *     storage coverage in sstr_p with the range of [spos, spos + (*len_p)),
 *     return value is BKC_STRING_RET_ERR, and an error msg will be prompted
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_cpyspec(bkc_string_t *dstr_p, int dpos, bkc_string_t *sstr_p,
        int spos, int *len_p, bkc_string_cpy_mode_m mode);

/*************************************************************************
 *                  bkc_string_chg
 *************************************************************************
 * parameters:
 * return value:
 *     BKC_STRING_RET_SUC: successfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     change the content text of the str_p, if want to clear
 *     the content of the str_p , txt_p can be set to NULL
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_chg(bkc_string_t *str_p, char *txt_p);

/*************************************************************************
 *                  bkc_string_cpy
 *************************************************************************
 * parameters:
 *     dstr_p:
 *         points to the string which sstr_p will be copied to
 *     sstr_p:
 *         points to the string of which the text content need
 *         to be copied to dstr_p
 * return value:
 *     BKC_STRING_RET_SUC: sucessfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     neither dstr_p nor sstr_p can be NULL. if either is NULL,
 *     return value is BKC_STRING_RET_ERR, and an error msg
 *     will be prompted.
 *     its a convenient way of calling
 *     bkc_string_cpyspec(dstr_p, 0, sstr_p, 0, -1, BKC_STRING_OVERRIDE)
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_cpy(bkc_string_t *dstr_p, bkc_string_t *sstr_p);

/*************************************************************************
 *                  bkc_string_gettext
 *************************************************************************
 * parameters:
 *     str_p:
 *         points to the string which need to get its text content
 *     mfunc:
 *         the malloc func the user pass in for creating the return text
 *     rstr_pp
 *         *rstr_pp will points to the text content of the string
 * return value:
 *     BKC_STRING_RET_SUC: sucessfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     get text content of the str_p through rstr_pp
 *
 *     str_p can not be NULL. if str_p == NULL, return value will be
 *     BKC_STRING_RET_ERR, and an error msg will be prompted
 *
 *     the string returned is malloced by mfunc, and the caller
 *     could decide how to free the returned text
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_gettext(bkc_string_t *str_p, char **rstr_pp,
        bkc_string_gtextmfunc_t mfunc);

/*************************************************************************
 *                  bkc_string_cmp
 *************************************************************************
 * parameters:
 *     dstr_p:
 *         points to the destination string which will be compared
 *     sstr_p:
 *         points to the srource string which will be compared
 * return value:
 *     0: the text content of dstr_p is the same as that of sstr_p
 *     (other): the text content of dstr_p is not as the same as sstr_p
 * description:
 *     judge if the text content of the two string are the same
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_cmp(bkc_string_t *dstr_p, bkc_string_t *sstr_p);

/*************************************************************************
 *                  bkc_string_cmpspec
 *************************************************************************
 * parameters:
 *     dstr_p:
 *         points to the destination string which will be compared
 *     dspos:
 *         start position of the destination string
 *     dlen:
 *         len of the destination string which will be compared
 *     sstr_p:
 *         points to the source string which will be compared
 *     sspos:
 *         start position of the source string
 *     slen:
 *         len of the source string which will be compared
 * return value:
 *     0: the text content of dstr_p is the same as that of sstr_p
 *     (other): the text content of dstr_p is not as the same as sstr_p
 * description:
 *     judge if the text content of the two string are the same
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_cmpspec(bkc_string_t *dstr_p, int dspos , int dlen,
        bkc_string_t *sstr_p, int sspos, int slen);

/*************************************************************************
 *                  bkc_string_isempty
 *************************************************************************
 * parameters:
 *     dstr_p:
 *         points to the destination string which will be compared
 *     sstr_p:
 *         points to the srource string which will be compared
 * return value:
 *     1: the text content of str_p is empty or str_p is NULL
 *     0: the text content of str_p is not NULL
 *     (other): abnormal return
 * description:
 *     judge if the str_p is empty.
 *     if str_p is NULL or the text content of str_p is empty then
 *     the string is emtpy
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_isempty(bkc_string_t *str_p);

/*************************************************************************
 *                  bkc_string_getlen
 *************************************************************************
 * parameters:
 *     str_p:
 *       points to the string which need to get its lenth
 * return value:
 *     (large than or equal 0):the length of the string count in
 *          characters(ascii)
 *     (other): abnormal return
 * description:
 *     get the string length
 *
 *     str_p can not be NULL. if str_p == NULL, return value is not
 *     defined, and  an error msg will be prompted
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_getlen(bkc_string_t *str_p);

/*************************************************************************
 *                  bkc_string_prtinfo
 *************************************************************************
 * parameters:
 *     str_p: points to the string
 * return value:
 *     BKC_STRING_RET_SUC: successfull return
 *     BKC_STRING_RET_ERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func can show the detailed infomation about this string
 *
 *     its usefull when take a white-box testing in this
 *     module for showing string info. in this case this func can be
 *     temporarily export to bkc_string.h for outer refering
 *
 *     str_p can be NULL, if str_p == NULL, nothing will be printed
 *****************************  notes  *****************************
 ************************************************************************/
int bkc_string_prtinfo(bkc_string_t *str_p);

#endif
