#include <wrapunix.h>
#include <gp_util.h>


int tcp_isreadable_bytm(int sock , int time_out)
{
    int                ret  = 0 ;
    long               timeout;
    struct pollfd fds[1];

    if (time_out <= 0)
        timeout = -1;
    else
        timeout = time_out;

    memset (fds, 0x0, sizeof (fds));
    fds[0].fd = sock;
    fds[0].events = POLLIN;  /* read event */

    ret = poll(fds, 1, timeout);
    if (ret < 0) {
        gp_errlog(LVL_ERROR, _FL_, "poll failed [%s][%d] timeout[%d]"
                    , strerror ( errno ) , errno , timeout);
        return -1;
    }

    if (ret == 0) { /* time out */
        gp_errlog(LVL_INFO , _FL_ , "poll timeout");
        return -2 ;
    }

    return 0;
}

int tcp_send_nbytes_tm(int sock, void *buf, int len, int time_out)
{
    int                left = len, cnt, sended = 0;
    int                ret  = 0 ;
    struct timeval     begin_time ;
    long               current ;
    long               begin_tm;
    long               timeout;
    long               consume = 0 ;
    struct pollfd      fds[1];

    memset(fds, 0x0, sizeof (fds));
    if (sock < 0 || buf == NULL || len < 0) {
        gp_errlog(LVL_ERROR, _FL_, "len [%d],buff[%p],sock[%d]  buff is null"
                    , len , buf , sock);
        return -1;
    }

    memset(&begin_time, 0x0, sizeof (struct timeval));
    memset(&current, 0x0, sizeof (struct timeval));

    GETTIMEOFDAY(begin_tm);

    if (time_out <= 0)
        timeout = -1;
    else
        timeout = time_out;

    while (left > 0) {
        memset(fds, 0x0, sizeof (fds));
        fds[0].fd = sock;
        fds[0].events = POLLOUT;  /* write event */

        ret = poll(fds, 1, timeout);
        if (ret < 0) {
            gp_errlog(LVL_ERROR, _FL_, "poll failed [%s][%d] timeout[%d]",
                strerror ( errno ), errno, timeout);
            return -1;
        }
        if (ret == 0) { /* time out */
            goto CONTINUE;
        }

        if (ret == 1) {
            cnt = send(sock, ((u_char * )buf) + sended, left, MSG_NOSIGNAL);
            if (cnt <= 0) {
                gp_errlog(LVL_ERROR, _FL_, "send[%d] failed [%s]",
                            left, strerror(errno));
                return -1;
            }
        }

        left   -= cnt;
        sended += cnt;
CONTINUE:
        GETTIMEOFDAY(current);
        consume = current - begin_tm;
        if (timeout != -1 && consume >= timeout  && left > 0) {
            gp_errlog(LVL_ERROR, _FL_, "send data consume [%ld] exccedd [%d]",
                    consume , timeout);
            return -1;
        } else if (timeout > 0) {
            timeout = timeout - consume;
        }
    }

    return 0;
}

