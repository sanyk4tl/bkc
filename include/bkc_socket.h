/*****************************************************************************
 *  copyleft(c)
 *****************************************************************************
 *  module: bkc_socket
 *  file name: bkc_socket.h
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
#ifndef BKC_SOCKET_H
#define BKC_SOCKET_H

/*********************included files*****************************************/
#include "bkc_dlist.h"

/*********************definition, types & constants *************************/
/*ret value of funcs in bkc_socket*/
#define BKC_SOCKET_RSUC   (0)
#define BKC_SOCKET_RERR   (-1)

#define BKC_SOCKET_AF_INET         (2)
#define BKC_SOCKET_AF_INET6        (23)

#define BKC_SOCKET_SO_STREAM     (1)
#define BKC_SOCKET_SO_DGRAM      (2)

#define BKC_SOCKET_SOL_SOCKET      (0xFFFF)

#define BKC_SOCKET_SO_REUSEADDR    (0x0004)
#define BKC_SOCKET_SO_KEEPALIVE    (0x0008)
#define BKC_SOCKET_SO_BROADCAST    (0x0020)
#define BKC_SOCKET_SO_LINGER       (0x0080)

/*for shutdown a socket*/
#define BKC_SOCKET_SD_RECEIVE     (0)
#define BKC_SOCKET_SD_SEND        (1)
#define BKC_SOCKET_SD_ALL         (2)

#define BKC_SOCKET_IPPROTO_IP      (0)
#define BKC_SOCKET_IP_VERSION_4    ((unsigned char)4)
#define BKC_SOCKET_IP_VERSION_6    ((unsigned char)6)

/*types forward declaration in bkc_socket module*/
typedef struct bkc_socket_tag_t bkc_socket_t;

/*ipv4 address */
typedef struct bkc_socket_ipv4_s{
     unsigned int ip;
} bkc_socket_ipv4_t;

/*ipv6 address */
typedef struct bkc_socket_ipv6_s{
    unsigned short ip[8];
} bkc_socket_ipv6_t;

/*union ip address */
typedef union bkc_socket_ip_u {
    bkc_socket_ipv4_t ipv4;
    bkc_socket_ipv6_t ipv6;
} bkc_socket_ip_n;

typedef struct bkc_socket_ipaddr_s {
    /*version is BKC_SOCKET_IP_VERSION_4 or BKC_SOCKET_IP_VERSION_6*/
    unsigned char version;
    unsigned char padding1;
    unsigned char padding2;
    unsigned char padding3;
    bkc_socket_ip_n addr;
} bkc_socket_ipaddr_t;

typedef struct bkc_socket_soaddr_s {
    int port;
    bkc_socket_ipaddr_t ipaddr;
} bkc_socket_soaddr_t;

typedef struct bkc_socket_linger_s {
    unsigned short l_onoff;
    unsigned short l_linger;
} bkc_socket_linger_t;

typedef struct bkc_socket_soset_s {
    /*list for the socket in the set*/
    bkc_dlist_t *solist_p;
} bkc_socket_soset_t;

/*the four basic operations on the socket set*/
#define BKC_SOCKET_SD_SET(socket_p, set_p)    \
    bkc_socket_sd_set(socket_p, set_p)

#define BKC_SOCKET_SD_CLR(socket_p, set_p)    \
    bkc_socket_sd_clr(socket_p, set_p)

#define BKC_SOCKET_SD_ISSET(socket_p, set_p)  \
    bkc_socket_sd_isset(socket_p, set_p)

#define BKC_SOCKET_SD_ZERO(set_p)    \
    bkc_socket_sd_zero(set_p)

/*********************prototype of open functions****************************/
/*****************************************************************************
 *                  bkc_socket_open
 *****************************************************************************
 * parameters:
 *     family:
 *         BKC_SOCKET_AF_INET: ipv4
 *         BKC_SOCKET_AF_INET6: ipv6
 *     type:
 *         specifies the communication semantics, can be:
 *         BKC_SOCKET_SO_STREAM
 *         BKC_SOCKET_SO_DGRAM
 *     protocol:
 *         specifies a particular protocol to be used with the socket, can be:
 *         BKC_SOCKET_IPPROTO_IP
 * return value:
 *     On success, a file descriptor for  the  new  socket  is  returned.
 *     On error, -1 is returned.
 * description:
 *     creates  an endpoint for communication and returns a descriptor.
 ***************************** notes **************************************
 ****************************************************************************/
bkc_socket_t *bkc_socket_open(int family, int type, int protocol);

