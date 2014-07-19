#include <wrapunix.h>
#include <gp_util.h>

#define DEFAULT_LOG_FILE_SIZE 30 * 1024 * 1024 



static int __log_level__;


void gp_background()
{
    pid_t pid;

    pid = fork();
    switch(pid) {
    case 0:
         return  ;
    case -1:
         fprintf(stdout, "fork failed[%s]\n", strerror(errno));
         exit(-1);
    default:
         exit(0);
    }
}


void gp_set_log_level(int level) 
{
    __log_level__ = level;
}

void                                                                            
gp_str_toupper( char *s )                                                      
{                                                                               
    while ( *s ) {                                                              
        *s = toupper( *s );                                                     
        ++s;                                                                    
    }                                                                           
}


void
gp_get_date_time( const char * fmt, char * dt_buf )
{
    int          i;
    int          k;
    int          fmt_len;
    struct tm    tm;
    struct timeb ptime;

    ftime(&ptime);
    localtime_r( &(ptime.time), &tm  );

    k = 0;
    i = 0;
    fmt_len = strlen( fmt );

    do
    {
        if ( fmt[ i ] == '%' )
        {
            switch ( fmt[ i + 1 ] )
            {
                case  'Y':
                case  'y':
                      sprintf( &dt_buf[k],"%04d",tm.tm_year+1900 );
                      k += 4;
                      break;
                case  'M':
                case  'm':
                      sprintf( &dt_buf[k], "%02d", tm.tm_mon+1 );
                      k += 2;
                      break;
                case  'D':
                case  'd':
                      sprintf( &dt_buf[k], "%02d", tm.tm_mday );
                      k += 2;
                      break;
                case  'H':
                case  'h':
                      sprintf( &dt_buf[k], "%02d", tm.tm_hour );
                      k += 2;
                      break;
                case  'N':
                case  'n':
                      sprintf( &dt_buf[k], "%02d", tm.tm_min );
                      k += 2;
                      break;
                case  'S':
                case  's':
                      sprintf( &dt_buf[k], "%02d", tm.tm_sec );
                      k += 2;
                      break;
                case  'U':
                case  'u':
                      sprintf( &dt_buf[k], "%03d", ptime.millitm);
                      k += 3;
                      break;
                default  :
                      dt_buf[k++] = fmt[i+1];
                      break;
            }
            i = i + 1;
        }
        else
            dt_buf[ k++ ] = fmt[ i ];

        i ++;

    } while ( i < fmt_len );

    dt_buf[ k ] = 0;

    return; 
}



static void rename_file_by_time( char* log_file )
{
    char backfile[256];
    time_t cNow;
    struct tm *pcNow;
    
    time(&cNow);
    pcNow = localtime( &cNow );
    sprintf(backfile,"%s.%02d%02d%02d%02d",
        log_file,
        pcNow->tm_mon+1,
        pcNow->tm_mday,
        pcNow->tm_hour,
        pcNow->tm_min);
    rename(log_file,backfile);
}



void
gp_errlog( int level,char * file,int line, char *format, ... )
{
    char log_file[256];
    static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    FILE *fp = NULL;
    long fsize = 0;
    va_list ap;
    char     strtime[256];

    
    if( level < __log_level__){
        return ;
    }

    memset( log_file, 0x00, sizeof(log_file) );
    memset(strtime,0x0,sizeof(strtime));
    sprintf( log_file, "%s/log/%s", getenv("RUNNERDIR"), "running.log" );


    gp_get_date_time( "%M%D.%H%N%S.%U", strtime );
    pthread_mutex_lock( &lock );
    if( (fp=fopen(log_file,"a")) == NULL ){
        fp = stderr;
        fprintf( fp, "open file error use stderr instead, <%s>, %s\n",
                    log_file, strerror(errno) );
    }



    fprintf( fp, "%s tid:%lu %s.%d,", strtime,pthread_self(),file,line );

    switch( level ){
        case LVL_DEBUG:
            fprintf( fp, "(D)," );
            break;
        case LVL_INFO:
            fprintf( fp, "(F)," );
            break;
        case LVL_WARN:
            fprintf( fp, "(W)," );
            break;
        case LVL_ERROR:
            fprintf( fp, "(E)," );
            break;
        case LVL_FREE:
            break;
        default:
            fprintf( fp, "(O)," );
            break;
    }


    va_start( ap, format );
    vfprintf( fp, format, ap );
    va_end( ap );
    if( level != LVL_FREE ){
        fprintf( fp, "\n" );
    }
    if( fp == stderr ){                            
        pthread_mutex_unlock( &lock );
        return;
    }


    fsize=ftell(fp);
    fclose(fp);
    if( fsize > DEFAULT_LOG_FILE_SIZE ){
        rename_file_by_time( log_file );
    }
    pthread_mutex_unlock( &lock );
}


#ifndef FALSE
#define FALSE 0
#else
#undef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#else
#undef TRUE
#define TRUE 1
#endif


char * gp_ltrim( char *str )
{
    register char *pstart, *ptr;

    if ( str == NULL )
        return NULL;

    pstart = ptr = str;

    while ( *( ( unsigned char * )ptr ) == 0x20 || *ptr == '\t' || \
            *ptr == '\r' || *ptr == '\n' )
        ptr ++;

    while ( *ptr != 0x0 )
        *str ++ = *ptr ++;

    *str = 0x0;

    return pstart;
}