int tcp_recv_nbytes(int sock, u_char *buf, int len, int time_out)
{
    long              current ;
    long              timeout ;
    long              begin_tm;
    int               left  ;
    int               recved ;
    int               ret;
    long              consume  = 0 ;
    struct  pollist {
        struct pollfd fds[1];
    } list;

    if (sock < 0) {
        gp_errlog(LVL_ERROR, _FL_, "sock <0");
        return -1;
    }

    if (buf == NULL || len < 0) {
        gp_errlog(LVL_ERROR, _FL_, "buf == NULL || len <0 ");
        return -1;
    }

    left     = len ;
    recved   = 0 ;

    GETTIMEOFDAY(begin_tm);
    if (time_out <= 0)
        timeout = -1;
    else
        timeout = time_out;

    while (left > 0) {

        list.fds[0].fd = sock;
        list.fds[0].events = POLLIN;  /* read event */

        ret = poll(list.fds, 1, timeout);
        if (ret < 0) {
            gp_errlog(LVL_ERROR, _FL_, "poll [%s][%d] timeout[%d]",
                    strerror ( errno ), errno, timeout);
            return -1;
        }

        if (ret == 0)  {
            gp_errlog(LVL_INFO, _FL_, "poll timeout[%s][%d]",
                        strerror ( errno ), errno);
            if (timeout > 0)
                goto CONTINUE;
            else  {
                gp_errlog(LVL_ERROR, _FL_, "socket error [%s] ",
                        strerror(errno));
                return -1;
            }

        }

        if (ret == 1) {
            ret = recv(sock, buf + recved, left, 0);
            if (ret < 0) {
                gp_errlog(LVL_ERROR, _FL_, "recv [%d] byte in [%d] [%s]"
                            "microseconds failded", left, timeout,
                            strerror(errno));
                return -1;
            } else if (ret == 0) {
                gp_errlog(LVL_ERROR, _FL_, "connect was reset by peer");
                return -1;
            }
            recved += ret;
            left -= ret;
        }

CONTINUE:
        GETTIMEOFDAY ( current );
        consume = current - begin_tm;
        if (timeout > 0 && consume >= time_out && left > 0) {
            gp_errlog(LVL_ERROR, _FL_, "total consume [%ld] mircoseconds,"
                        "excceed [%ld] mircoseconds", consume, time_out);
            return -1;
        } else if (timeout > 0) {
            timeout = timeout  -  consume ;
        }
    }

    return 0;
}


int bcl_setnbio(int fd, int flg)
{
    int     optval;

    if ( flg == TRUE )
        optval = 1;
    else
        optval = 0;

    if (ioctl(fd, FIONBIO, &optval) == -1) {
        gp_errlog(LVL_ERROR, _FL_, "ioctl[%s]", strerror(errno));
        return -1;
    }

    return 0;
}

int bcl_tcp_aconnector(u_int _addr, u_short _port)
{
    struct  sockaddr_in saddr;
    int     sock = -1;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        gp_errlog(LVL_ERROR, _FL_, "socket failed[%s]", strerror(errno));
        return -1;
    }

    if (bcl_setnbio(sock, TRUE) != 0) {
        gp_errlog(LVL_ERROR , _FL_, "bcl_setnbio()");
        close(sock);
        return -1;
    }

    memset(&saddr, 0x0, sizeof (saddr));
    saddr.sin_family       = AF_INET;
    saddr.sin_addr.s_addr  = _addr;
    saddr.sin_port         = htons(_port);

    if (connect(sock, (void*)&saddr, sizeof (saddr)) == 0) {
        /* success immediately */
        if (bcl_setnbio(sock, FALSE) != 0) {
            gp_errlog(LVL_ERROR , _FL_, "bcl_setnbio");
            close(sock);
            return -1;
        }
        return sock;
    }

    if (errno == EINPROGRESS)
        return sock;

    gp_errlog(LVL_ERROR, _FL_ , "connect[%s]", strerror(errno));
    close(sock);
    return -1;
}

int gp_strisdigit ( char *_str )
{
    while (*_str && *_str != '\n' && *_str != '\r') {
        if (!isdigit(*(_str ++))) {
            return FALSE;
        }
    }

    return TRUE;
}

/* no thread safe , but not use dns is ok */
int tcp_analysis_port(char *_portstr, u_short *_port)
{
    struct servent  *sent;

    if ( gp_strisdigit ( _portstr ) ) {
        *_port = ( u_short ) atoi ( _portstr );
        return 0;
    }

    if ((sent=getservbyname(_portstr, "tcp")) != NULL) {
        *_port = (u_short) (ntohs(sent->s_port ));
        return 0;
    }

    gp_errlog(LVL_ERROR, _FL_, "getservbyname()");
    return -1;
}

int tcp_analysis_addr ( char *_addrstr, u_int *_addr )
{
    struct hostent *hent;

    if ((*_addr = inet_addr(_addrstr)) != INADDR_NONE)
        return 0;

    if ((hent = gethostbyname(_addrstr)) != NULL) {
        *_addr = ((struct in_addr *)(hent->h_addr_list[0]))->s_addr;
        return 0;
    }

    gp_errlog(LVL_ERROR, _FL_, "gethostbyname()" );

    return -1;
}