/*****************************************************************************
 *                  bkc_socket_bind
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the  file descriptor which created by BKC_SOCKET_open.
 *     addr_p
 *         the ip address to be bind to sockfd
 * return value:
 *     On success, zero is returned.  On error, -1 is returned.
 * description:
 *     bind a ip address to socket referred by sockfd.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_bind(
        bkc_socket_t *socket_p, bkc_socket_soaddr_t *addr_p);

/*****************************************************************************
 *                  bkc_socket_listen
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the  file descriptor which created by BKC_SOCKET_open.
 *     backlog
 *         The backlog argument defines the maximum length to which the
 *         queue of pending connections for sockfd may grow.
 * return value:
 *     On success, zero is returned.  On error, -1 is returned.
 * description:
 *     marks  the socket referred to by sockfd as a passive socket.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_listen(bkc_socket_t *socket_p, int backlog);

/*****************************************************************************
 *                  bkc_socket_accept
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the file descriptor which created by BKC_SOCKET_open.
 *     ipaddr:
 *         for storing the address of the peer socket.
 *     port:
 *         for storing the port of the peer socket.
 * return value:
 *     On success, a file descriptor for  the  new  socket  is  returned.
 *     On error, -1 is returned.
 * description:
 *     It extracts the first connection request on the queue of pending
 *     connections  for  the  listening  socket, sockfd, creates a new
 *     connected socket, and returns a new file descriptor
 *     referring to that socket.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_accept(bkc_socket_t *socket_p, bkc_socket_soaddr_t *addr_p);

/*****************************************************************************
 *                  bkc_socket_connect
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the file descriptor which created by BKC_SOCKET_open.
 *     ipaddr:
 *         the address of the peer socket.
 *     port:
 *         the port of the peer socket.
 * return value:
 *     On success, zero is returned.  On error, -1 is returned.
 * description:
 *     this func connects the socket referred to by the file descriptor
 *     sockfd to the address specified by ipaddr.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_connect(bkc_socket_t *socket_p, bkc_socket_soaddr_t *addr_p);

/*****************************************************************************
 *                  bkc_socket_send
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the file descriptor which created by BKC_SOCKET_open.
 *     buf:
 *         buf to be sent.
 *     len:
 *         length of the buf.
 *     flags:
 *         must be set to 0, other value have no significance
 *         currently.
 * return value:
 *     On success, is the number of send-bytes.  On error, -1 is returned.
 * description:
 *     transmit a message to another socket through sockfd.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_send(bkc_socket_t *socket_p, char *buf_p, int len, int flags);

/*****************************************************************************
 *                  BKC_SOCKET_sendto
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the file descriptor which created by BKC_SOCKET_open.
 *     buf:
 *         buf to be sent.
 *     len:
 *         length of the buf.
 *     flags:
 *         must be set to 0, other value have no significance
 *         currently.
 *     ipaddr:
 *         the destination address.
 *     port:
 *         the destination port.
 * return value:
 *     On success, is the number of sent-bytes.  On error, -1 is returned.
 * description:
 *     transmit a message to another socket through sockfd.
 ***************************** notes **************************************
 ****************************************************************************/
int BKC_SOCKET_sendto(bkc_socket_t *socket_p, char *buf_p, int len,
        int flags, bkc_socket_soaddr_t *addr_p);

