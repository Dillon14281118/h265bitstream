/**
 * @h265_analyze.c
 * reading bitstream of H.265
 * @author hanyi <13141211944@163.com>
 */

#include "h265_stream.h"

 //#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 32*1024*1024

#if (defined(__GNUC__))
#define HAVE_GETOPT_LONG

#include <getopt.h>


static struct option long_options[] =
{
    { "probe",   no_argument, NULL, 'p'},
    { "output",  required_argument, NULL, 'o'},
    { "help",    no_argument,       NULL, 'h'},
    { "verbose", required_argument, NULL, 'v'},
};
#endif