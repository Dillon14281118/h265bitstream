/**
 * @h265_nal.c
 * reading bitstream of H.265
 * @author hanyi <13141211944@163.com>
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "bs.h"
#include "h265_stream.h"

/**
 Create a new H265 stream object.  Allocates all structures contained within it.
 @return    the stream object
 */
h265_stream_t* h265_new()
{
    h265_stream_t* h = (h265_stream_t*)calloc(1, sizeof(h265_stream_t));

    h->nal = (nal_t*)calloc(1, sizeof(nal_t));

    // initialize tables
    for (int i = 0; i < 16; i++) { h->vps_table[i] = (vps_t*)calloc(1, sizeof(vps_t)); }
    for (int i = 0; i < 32; i++) { h->sps_table[i] = (sps_t*)calloc(1, sizeof(sps_t)); }
    for (int i = 0; i < 256; i++) { h->pps_table[i] = (pps_t*)calloc(1, sizeof(pps_t)); }
    h->vps = h->vps_table[0];
    h->sps = h->sps_table[0];
    h->pps = h->pps_table[0];
    h->aud = (aud_t*)calloc(1, sizeof(aud_t));
    h->ptl = (profile_tier_level_t*)calloc(1, sizeof(profile_tier_level_t));
    h->vui = (vui_t*)calloc(1, sizeof(vui_t));
    h->hrd = (hrd_t*)calloc(1, sizeof(hrd_t));
    h->sld = (scaling_list_data_t*)calloc(1, sizeof(scaling_list_data_t));
    h->ssh = (slice_segment_header_t*)calloc(1, sizeof(slice_segment_header_t));
    h->strps = (st_ref_pic_set_t*)calloc(1, sizeof(st_ref_pic_set_t));
    h->slice_data = (slice_data_rbsp_t*)calloc(1, sizeof(slice_data_rbsp_t));
    //h->num_seis = 0;
    //h->seis = NULL;
    //h->sei = NULL;  //This is a TEMP pointer at whats in h->seis...
    //h->sh = (slice_header_t*)calloc(1, sizeof(slice_header_t));
    //h->slice_data = (slice_data_rbsp_t*)calloc(1, sizeof(slice_data_rbsp_t));

    return h;
}


/**
 Free an existing H265 stream object.  Frees all contained structures.
 @param[in,out] h   the stream object
 */
void h265_free(h265_stream_t* h)
{
    free(h->nal);
    for (int i = 0; i < 16; i++) { free(h->vps_table[i]); }
    for (int i = 0; i < 32; i++) { free(h->sps_table[i]); }
    for (int i = 0; i < 256; i++) { free(h->pps_table[i]); }

    free(h->aud);
    free(h->ptl);
    free(h->vui);
    free(h->hrd);
    free(h->sld);
    //if (h->seis != NULL)
    //{
    //    for (int i = 0; i < h->num_seis; i++)
    //    {
    //        sei_t* sei = h->seis[i];
    //        sei_free(sei);
    //    }
    //    free(h->seis);
    //}
    //free(h->sh);
    free(h);
}

/**
 Find the beginning and end of a NAL (Network Abstraction Layer) unit in a byte buffer containing H265 bitstream data.
 @param[in]   buf        the buffer
 @param[in]   size       the size of the buffer
 @param[out]  nal_start  the beginning offset of the nal
 @param[out]  nal_end    the end offset of the nal
 @return                 the length of the nal, or 0 if did not find start of nal, or -1 if did not find end of nal
 */
 // DEPRECATED - this will be replaced by a similar function with a slightly different API
int find_nal_unit(uint8_t* buf, int size, int* nal_start, int* nal_end)
{
    int i;
    // find start
    *nal_start = 0;
    *nal_end = 0;

    i = 0;
    while (   //( next_bits( 24 ) != 0x000001 && next_bits( 32 ) != 0x00000001 )
        (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01) &&
        (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0 || buf[i + 3] != 0x01)
        )
    {
        i++; // skip leading zero
        if (i + 4 >= size) { return 0; } // did not find nal start
    }

    if (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01) // ( next_bits( 24 ) != 0x000001 )
    {
        i++;
    }

    if (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01) { /* error, should never happen */ return 0; }
    i += 3;
    *nal_start = i;

    while (   //( next_bits( 24 ) != 0x000000 && next_bits( 24 ) != 0x000001 )
        (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0) &&
        (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01)
        )
    {
        i++;
        // FIXME the next line fails when reading a nal that ends exactly at the end of the data
        if (i + 3 >= size) { *nal_end = size; return -1; } // did not find nal end, stream ended first
    }

    *nal_end = i;
    return (*nal_end - *nal_start);
}

