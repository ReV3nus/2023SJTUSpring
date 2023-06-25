/*
 * proxy.c - ICS Web proxy
 *
 *
 */

#include "csapp.h"
#include <stdarg.h>
#include <sys/select.h>

/*
 * Function prototypes
 */
int parse_uri(char *uri, char *target_addr, char *path, char *port);
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, size_t size);
ssize_t Rio_readn_w(int fd, void *usrbuf, size_t n) 
{
    ssize_t rc;

    if ((rc = rio_readn(fd, usrbuf, n)) < 0)
    {
        fprintf(stderr, "%s: %s\n", "Rio_readn error", strerror(errno));
        return 0;
    }
    return rc;
}
ssize_t Rio_readlineb_w(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
    {
        fprintf(stderr, "%s: %s\n", "Rio_readlineb error", strerror(errno));
        return 0;
    }
    return rc;
} 
ssize_t Rio_writen_w(int fd, void *usrbuf, size_t n) 
{
    if (rio_writen(fd, usrbuf, n) != n)
    {
        fprintf(stderr, "%s: %s\n","Rio_writen error", strerror(errno));
        return -1;
    }
    return 0;
}
ssize_t recv_w(int fd, void *buf, size_t n, int flags)
{
    int rc=recv(fd,buf,n,flags);
    if(rc<0)
    {
        Sleep(1);
        rc=recv(fd,buf,n,flags);
        if(rc<0)
        {
            Sleep(1);
            rc=recv(fd,buf,n,flags);
            if(rc<0)return 0;
        }
    }
    return rc;
}
ssize_t Rio_readnb_w(rio_t *rp, void *usrbuf, size_t n) 
{
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
    {
        fprintf(stderr, "%s: %s\n","Rio_readnb error", strerror(errno));
        return 0;
    }
    return rc;
}
void doit(int connfd,struct sockaddr_in *sockaddr)
{
    Signal(SIGPIPE,SIG_IGN);
    errno=0;
    // FILE * testlog=fopen("testlog","a");
    char BUF[MAXLINE<<2];
    char headbuf[64][MAXLINE];
    int lhead=0;
    int nhead[64];
    unsigned char bodybuf[MAXLINE<<9];
    int nbody=0;
    char method[MAXLINE],uri[MAXLINE],version[MAXLINE];
    char hostname[MAXLINE],pathname[MAXLINE],port[MAXLINE];
    int serverfd;
    ssize_t N=0;
    rio_t rio;

/**************************
/receive request head
**************************/
    memset(rio.rio_buf,0,sizeof(rio.rio_buf));
    rio.rio_buf[0]='\0';
    int POSTlength=0;
    memset(headbuf,0,sizeof(headbuf));
    Rio_readinitb(&rio,connfd);
    Rio_readlineb_w(&rio,BUF,MAXLINE);

    sscanf(BUF,"%s %s %s",method,uri,version);
    if(parse_uri(uri,hostname,pathname,port)<0)
    {
        fprintf(stderr, "%s\n", "parse_uri error");
        Close(connfd);
        return;
    }
    sprintf(headbuf[lhead++],"%s %s %s\r\n",method,pathname,version);

/**************************
/REMAINING CLIENT REQUEST HEAD
**************************/
    while((nhead[lhead]=Rio_readlineb_w(&rio,headbuf[lhead],MAXLINE))!=0) 
    {
        lhead++;
        if(headbuf[lhead-1][0]=='\r'&&headbuf[lhead-1][1]=='\n')break;
        if(!strncasecmp(headbuf[lhead-1],"Content-Length",14))
        {
            sscanf(headbuf[lhead-1],"Content-Length:%d",&POSTlength);
        }
    }
    if(errno&&errno!=EINTR)
    {
        Close(connfd);
        return;
    }
    if(headbuf[lhead-1][0]!='\r'||headbuf[lhead-1][1]!='\n')
    {
        fprintf(stderr, "%s\n", "client crashed");
        Close(connfd);
        return;
    }


/**************************
/CLIENT REQUEST BODY if POST
**************************/
    if(!strcasecmp(method,"POST"))
    {
        memset(bodybuf,0,sizeof(bodybuf));
        while(POSTlength)
        {
            int n=Rio_readnb_w(&rio,bodybuf,POSTlength);
            POSTlength-=n;
            nbody+=n;
            if(!n)break;
        }
        if(POSTlength||(errno&&errno!=EINTR))
        {
            fprintf(stderr, "%s\n", "client crashed");
            Close(connfd);
            return;
        }
    }
/**************************
/SEND REQUEST TO SERVER
**************************/
    if((serverfd=open_clientfd(hostname,port))<0)
    {
        fprintf(stderr, "%s\n", "Open_clientfd error");
        Close(connfd);
        return;
    }
    nhead[0]=strlen(headbuf[0]);
    for(int i=0;i<lhead;i++)
    {
        if(Rio_writen_w(serverfd,headbuf[i],nhead[i])<0)
        {
            fprintf(stderr, "%s\n", "Writing to server error.\n");
            Close(serverfd);
            Close(connfd);
            return;
        }
    }
    if(!strcasecmp(method,"POST"))
    {
        if(Rio_writen_w(serverfd,bodybuf,nbody)<0)
        {
            fprintf(stderr, "%s\n", "Writing to server error.\n");
            Close(serverfd);
            Close(connfd);
            return;
        }
    }
/**************************
/SERVER RESPONSE HEAD
**************************/
    rio_t srio;
    memset(srio.rio_buf,0,sizeof(srio.rio_buf));
    srio.rio_buf[0]='\0';
    int RESPlength=0;
    Rio_readinitb(&srio,serverfd);

    memset(headbuf,0,sizeof(headbuf));
    memset(nhead,0,sizeof(nhead));
    lhead=0;
    while((nhead[lhead]=Rio_readlineb_w(&srio,headbuf[lhead],MAXLINE))!=0)
    {
        N+=nhead[lhead];
        lhead++;
        if(headbuf[lhead-1][0]=='\r'&&headbuf[lhead-1][1]=='\n')break;
        if(!strncasecmp(headbuf[lhead-1],"Content-Length",14))
        {
            sscanf(headbuf[lhead-1],"Content-Length:%d",&RESPlength);
        }
    }
    if(errno&&errno!=EINTR)
    {
        Close(serverfd);
        Close(connfd);
        return;
    }
    if(headbuf[lhead-1][0]!='\r'||headbuf[lhead-1][1]!='\n')
    {
        fprintf(stderr, "%s\n", "server crashed");
        Close(serverfd);
        Close(connfd);
        return;
    }

    for(int i=0;i<lhead;i++)
    {
        if(Rio_writen_w(connfd,headbuf[i],nhead[i])<0)
        {
            fprintf(stderr, "%s\n", "Writing to client error.\n");
            Close(serverfd);
            Close(connfd);
            return;
        }
    }

/**************************
/SERVER RESPONSE BODY
**************************/
    if(RESPlength)
    {
        memset(bodybuf,0,sizeof(bodybuf));
        if(srio.rio_cnt>0)
        {
            memcpy(bodybuf,srio.rio_bufptr,srio.rio_cnt);
            if(Rio_writen_w(connfd,bodybuf,srio.rio_cnt)<0)
            {
                fprintf(stderr, "%s\n", "server crashed");
                Close(serverfd);
                Close(connfd);
                return;
            }
            N+=srio.rio_cnt;
            nbody+=srio.rio_cnt;
            RESPlength-=srio.rio_cnt;
        }
        while(RESPlength)
        {
            int n=read(serverfd,bodybuf,MAXLINE>RESPlength?RESPlength:MAXLINE);
            if(n<=0)break;
            // if(RESPlength>MAXLINE)n=Rio_readnb_w(&srio,bodybuf,MAXLINE);
            // else n=Rio_readnb_w(&srio,bodybuf,RESPlength);
            if(Rio_writen_w(connfd,bodybuf,n)<0)break;
            N+=n;
            nbody+=n;
            RESPlength-=n;
        }
        if(RESPlength||(errno&&errno!=EINTR))
        {
            fprintf(stderr, "%s\n", "server crashed");
            Close(serverfd);
            Close(connfd);
            return;
        }
        RESPlength=-1;
    }

/**************************
/SEND RESPONSE TO CLIENT
// **************************/
//     if(RESPlength==-1)
//     {
//         if(Rio_writen_w(connfd,bodybuf,nbody)<0)
//         {
//             fprintf(stderr, "%s\n", "Writing to client error.\n");
//             Close(serverfd);
//             Close(connfd);
//             return;
//         }
//     }



    // fprintf(testlog,"finished reading from server\n");
    char logstring[MAXLINE];
    format_log_entry(logstring,sockaddr,uri,N);
    printf("%s\n ",logstring);
    fflush(stdout);
    // fprintf(testlog,"logstring:\t%s",logstring);
    // fclose(testlog);
    Close(serverfd);
    Close(connfd);
    return;
}
/*
 * main - Main routine for the proxy program
 */
