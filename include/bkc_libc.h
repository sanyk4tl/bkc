/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc_libc
 *  file name: bkc_libc.h
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
#ifndef BKC_LIBC_H
#define BKC_LIBC_H

/*********************included files*************************************/

/*********************definition, types & constants ***********************/
/*ret value of funcs in bkc_libc*/
#define BKC_LIBC_RSUC    (0)
#define BKC_LIBC_RERR    (-1)
#define BKC_LIBC_REMPTY  (-2)

/*types forward declaration in bkc_libc module*/

/*********************prototype of open functions************************/
/*************************************************************************
 *                  bkc_libc_strncpy
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     this func is a wrapper of the standard libc
 *
 *     copies the string pointed to by src, including
 *     the terminating null byte ('\0'), to the buffer  pointed  to  by  dest.
 *     The  strings  may  not overlap, and the destination string dest must be
 *     large enough to receive the copy
 *
 *     note:If there is no null byte among the first n bytes
 *     of src, the string placed in dest will not be null terminated
 *
 *     this function return a pointer to the destination string dest
 *
 *     #include <string.h> is the original standard include-directive
 *****************************  Notes  *****************************
 ************************************************************************/
char *bkc_libc_strncpy(char *dest, const char *src, size_t n);

/*************************************************************************
 *                  bkc_libc_strncmp
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     this func is a wrapper of the standard libc
 *
 *     this function return an integer less than, equal
 *     to, or greater than zero if s1 (or the first n bytes thereof) is found,
 *     respectively, to be less than, to match, or be greater than s2
 *
 *     #include <string.h> is the original standard include-directive
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_libc_strncmp(const char *s1, const char *s2, size_t n);

/*************************************************************************
 *                  bkc_libc_strndup
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     this func is a wrapper of the standard libc
 *
 *     this func returns  a  pointer to a new string which is a
 *     duplicate of the string s.  Memory for the new string is obtained  with
 *     bkc_libc_malloc, and can be freed with bkc_libc_free.
 *
 *     it only copies at most n characters.  If s is longer than n,
 *     only n characters are copied, and a  terminating null
 *     byte ('\0') is added.
 *
 *     note: returned string is malloced by bkc_libc_malloc, and
 *     can always be used for bkc_libc_strlen(), because it is
 *     guaranteed to have a null byte('\0') at the end
 *
 *     #include <string.h> is the original standard include-directive
 *****************************  Notes  *****************************
 ************************************************************************/
char *bkc_libc_strndup(const char *s, size_t n);

/*************************************************************************
 *                  bkc_libc_strlen
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     this func is a wrapper of the standard libc
 *
 *     this func calculates  the  length  of  the string s, not
 *     including the terminating '\0' character. and returns
 *     the number of characters in s.
 *
 *     note:the s must be terminated by the null byte('\0').
 *     and if s is NULL, the returned count will be zero(0)
 *
 *     #include <string.h> is the original standard include-directive
 *****************************  Notes  *****************************
 ************************************************************************/
size_t bkc_libc_strlen(const char *s);

/*************************************************************************
 *                  bkc_libc_strncat
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     this func is a wrapper of the standard libc
 *
 *     this func appends the src string to the dest string, overwriting
 *     the null byte ('\0') at the end of dest, and then adds a terminating
 *     null  byte.   The  strings may not overlap, and the dest string
 *     must have enough space for the result
 *
 *     If src contains n or more characters, it will write  n+1  characters
 *     to  dest  (n  from src plus the terminating null byte).
 *     Therefore, the size of dest must be at least
 *     bkc_libc_strlen(dest)+n+1
 *
 *     this function will return a pointer to the  resulting
 *     string dest
 *
 *     #include <string.h> is the original standard include-directive
 *****************************  Notes  *****************************
 ************************************************************************/
char *bkc_libc_strncat(char *dest, const char *src, size_t n);

/*************************************************************************
 *                  bkc_libc_memset
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     this func is a wrapper of the standard libc
 *
 *     this func returns a pointer to the memory area s.
 *
 *     #include <string.h> is the original standard include-directive
 *****************************  Notes  *****************************
 ************************************************************************/
void *bkc_libc_memset(void *s, int c, size_t n);

/*************************************************************************
 *                  bkc_libc_memcpy
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     this func is a wrapper of the standard libc
 *
 *     this func returns a pointer to dest
 *
 *     #include <string.h> is the original standard include-directive
 *****************************  Notes  *****************************
 ************************************************************************/
void *bkc_libc_memcpy(void *dest, const void *src, size_t n);

/*************************************************************************
 *                  bkc_libc_printf
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     this func is a wrapper of the standard libc
 *
 *     upon successful return, this func returns the number of characters
 *     printed  (not  including  the  trailing  '\0'  used  to  end  output to
 *     strings)
 *
 *     #include <stdio.h> is the original standard include-directive
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_libc_printf(const char *format, ...);

/*************************************************************************
 *                  bkc_libc_snprintf
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     this func is a wrapper of the standard libc
 *
 *     this func writes  at most size bytes (including the trailing null
 *     byte ('\0') to str.
 *
 *     and it do not write  more  than  size bytes  (including
 *     the trailing '\0').  If the output was truncated due
 *     to this limit then the return value is the number  of  characters  (not
 *     including the trailing '\0') which would have been written to the final
 *     string
 *
 *     if an output error is encountered, a negative value is returned
 *
 *     #include <stdio.h> is the original standard include-directive
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_libc_snprintf(char *str, size_t size, const char *format, ...);

/*************************************************************************
 *                  bkc_libc_malloc
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     this func is a wrapper of the standard libc
 *
 *     this func allocates  size  bytes and returns a pointer to the allocated
 *     memory.  the memory is not  cleared.   if  size  is  0,  then  it
 *     returns  either  NULL, or a unique pointer value that can later be
 *     successfully passed to bkc_libc_free()
 *
 *     #include <stdlib.h> is the original standard include-directive
 *****************************  Notes  *****************************
 ************************************************************************/
void *bkc_libc_malloc(size_t size);

/*************************************************************************
 *                  bkc_libc_free
 *************************************************************************
 * parameters:
 * return value:
 * description:
 *     this func is a wrapper of the standard libc
 *
 *     this func frees the memory space pointed to by ptr, which
 *     must  have  been returned by a previous call to bkc_libc_malloc().
 *     if bkc_libc_free(ptr) has already been called before, undefined
 *     behavior occurs.  if ptr is NULL, no operation is performed.
 *
 *     #include <stdlib.h> is the original standard include-directive
 *****************************  Notes  *****************************
 ************************************************************************/
void bkc_libc_free(void *ptr);

/*************************************************************************
 *                  bkc_libc_watchmm
 *************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_LIBC_RSUC: sucessfull return
 *     BKC_LIBC_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_LIBC_SPF_RELEASE is not
 *     defined in bkc_libc_spf.h) this func will always show empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 *****************************  Notes  *****************************
 ************************************************************************/
int bkc_libc_watchmm(void);
#endif

