#include <wrapunix.h>
void pr_errlog(int level, char * file, int line, char * fmt, ...)
{
     

    va_list argp;

    fprintf(stderr, "%s:%d", file, line);
    va_start(argp, fmt);
    vfprintf(stderr,fmt, argp);
    va_end(argp);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);

}
