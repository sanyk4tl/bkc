/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_string
 *  file name: bkc_string_tspf.h
 *  description: supporting utilites for testing the bkc_string module
 *************************************************************************
 *  log:
 *  >>>
 *  version: 
 *      v1.0.1-v1.0.0
 *  time:
 *      Thursday, April 16, 2009 (090416) at 12:45:07 AM UTC/GMT +8 hours
 *  location:
 *      ChongQing, China on Earth
 *      latitude: 29.3 North, longtitude: 106.35 East 
 *      international country code: + 86 (China) 
 *      area code: 23
 *  weather:
 *      clear, mild
 *
 *  pgr_id:sizhe(email:tancng#gmail.com)
 *
 *  description: created
 ************************************************************************/
#ifndef BKC_STRING_TSPF_H
#define BKC_STRING_TSPF_H
/*********************included files*************************************/

/*********************definition, types & constants ***********************/
/*for memory diagnose*/
typedef void *(*bkc_string_tspf_malloc_func_t)(unsigned int size);
typedef	void (*bkc_string_tspf_free_func_t)(void *mem_p);

/*********************prototype of open functions************************/

/*************************************************************************
 *                  bkc_string_tspf_memdgn
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     called to diagnose the current memory status, this usefull
 *     in white-box testing
 *     if the memory is steady after calling this func or calling this
 *     func repeatly, we can make sure that there isn't 
 *     any fleeing memroy.
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_string_tspf_memdgn(bkc_string_tspf_malloc_func_t malloc_func, 
        bkc_string_tspf_free_func_t free_func);
#endif