/*****************************************************************************
 *                  bkc_socket_recv
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the file descriptor which created by BKC_SOCKET_open.
 *     buf:
 *         buf to be sent.
 *     len:
 *         length of the buf.
 *     flags:
 *         must be set to 0, other value have no significance
 *         currently.
 * return value:
 *     On success, is the number of received-bytes.  On error, -1 is returned.
 * description:
 *     receive a message from sockfd.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_recv(bkc_socket_t *socket_p, char *buf_p, int len, int flags);

/*****************************************************************************
 *                  bkc_socket_recvfrom
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the file descriptor which created by BKC_SOCKET_open.
 *     buf:
 *         buf to be sent.
 *     len:
 *         length of the buf.
 *     flags:
 *         must be set to 0, other value have no significance
 *         currently.
 * return value:
 *     On success, is the number of received-bytes.  On error, -1 is returned.
 * description:
 *     receive a message from sockfd.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_recvfrom(bkc_socket_t *socket_p, char *buf_p, int len, int flags,
        bkc_socket_soaddr_t *addr_p);

/*****************************************************************************
 *                  bkc_socket_getsockname
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the file descriptor which created by BKC_SOCKET_open.
 *     ipaddr:
 *         for storing the destination address.
 *     port:
 *         for storing the destination port.
 * return value:
 *     On success, is the number of received-bytes.  On error, -1 is returned.
 * description:
 *     returns the current address to which the socket sockfd is bound.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_getsockname(bkc_socket_t *socket_p,
        bkc_socket_soaddr_t *addr_p);

/*****************************************************************************
 *                  bkc_socket_select
 *****************************************************************************
 * parameters:
 *     nfds£º
 *         the highest-numbered file descriptor in any of the
 *         three sets, plus 1.
 *     readset:
 *         file descriptor set for  read-checking.
 *     writeset:
 *         file descriptor set for  write-checking.
 *     exceptset:
 *         file descriptor set for  error-checking.
 *     timeout:
 *         BKC_SOCKET_NO_WAIT(0):
 *             return at once.
 *         BKC_SOCKETL_WAIT_FOREVER(-1):
 *             always waiting until a file descriptor is ready.
 *         (other):
 *             microseconds to wait
 * return value:
 *     On success, is the number of received-bytes.  On error, -1 is returned.
 * description:
 *     monitor multiple file descriptors, waiting until one or more
 *     of the file descriptors become "ready" for some class of I/O operation.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_select(bkc_socket_soset_t *readset_p,
        bkc_socket_soset_t *writeset_p,  bkc_socket_soset_t *exceptset_p,
        int timeout);

/*****************************************************************************
 *                  bkc_socket_getsockopt
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the file descriptor which created by BKC_SOCKET_open.
 *     level:
 *         the protocal level which the attributes belongs to.
 *     optname:
 *         name of the attribute to get.
 *     optval:
 *         buf to store value of the designated attribute.
 *         can not be NULL.
 *     optlen:
 *         len of the optval.
 *         can not be NULL.
 * return value:
 *     On success, zero is returned.  On error, -1 is returned.
 * description:
 *     get the attibute value of the socket.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_getsockopt(bkc_socket_t *socket_p, int level, int optname,
        void *optval, int *optlen);

/*****************************************************************************
 *                  bkc_socket_setsockopt
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the file descriptor which created by BKC_SOCKET_open.
 *     level:
 *         the protocal level which the attributes belongs to.
 *     optname:
 *         name of the attribute to set.
 *     optval:
 *         buf to store value of the designated attribute.
 *     optlen:
 *         len of the optval.
 * return value:
 *     On success, zero is returned.  On error, -1 is returned.
 * description:
 *     set the attibute value of the socket.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_setsockopt(bkc_socket_t *socket_p, int level, int optname,
    void *optval, int optlen);

/*****************************************************************************
 *                  bkc_socket_shutdown
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the file descriptor which created by BKC_SOCKET_open.
 *     what:
 *         BKC_SOCKET_DIS_RECEIVE:
 *             disable the receiving on this socket.
 *         BKC_SOCKET_DIS_SEND:
 *             disable the sending on this socket.
 *         BKC_SOCKET_DIS_ALL:
 *             disable both sending and receiving on this socket.
 * return value:
 *     On success, zero is returned.  On error, -1 is returned.
 * description:
 *     control the data transmission on the socket.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_shutdown(bkc_socket_t *socket_p, int what);

/*****************************************************************************
 *                  bkc_socket_close
 *****************************************************************************
 * parameters:
 *     sockfd£º
 *         the file descriptor which created by BKC_SOCKET_open.
 * return value:
 *     On success, zero is returned.  On error, -1 is returned.
 * description:
 *     close the socket.
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_close(bkc_socket_t *socket_p);

/*****************************************************************************
 *                  bkc_socket_sd_set
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_sd_set(bkc_socket_t *socket_p, bkc_socket_soset_t *set_p);

/*****************************************************************************
 *                  bkc_socket_sd_clr
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_sd_clr(bkc_socket_t *socket_p, bkc_socket_soset_t *set_p);

/*****************************************************************************
 *                  bkc_socket_sd_isset
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_sd_isset(bkc_socket_t *socket_p, bkc_socket_soset_t *set_p);

/*****************************************************************************
 *                  bkc_socket_sd_zero
 *****************************************************************************
 * parameters:
 * return value:
 * description:
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_sd_zero(bkc_socket_t *socket_p);

/*****************************************************************************
 *                  bkc_socket_watchmm
 *****************************************************************************
 * parameters: N/A
 * return value:
 *     BKC_SOCKET_RSUC: sucessfull return
 *     BKC_SOCKET_RERR: erroneous return
 *     (other): abnormal return
 * description:
 *     this func is only used for debugging, and can show the memory
 *     infomation about this module.
 *     if not in debuggin mode ( BKC_SOCKET_SPF_RELEASE is not
 *     defined in bkc_socket_spf.h) this func will always show
 *     empty messages
 *
 *     it is usefull when take a white-box testing in this
 *     module for showing the mormory used by this module
 ***************************** notes **************************************
 ****************************************************************************/
int bkc_socket_watchmm(void);

#endif