int
gp_getsockerr ( int sock )
{
    int     errval;
    u_int   errlen = sizeof (errval);

    if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (void *)&errval, &errlen) < 0) {
        gp_errlog(LVL_ERROR, _FL_, "getsockopt()[%s]", strerror(errno));
        return -1;
    }

    return errval;
}


int tcp_aconn(u_int _addr, u_short _port, int _tmout)
{
    int                     sock = -1;
    fd_set                  read_fd;
    fd_set                  write_fd;
    struct timeval          tmout;
    struct timeval        * ptv = NULL;
    int                     ret = 0;

    if ((sock = bcl_tcp_aconnector(_addr, _port)) == -1) {
        gp_errlog(LVL_ERROR, _FL_, "bcl_tcp_aconnector");
        return -1;
    }

    if (errno != EINPROGRESS) /* success immediately */
        return sock;

    FD_ZERO(&read_fd);
    FD_ZERO(&write_fd);

    FD_SET(sock, &write_fd);
    FD_SET(sock, &read_fd);

    if (_tmout > 0) {
        tmout.tv_sec = _tmout / 1000;
        tmout.tv_usec = (_tmout % 1000) * 1000 ;
        ptv = &tmout;
    }

    if ((ret = select(sock + 1, &read_fd, &write_fd, 0, ptv)) <= 0) {
        if (ret == 0) {
            gp_errlog(LVL_ERROR, _FL_, "bcl_select timeout[%d]", _tmout);
        } else {
            gp_errlog(LVL_ERROR, _FL_, "select failed [%s]", strerror(errno));
        }
        close ( sock );
        return -1;
    }


    if (FD_ISSET(sock, &read_fd) || FD_ISSET(sock, &write_fd)) {
        if (gp_getsockerr(sock) == 0) {
            bcl_setnbio(sock, FALSE);
            return sock;
        }
    }

    /* failed */
    gp_errlog(LVL_ERROR, _FL_, "connect [%s]", strerror(errno));
    close(sock);
    return -1;
}


int tcp_listener ( char * ip , char * pp, int reuse_addr )
{
    u_int                  addr ;
    u_short                port ;
    struct sockaddr_in     list_addr ;
    int                    sockfd ;
    int                    ret ;


    if (ip[0] == 0) {
        addr = INADDR_ANY ;
    } else  {
        ret = tcp_analysis_addr(ip, &addr);
        if (ret < 0) {
            gp_errlog(LVL_ERROR, _FL_, "tcp_anaylase_addr [%s] failed" , ip);
            return -1;
        }
    }

    ret = tcp_analysis_port(pp, &port);
    if (ret < 0) {
        gp_errlog(LVL_ERROR, _FL_, "tcp_anaylase_port [%s] failed" , port);
        return -1;
    }

    list_addr.sin_family      = AF_INET;
    list_addr.sin_addr.s_addr = addr;
    list_addr.sin_port        = htons(port);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        gp_errlog(LVL_ERROR, _FL_, "socket failed [%s]", strerror(errno));
        return -1;
    }

    if (reuse_addr) {
        int     on = 1;
        ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                           &on, sizeof (on));
        if (ret < 0) {
            /* warning , but do not return fail */
            gp_errlog(LVL_ERROR, _FL_, "resuse addr failed[%s]",
                        strerror(errno));
        }
    }

    ret = bind(sockfd, (struct sockaddr *)&list_addr, sizeof (list_addr));
    if (ret < 0) {
        gp_errlog(LVL_ERROR, _FL_, "bind socket [%d] to  [%s:%s] failed",
                sockfd, ip, port);
        close(sockfd);
        return -1;
    }

    ret = listen(sockfd, SOMAXCONN);
    if (ret < 0) {
        gp_errlog(LVL_ERROR, _FL_, "listen socket [%d] to [%s:%s] failed",
                sockfd, ip, port);
        close(sockfd);
        return -1;
    }

    return sockfd ;
}