/**
   Convert RBSP data to NAL data (Annex B format).
   The size of nal_buf must be 3/2 * the size of the rbsp_buf (rounded up) to guarantee the output will fit.
   If that is not true, output may be truncated and an error will be returned.
   If that is true, there is no possible error during this conversion.
   @param[in] rbsp_buf   the rbsp data
   @param[in] rbsp_size  pointer to the size of the rbsp data
   @param[in,out] nal_buf   allocated memory in which to put the nal data
   @param[in,out] nal_size  as input, pointer to the maximum size of the nal data; as output, filled in with the actual size of the nal data
   @return  actual size of nal data, or -1 on error
 */
 // 7.3.1 NAL unit syntax
 // 7.4.1.1 Encapsulation of an SODB within an RBSP
int rbsp_to_nal(const uint8_t* rbsp_buf, const int* rbsp_size, uint8_t* nal_buf, int* nal_size)
{
    int i;
    int j = 1;
    int count = 0;

    if (*nal_size > 0) { nal_buf[0] = 0x00; } // zero out first byte since we start writing from second byte

    for (i = 0; i < *rbsp_size; )
    {
        if (j >= *nal_size)
        {
            // error, not enough space
            return -1;
        }
        //·À³åÍ»
        if ((count == 2) && !(rbsp_buf[i] & 0xFC)) // HACK 0xFC
        {
            nal_buf[j] = 0x03;
            j++;
            count = 0;
            continue;
        }
        nal_buf[j] = rbsp_buf[i];
        if (rbsp_buf[i] == 0x00)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        i++;
        j++;
    }

    *nal_size = j;
    return j;
}

/**
   Convert NAL data (Annex B format) to RBSP data.
   The size of rbsp_buf must be the same as size of the nal_buf to guarantee the output will fit.
   If that is not true, output may be truncated and an error will be returned.
   Additionally, certain byte sequences in the input nal_buf are not allowed in the spec and also cause the conversion to fail and an error to be returned.
   @param[in] nal_buf   the nal data
   @param[in,out] nal_size  as input, pointer to the size of the nal data; as output, filled in with the actual size of the nal data
   @param[in,out] rbsp_buf   allocated memory in which to put the rbsp data
   @param[in,out] rbsp_size  as input, pointer to the maximum size of the rbsp data; as output, filled in with the actual size of rbsp data
   @return  actual size of rbsp data, or -1 on error
 */
 // 7.3.1 NAL unit syntax
 // 7.4.1.1 Encapsulation of an SODB within an RBSP
int nal_to_rbsp(const uint8_t* nal_buf, int* nal_size, uint8_t* rbsp_buf, int* rbsp_size)
{
    int i;
    int j = 0;
    int count = 0;

    for (i = 0; i < *nal_size; i++)
    {
        // in NAL unit, 0x000000, 0x000001 or 0x000002 shall not occur at any byte-aligned position
        if ((count == 2) && (nal_buf[i] < 0x03))
        {
            return -1;
        }

        if ((count == 2) && (nal_buf[i] == 0x03))
        {
            // check the 4th byte after 0x000003, except when cabac_zero_word is used, in which case the last three bytes of this NAL unit must be 0x000003
            if ((i < *nal_size - 1) && (nal_buf[i + 1] > 0x03))
            {
                return -1;
            }

            // if cabac_zero_word is used, the final byte of this NAL unit(0x03) is discarded, and the last two bytes of RBSP must be 0x0000
            if (i == *nal_size - 1)
            {
                break;
            }

            i++;
            count = 0;
        }

        if (j >= *rbsp_size)
        {
            // error, not enough space
            return -1;
        }

        rbsp_buf[j] = nal_buf[i];
        if (nal_buf[i] == 0x00)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        j++;
    }

    *nal_size = i;
    *rbsp_size = j;
    return j;
}
