// h265bitstreamCMake.cpp: 定义应用程序的入口点。
//

/**
 * @h265_analyze.c
 * reading bitstream of H.265
 * @author hanyi <13141211944@163.com>
 */
#include "h265bitstreamCMake.h"

static char options[] =
"\t-o output_file, defaults to test.265\n"
"\t-v verbose_level, print more info\n"
"\t-p print codec for HTML5 video tag's codecs parameter, per RFC6381\n"
"\t-h print this message and exit\n";

void usage()
{

    fprintf(stderr, "h265_analyze, version 1.0\n");
    fprintf(stderr, "Analyze H.265 bitstreams in Annex B format\n");
    fprintf(stderr, "Usage: \n");

    fprintf(stderr, "h265_analyze [options] <input bitstream>\noptions:\n%s\n", options);
}

int main(int argc, char* argv[])
{
    FILE* infile;

    uint8_t* buf = (uint8_t*)malloc(BUFSIZE);

    h265_stream_t* h = h265_new();

    if (argc < 2) { usage(); return EXIT_FAILURE; }

    int opt_verbose = 1;
    int opt_probe = 0;

#ifdef HAVE_GETOPT_LONG
    int c;
    int long_options_index;
    extern char* optarg;
    extern int   optind;

    while ((c = getopt_long(argc, argv, "o:phv:", long_options, &long_options_index)) != -1)
    {
        switch (c)
        {
        case 'o':
            if (h265_dbgfile == NULL) { h265_dbgfile = fopen(optarg, "wt"); }
            break;
        case 'p':
            opt_probe = 1;
            opt_verbose = 0;
            break;
        case 'v':
            opt_verbose = atoi(optarg);
            break;
        case 'h':
        default:
            usage();
            return 1;
        }
    }

    infile = fopen(argv[optind], "rb");

#else

    infile = fopen(argv[1], "rb");

#endif

    if (infile == NULL) { fprintf(stderr, "!! Error: could not open file: %s \n", strerror(errno)); exit(EXIT_FAILURE); }

    if (h265_dbgfile == NULL) { h265_dbgfile = stdout; }


    size_t rsz = 0;
    size_t sz = 0;
    int64_t off = 0;
    uint8_t* p = buf;

    int nal_start, nal_end;

    while (1)
    {
        rsz = fread(buf + sz, 1, BUFSIZE - sz, infile);
        if (rsz == 0)
        {
            if (ferror(infile)) { fprintf(stderr, "!! Error: read failed: %s \n", strerror(errno)); break; }
            break;  // if (feof(infile)) 
        }

        sz += rsz;
        //寻找nal的开头和结尾的位置，返回nal长度
        while (find_nal_unit(p, sz, &nal_start, &nal_end) > 0)
        {
            if (opt_verbose > 0)
            {
                fprintf(h265_dbgfile, "!! Found NAL at offset %lld (0x%04llX), size %lld (0x%04llX) \n",
                    (long long int)(off + (p - buf) + nal_start),
                    (long long int)(off + (p - buf) + nal_start),
                    (long long int)(nal_end - nal_start),
                    (long long int)(nal_end - nal_start));
            }

            p += nal_start;
            read_debug_nal_unit(h, p, nal_end - nal_start);

            if (opt_probe && h->nal->nal_unit_type == NAL_UNIT_VPS)
            {
                // print codec parameter, per RFC 6381.

                // TODO: add more, move to h265_stream (?)
                break; // we've seen enough, bailing out.
            }

            if (opt_probe && h->nal->nal_unit_type == NAL_UNIT_SPS)
            {
                // print codec parameter, per RFC 6381.

                // TODO: add more, move to h265_stream (?)
                break; // we've seen enough, bailing out.
            }

            if (opt_probe && h->nal->nal_unit_type == NAL_UNIT_PPS)
            {
                // print codec parameter, per RFC 6381.

                // TODO: add more, move to h265_stream (?)
                break; // we've seen enough, bailing out.
            }

            if (opt_verbose > 0)
            {
                // fprintf( h265_dbgfile, "XX ");
                // debug_bytes(p-4, nal_end - nal_start + 4 >= 16 ? 16: nal_end - nal_start + 4);

                // debug_nal(h, h->nal);
            }

            p += (nal_end - nal_start);
            sz -= nal_end;
        }

        // if no NALs found in buffer, discard it
        if (p == buf)
        {
            fprintf(stderr, "!! Did not find any NALs between offset %lld (0x%04llX), size %lld (0x%04llX), discarding \n",
                (long long int)off,
                (long long int)off,
                (long long int)off + sz,
                (long long int)off + sz);

            p = buf + sz;
            sz = 0;
        }

        memmove(buf, p, sz);
        off += p - buf;
        p = buf;
    }

    h265_free(h);
    free(buf);

    fclose(h265_dbgfile);
    fclose(infile);
    //system("PAUSE");
    return 0;
}

