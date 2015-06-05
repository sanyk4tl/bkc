/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc
 *  file name: bkc_test.c
 *  description:  facilities for testing the bkc mod
 *************************************************************************
 *  log:
 *  >>>
 *  version: 
 *      v0.0.1
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
#include "bkc.h"
/*********************definition, types & constants ***********************/

/*********************variables******************************************/

/*********************prototype of local functions***********************/

/*********************implementation of open and local functions*********/
static char *file_name_p = __FILE__;

int main(int argc, char*argv[])
{
    printf("file_name_p is %s\n", file_name_p);
    printf("ok, this is the main test\n");
    return 0;
}

 
