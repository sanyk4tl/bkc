/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_crc
 *  file name: bkc_crc.h
 *  description: the basic string facility supplied in bkc
 *****************************************************************************
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
 ****************************************************************************/
#ifndef BKC_CRC_H
#define BKC_CRC_H

/*********************included files*****************************************/
//#include "bkc_crc_spat.h"

/*********************definition, types & constants *************************/
/*ret value of funcs in bkc_crc*/

#define BKC_CRC_RSUC    (0)
#define BKC_CRC_RERR    (-1)
#define BKC_CRC_CKSUC   (1)
#define BKC_CRC_CKFAIL  (2)

/*types forward declaration in bkc_crc module*/
/*check sum mode*/
typedef enum bkc_crc_csmode_e {
    BKC_CRC_CSMODE_EMPTY,
    BKC_CRC_CSMODE_16,
    BKC_CRC_CSMODE_32,
} bkc_crc_csmode_m;

/*********************prototype of open functions****************************/
/*****************************************************************************
 *                  bkc_crc_gen_cksum
 *****************************************************************************
 * parameters:
 *     buf_p:
 *       points to the data buffer of which the the crc cksum will be
 *       generated from
 *     ret_cksum_p:
 *       points to the buffer which will be used to return
 *       the crc cksum generated from the buf_p
 *     mode:
 *       the type of the crc cksum that will be generated
 *       BKC_CRC_CSMODE_16:
 *         the cksum returned by ret_cksum_p has the type as unsigned
 *         short with 16 bits width.
 *       BKC_CRC_CSMODE_32:
 *         the cksum returned by ret_cksum_p has the type as unsigned
 *         int with 32 bits width.
 * return value:
 *     BKC_CRC_RSUC: sucessfull return
 *     BKC_CRC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to generate the crc cksum from buf_p according
 *     to the mode
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_crc_gen_cksum(void *buf_p, void *ret_cksum_p, bkc_crc_csmode_m mode);

/*****************************************************************************
 *                  bkc_crc_ck
 *****************************************************************************
 * parameters:
 *     buf_p:
 *       points to the data buffer which need to be checked
 *     ret_cksum_p:
 *       points to the buffer in which the ready crc cksum is stored
 *     mode:
 *       the type of the crc cksum supplied in cksum_p
 *       BKC_CRC_CSMODE_16:
 *         the cksum stored in ret_cksum_p has the type as unsigned
 *         short with 16 bits width.
 *       BKC_CRC_CSMODE_32:
 *         the cksum stored in ret_cksum_p has the type as unsigned
 *         int with 32 bits width.
 * return value:
 *     BKC_CRC_RSUC: sucessfull return
 *     BKC_CRC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is used to check the buf_p with the crc cksum stored
 *     in cksum_p according to the mode
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_crc_ck(void *buf_p, void *cksum_p, int mode);

/*****************************************************************************
 *                  bkc_crc_watchmm
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_CRC_RSUC: sucessfull return
 *     BKC_CRC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_CRC_SPF_RELEASE is not
 *     defined in bkc_crc_spf.h) this func will always show
 *     empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 *****************************  notes  ************************************
 ****************************************************************************/
int bkc_crc_watchmm(void);

#endif

