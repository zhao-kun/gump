#ifndef _PR_UTIL_H
#define _PR_UTIL_H


#ifdef __cplusplus
extern C {
#endif


#define  LVL_DEBUG     0
#define  LVL_INFO      1
#define  LVL_WARN      2
#define  LVL_ERROR     3
#define  LVL_FREE      4

#define GETTIMEOFDAY(x)                         \
do {                                           \
   struct timeval tmptimeval__;               \
   gettimeofday(&tmptimeval__,NULL);          \
   (x) = (tmptimeval__.tv_sec * 1000 + tmptimeval__.tv_usec/1000 ); \
} while (0);


void gp_errlog( int level,char * file,int line, char *format, ... );
char * gp_ltrim( char *str );
char * gp_rtrim( char *str );
char * gp_strchr( char *_str, char _ch );
void gp_sig_blockall();
void gp_sig_block(int sig);
int gp_file_lock( char* lock_file_name );
void gp_udelay( long usec );

#ifdef __cplusplus
}
#endif

#endif