struct varhead
{
    int connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
};
void *thread(void *vargp)
{
    struct varhead var=*((struct varhead *)vargp);
    int connfd=var.connfd;
    // socklen_t clientlen=var.clientlen;
    struct sockaddr_storage clientaddr=var.clientaddr;
    Pthread_detach(pthread_self());
    Free(vargp);
    doit(connfd,(struct sockaddr_in *)&clientaddr);
    return NULL;
}
int main(int argc, char **argv)
{
    /* Check arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        exit(0);
    }
    // char hostname[MAXLINE],port[MAXLINE];

    int listenfd;
    listenfd=Open_listenfd(argv[1]);
    // socklen_t clientlen;
    // struct sockaddr_storage clientaddr;
    pthread_t tid;
    // int *connfdp;
    struct varhead* varheadp;
    while(1)
    {
        varheadp=Malloc(sizeof(struct varhead));
        varheadp->clientlen=sizeof(struct sockaddr_storage);
        varheadp->connfd=Accept(listenfd,(SA*)&varheadp->clientaddr,&varheadp->clientlen);
        Pthread_create(&tid,NULL,thread,varheadp);
        // doit(connfd,(struct sockaddr_in *)&clientaddr);
    }


    exit(0);
}


/*
 * parse_uri - URI parser
 *
 * Given a URI from an HTTP proxy GET request (i.e., a URL), extract
 * the host name, path name, and port.  The memory for hostname and
 * pathname must already be allocated and should be at least MAXLINE
 * bytes. Return -1 if there are any problems.
 */
int parse_uri(char *uri, char *hostname, char *pathname, char *port)
{
    char *hostbegin;
    char *hostend;
    char *pathbegin;
    int len;

    if (strncasecmp(uri, "http://", 7) != 0) {
        hostname[0] = '\0';
        return -1;
    }

    /* Extract the host name */
    hostbegin = uri + 7;
    hostend = strpbrk(hostbegin, " :/\r\n\0");
    if (hostend == NULL)
        return -1;
    len = hostend - hostbegin;
    strncpy(hostname, hostbegin, len);
    hostname[len] = '\0';

    /* Extract the port number */
    if (*hostend == ':') {
        char *p = hostend + 1;
        while (isdigit(*p))
            *port++ = *p++;
        *port = '\0';
    } else {
        strcpy(port, "80");
    }

    /* Extract the path */
    pathbegin = strchr(hostbegin, '/');
    if (pathbegin == NULL) {
        pathname[0] = '\0';
    }
    else {
        // pathbegin++;
        strcpy(pathname, pathbegin);
    }

    return 0;
}

/*
 * format_log_entry - Create a formatted log entry in logstring.
 *
 * The inputs are the socket address of the requesting client
 * (sockaddr), the URI from the request (uri), the number of bytes
 * from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr,
                      char *uri, size_t size)
{
    time_t now;
    char time_str[MAXLINE];
    char host[INET_ADDRSTRLEN];

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    if (inet_ntop(AF_INET, &sockaddr->sin_addr, host, sizeof(host)) == NULL)
        unix_error("Convert sockaddr_in to string representation failed\n");

    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %s %s %zu", time_str, host, uri, size);
}