char * gp_rtrim( char *str )
{
    register char *ptr;

    if ( str == NULL )
        return NULL;

    ptr = str + strlen( str ) - 1;

    while ( ptr >= str && ( *( ( unsigned char * )ptr ) == 0x20 || \
            *ptr == '\t' ||  *ptr == '\r' || *ptr == '\n' ))
        *ptr -- = 0x0;

    return str;
}

char *
gp_strchr( char *_str, char _ch )
{
    register int i=0,j;

    while( _str[i] != 0 ) {
        if( _str[i] == _ch ) {
            if( i == 0 )
                break;
            j = i-1;
            do {
                if( _str[j] != '\\' )
                    break;
                j--;
            } while( j >= 0 );
            if( ( ( i - j ) % 2 ) )
                break;
        }
        i++;
    }
    if( _str[i] == 0 )
        return NULL;
    return &(_str[i]);
}

int
gp_char_num( char *str, char ch )
{
    char *lp, *cp;
    int num = 0;

    if ( str == NULL )
        return -1;

    lp = str;
    while ( (cp=gp_strchr(lp, ch)) != NULL ) {
        ++num;
        lp = cp + 1;
    }

    return num;
}


char**
gp_strsplit( char *str, char deli, int *spnum )
{
    char **retval, *lp=str, *cp;
    int  num=0, i, len;

    if ( str == NULL ) {
        *spnum = 0;
        return NULL;
    }

    if ( strlen(str) == 0 ) {
        *spnum = 0;
        return NULL;
    }

    num = gp_char_num(str, deli) + 1;
    *spnum = num;
    retval = (char **)malloc( num * sizeof(char*) );

    for ( i=0 ; i<num ; ++i ) {
        cp = gp_strchr( lp, deli );

        if ( cp == NULL )
            len = strlen( lp );
        else
            len = cp - lp;

        retval[i] = (char *)malloc( len + 1 );
        strncpy( retval[i], lp, len );
        retval[i][len] = 0 ;
        lp = cp + 1;
    }

    return retval;
}

void
gp_freespace( char **p, int nitems )
{
    int        i;

    if ( p == (char **)NULL )
        return;

    for ( i = 0; i < nitems; i ++ ) 
        if ( p[i] != NULL )
            free( p[i] );

    free( p );
}


void gp_sig_block(int sig)
{
   sigset_t        in ; 
   sigset_t        out ; 

   sigemptyset( &in );
   sigaddset( &in , sig ) ;
   sigprocmask( SIG_BLOCK , &in , &out );
   return ;
}

void gp_sig_blockall()
{
   sigset_t        in;
   sigset_t        out ; 

   sigfillset( &in );
   sigdelset( &in , SIGTERM );
   sigdelset( &in , SIGTRAP );
   sigprocmask( SIG_BLOCK , &in , &out );
   return ;
}

void gp_sig_unblock(int sig)
{
   sigset_t        in ; 
   sigset_t        out ; 

   sigemptyset( &in );
   sigaddset( &in , sig ) ;
   sigprocmask( SIG_UNBLOCK , &in , &out );
 
   return ;
}



#define write_lock( fd , offset , whence , len ) \
            lock_reg( fd , F_SETLK , F_WRLCK , offset , whence , len )

#define write_lockw( fd , offset , whence , len ) \
            lock_reg( fd , F_SETLKW , F_WRLCK , offset , whence , len )

#define FILE_MODE    (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

#define un_lock(fd,offset,whence,len) \
    lock_reg( fd , F_SETLK , F_UNLCK , offset , whence , len )



static int 
lock_reg( int fd , int cmd , int type , off_t offset , int whence , off_t len )
{
    struct flock lock;

    lock.l_type        = type;
    lock.l_start    = offset;
    lock.l_whence    = whence;
    lock.l_len        = len;

    return ( fcntl( fd , cmd , &lock ) );
}

int gp_file_waitlock( char* lock_file_name )
{
    int fd ;
    
    if ((fd=open(lock_file_name , O_WRONLY | O_CREAT , FILE_MODE ) ) < 0) {
        gp_errlog( LVL_ERROR , _FL_ ,  "fopen" , lock_file_name );
        return -6;
    }

    if (write_lockw( fd , 0 , SEEK_SET , 1 ) < 0) {    
        gp_errlog( LVL_ERROR , _FL_ , "write_lock[%s]" , strerror(errno)  );
        return -5;
    }

    return fd;
}

int gp_file_lock( char* lock_file_name )
{
    
    int fd ;
    char buf[10];
    
    if ((fd = open(lock_file_name, O_WRONLY | O_CREAT, FILE_MODE)) < 0) {
        gp_errlog(LVL_ERROR , _FL_ ,  "fopen" , lock_file_name);
        return -6;
    }

    if (write_lock(fd , 0 , SEEK_SET , 0) < 0) {    
        gp_errlog( LVL_ERROR , _FL_ , "write_lock[%s]" , strerror(errno)  );
        return -5;
    }

    if (ftruncate(fd , 0) < 0) {
        gp_errlog( LVL_ERROR , _FL_ ,  "ftruncate" , "" );
        return -4;
    }

    sprintf(buf , "%d\n" , getpid());

    if (write(fd, buf, strlen(buf)) != strlen(buf)) {
        gp_errlog( LVL_ERROR , _FL_ ,  "write" , "" );
        return -3;
    }

    return fd;
}


void gp_file_unlock(int fd)
{
    close(fd);
    return ;
}


void gp_udelay( long usec )
{
    struct    timeval   timeout;

    timeout.tv_usec = usec%1000000;
    timeout.tv_sec = usec/1000000;

    select( 1, NULL, NULL, NULL, &timeout );
}
