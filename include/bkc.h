/*************************************************************************
 *  copyleft(c)
 *************************************************************************
 *  module: bkc
 *  file name: bkc.h
 *  description: the main interface of the bkc library.
 *************************************************************************
 *  version:
 *    v1.1.7
 *************************************************************************
 *  overlook:
 *
 *  x1. bkc is short for blade kit of c language.
 *
 *  x2. bkc is a light-weighted platform which provides the most usefull
 *      and commonly used functionalites in the development of c language.
 *      bkc is also the platform on which the excellent ideas can be
 *      easily tried and realized.
 *
 *     bkc lives for two aims:
 *     a. be helpfull with the learning and practising c language for
 *        the buddies who need to use c language.
 *
 *     b. be useful for the development of all
 *        imaginable projects in c language, especially for the
 *        embeded ones.
 *
 *  x3. bkc believes that life can be better if the helpfull things
 *     are spreading fiercely and freely.
 *
 *************************************************************************
 *  license:
 *
 *  bkc conforms to "EMPTY LICENSE".
 *
 *  this means that:
 *  x1. bkc will always be source opened, and can always
 *      be gotten freely via network or other methods when we needed.
 *      'freely' includes without any payment.
 *
 *  x2. every buddy who gets bkc can use it freely at once.
 *      'freely' includes without any payment and in any methods.
 *
 *  x3. commercial use of bkc is welcomed and recommended.
 *
 *      PLEASE NOTE!
 *      there is always a simple rule:
 *      the commercial group must pay the real-value of the bkc
 *      to the participants who participates in the commercial group.
 *
 *      the participants includes employees and employers in the
 *      commercial group.
 *
 *      other detailed information such as the real-value calculation and
 *      corresponding payment methods is detailed in "EMPTY LICENSE"
 *      specification.
 *
 *  "EMPTY LICENSE" specification can be found in the
 *  path of bkc/auxiliary/sandbox/bkc_multicube/EMPTY_LICENSE.txt
 *
 *
 *  the following are the declaration recommended in "EMPTY_LICENSE".
 *
 *  Copyright Declaration
 *
 *  This work is an open resource, and it is distributed in the hope that
 *  it will be helpful and useful.
 *
 *  You can use it freely to your needs under the EMPTY LICENSE.
 *  There is also a clear warranty. Please see the EMPTY LICENSE
 *  for more details.
 *
 *  You should have received a copy of the EMPTY LICENSE along with this
 *  work. If not, see <http://groups.google.de/group/pub-res-fd>
 *
 *************************************************************************
 *  trunk docs:
 *
 *  x1. preface doc:
 *    auxiliary/trunk/bkc_trunkdoc_preface.pdf
 *
 *    this is the big gate of bkc, everything about bkc
 *    can be found from here.
 *    The modifiable version of this doc is
 *    bkc/auxiliary/sandbox/bkc_multicube/bkc_trunkdoc_preface.odt
 *
 *  x2. designment doc:
 *    auxiliary/trunk/bkc_trunkdoc_designment.pdf
 *    this is the designment of bkc.
 *
 *    The modifiable version of this doc is
 *    bkc/auxiliary/sandbox/bkc_multicube/bkc_trunkdoc_designment.odg
 *
 *  x3. modification history doc:
 *    auxiliary/trunk/bkc_trunkdoc_modhistory.txt
 *    this is a history of important modifications for bkc
 *
 *  x4. selfmending doc:
 *    bkc_trunkdoc_selfmending.txt
 *    more aims and prospects of bkc are recorded here.
 *
 *************************************************************************
 *  current module list:
 *
 *  all modules available in bkc are as follows:
 *
 *  bkc_dlist
 *  bkc_libc
 *  bkc_pthread
 *  bkc_queue
 *  bkc_string
 *  bkc_stack
 *  bkc_regex
 *  bkc_pasync
 *  bkc_time
 *  bkc_timer
 *  bkc_socket
 *  bkc_crc
 *  bkc_store
 *
 *  for detailed information, such as functionality, status and maturity
 *  of each module can refer to the
 *  bkc/auxiliary/sandbox/bkc_multicube/bkc_modules_info.txt
 *************************************************************************
 *  how to build and use bkc:
 *
 *  platform dependency:
 *
 *    currently bkc has been built on ubuntu 9.04(linux os)
 *    toolchains requirement:
 *      c99 or c89 compiler such as gcc
 *      make program such as gnu make
 *      shell program such as bash
 *
 *  for detailed information, can refer to the
 *  bkc/auxiliary/sandbox/bkc_multicube/bkc_building_and_using.txt
 *************************************************************************
 *  how to add a module in bkc:
 *
 *  add a module is simple in bkc now with the help of gdg_creat_mod.sh
 *  script.
 *
 *  for detailed information, can refer to the
 *  bkc/auxiliary/sandbox/bkc_multicube/bkc_adding_new_mod.txt
 *************************************************************************
 *  usefull coding helper in bkc:
 *
 *  the file named bkc_<modulename>_spf.h in each module has
 *  supplied try-throw and erring and warning macros for developing
 *  of each module,
 *  i.e. bkc_dlist_spf.h.
 *  there is also a small memory watcher in bkc_<modulename>_spf.h,
 *  which can be used to safeguard the memory used in this module
 *
 *************************************************************************
 *  communication and maintenance info:
 *
 *  the station for bkc is <http://groups.google.de/group/bkc-group>
 *    everything include this gate doc can be found there.
 *
 *  for getting support and reporting bugs
 *    currently can just mail to bkc-maintaining mailbox
 *    buglist information now can be found and traced in the bug tracker.
 *
 *  for suggestion
 *    any suggestions about the growth of bkc is very welcomed.
 *    if you need some functionality that you have no time
 *    to complete or just a good idea or a wonderfull code episode,
 *    you are welcomed to mail to the bkc-maintaining mailbox
 *
 *  for joining in and contributing:
 *    any code segment and ideas in connection with bkc are welcomed.
 *
 *  bkc-maintaining mailbox:
 *    sanyk.tan@gmail.com
 *************************************************************************
 *  small tips:
 *
 *  every one is sincerely welcomed to join in bkc, and this is
 *  a long-term act of passing on hope.
 *************************************************************************
 *  authors:
 *  pgr_id: sizhe(sanyk.tan@gmail.com)
 *
 *************************************************************************
 *  permanent acknowledgement:
 *
 *  from pgr_id:sizhe to this buddies in those memorable days.
 *  detailed list is carefully positioned in
 *  bkc/auxiliary/sandbox/bkc_multicube/bkc_perack_pgrid_sizhe.txt
 *************************************************************************
 *  log:
 *
 *  milestones are also recorded and detailed in
 *  bkc/auxiliary/sandbox/bkc_multicube/bkc_trunkdoc_modhistory.txt
 *
 *  developing logs scripted by the buddies in bkc can be found in
 *  bkc/auxiliary/sandbox/bkc_devdaily/<buddy_id_name>.
 *
 *  >>>
 *  version:
 *      v1.0.0
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
 *  pgr_id: sizhe(email:sanyk.tan#gmail.com)
 *
 *  description: created, add the bkc_dlist module
 *
 *  >>>
 *  version:
 *      v1.0.0
 *  time:
 *      Monday, May 4, 2009 (090504) at 1:10:27 AM UTC/GMT +8 hours
 *  location:
 *      Beijing, China on Earth
 *      latitude: 39.55 North, longtitude: 116.23 East
 *      international country code: + 86 (China)
 *      area code: 10
 *  weather:
 *      clear, mild
 *      temperature: 63 F (comfort level is 63 F)
 *      wind: 3 mph from 120 southeast by east
 *
 *  pgr_id: sizhe(email:sanyk.tan#gmail.com)
 *
 *  description: updating
 *
 *  >>>
 *  version:
 *      v1.0.1
 *  time:
 *      Monday, May 18, 2009 at 12:35:01 AM UTC/GMT +8 hours
 *  location:
 *      Beijing, China on Earth
 *      latitude: 39.55 North, longtitude: 116.23 East
 *      international country code: + 86 (China)
 *      area code: 10
 *  weather:
 *      clear, mild
 *      temperature: 82 F (comfort level is 81 F)
 *      wind: 1 mph from 0 north
 *
 *  pgr_id: sizhe(email:sanyk.tan#gmail.com)
 *
 *  description: created, add the bkc_string module
 *
 *  >>>
 *  version:
 *      v1.1.7
 *  time:
 *      Saturday, 27 February 2010, 16:54:57
 *      Monday, May 18, 2009 at 12:35:01 PM UTC/GMT +8 hours
 *  location:
 *      Beijing, China on Earth
 *      latitude: 39.55 North, longtitude: 116.23 East
 *      international country code: + 86 (China)
 *      area code: 10
 *  weather:
 *      ice fog, chilly
 *      temperature: 0 C (comfort level -3 C)
 *      wind: 9 km/h from 140 southeast
 *
 *  pgr_id: sizhe(email:sanyk.tan#gmail.com)
 *
 *  description:
 *      update building architecture and all the makefiles in
 *      subsidiary modules in bkc.
 ************************************************************************/
#ifndef BKC_H
#define BKC_H
/*********************included files*************************************/
#include "bkc_dlist.h"
#include "bkc_string_group.h"
#include "bkc_net_group.h"
#include "bkc_libc.h"
#include "bkc_pthread.h"
#include "bkc_queue.h"
#include "bkc_stack.h"
#include "bkc_regex.h"
#include "bkc_pasync.h"
#include "bkc_time.h"
#include "bkc_timer.h"
#include "bkc_socket.h"
#include "bkc_crc.h"
#include "bkc_store.h"

/*********************definition, types & constants *********************/

/*********************prototype of open functions************************/

#endif

