/**
 * @h265_stream.c
 * reading bitstream of H.265
 * @author hanyi <13141211944@163.com>
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
//#include <math.h>
#include <algorithm>
#include "bs.h"
#include "h265_stream.h"

using namespace std;

FILE* h265_dbgfile = NULL;

#define printf(...) fprintf((h265_dbgfile == NULL ? stdout : h265_dbgfile), __VA_ARGS__)
int more_rbsp_trailing_data(h265_stream_t* h, bs_t* b) { return !bs_eof(b); }
//7.3.2.1 VPS
void read_debug_video_parameter_set_rbsp(h265_stream_t* h, bs_t* b)
{
    int i = 0;
    int j = 0;
    vps_t* vps = h->vps;
    if (1)
    {
        memset(vps, 0, sizeof(vps_t));
    }
    vps->vps_video_parameter_set_id = bs_read_u(b, 4);
    vps->vps_base_layer_internal_flag = bs_read_u1(b);
    vps->vps_base_layer_available_flag = bs_read_u1(b);
    vps->vps_max_layers_minus1 = bs_read_u(b, 6);
    vps->vps_max_sub_layers_minus1 = bs_read_u(b, 3);
    vps->vps_temporal_id_nesting_flag = bs_read_u1(b);
    vps->vps_reserved_0xffff_16bits = bs_read_u(b, 16);
    read_profile_tier_level(h, b, 1, vps->vps_max_sub_layers_minus1);
    vps->vps_sub_layer_ordering_info_present_flag = bs_read_u1(b);
    for (i = (vps->vps_sub_layer_ordering_info_present_flag ? 0 : vps->vps_max_sub_layers_minus1);i <= vps->vps_max_sub_layers_minus1;i++)
    {
        vps->vps_max_dec_pic_buffering_minus1[i] = bs_read_ue(b);
        vps->vps_max_num_reorder_pics[i] = bs_read_ue(b);
        vps->vps_max_latency_increase_plus1[i] = bs_read_ue(b);
    }
    vps->vps_max_layer_id = bs_read_u(b, 6);
    vps->vps_num_layer_sets_minus1 = bs_read_ue(b);
    for (i = 1;i <= vps->vps_num_layer_sets_minus1;i++)
        for (j = 0;j <= vps->vps_max_layer_id;j++)
        {
            vps->layer_id_included_flag[i][j] = bs_read_u1(b);
        }
    vps->vps_timing_info_present_flag = bs_read_u1(b);
    /*
    if(vps->vps_timing_info_present_flag)
    {
        vps->vps_num_units_in_tick = bs_read_u(b, 32);
        vps->vps_time_scale = bs_read_u(b, 32);
        vps->vps_poc_proportional_to_timing_flag = bs_read_u1(b);
        if (vps->vps_poc_proportional_to_timing_flag)
        {
            vps->vps_num_ticks_poc_diff_one_minus1 = bs_read_ue(b);
        }
        vps->vps_num_hrd_parameters = bs_read_ue(b);
        for (i = 0;i < vps->vps_num_hrd_parameters;i++)
        {
            vps->hrd_layer_set_idx[i] = bs_read_ue(b);
            if (i > 0)
            {
                vps->cprms_present_flag[i] = bs_read_u1(b);
            }
            //TODO: hrd-parameters()
        }
    }
    vps->vps_extension_flag = bs_read_u1(b);
    if (vps->vps_extension_flag)
    {
        while (more_rbsp_trailing_data(h, b))
        {
            vps->vps_extension_data_flag = bs_read_u1(b);
        }
    }
    read_debug_rbsp_trailing_bits(h, b);
    */
}
//7.3.2.2 SPS
void read_debug_seq_parameter_set_rbsp(h265_stream_t* h, bs_t* b)
{
    int i = 0;
    sps_t* sps = h->sps;
    if (1)
    {
        memset(sps, 0, sizeof(sps_t));
        sps->chroma_format_idc = 1;
    }
    sps->sps_video_parameter_set_id = bs_read_u(b, 4);
    sps->sps_max_sub_layers_minus1 = bs_read_u(b, 3);
    sps->sps_temporal_id_nesting_flag = bs_read_u1(b);
    //TODO: PTL
    read_profile_tier_level(h, b, 1, sps->sps_max_sub_layers_minus1);

    sps->sps_seq_parameter_set_id = bs_read_ue(b);
    sps->chroma_format_idc = bs_read_ue(b);
    if (sps->chroma_format_idc == 3)
        sps->separate_colour_plane_flag = bs_read_u1(b);
    sps->pic_width_in_luma_samples = bs_read_ue(b);
    sps->pic_height_in_luma_samples = bs_read_ue(b);
    sps->conformance_window_flag = bs_read_u1(b);
    if (sps->conformance_window_flag)
    {
        sps->conf_win_left_offset = bs_read_ue(b);
        sps->conf_win_right_offset = bs_read_ue(b);
        sps->conf_win_top_offse = bs_read_ue(b);
        sps->conf_win_bottom_offset = bs_read_ue(b);
    }
    sps->bit_depth_luma_minus8 = bs_read_ue(b);
    sps->bit_depth_chroma_minus8 = bs_read_ue(b);
    sps->log2_max_pic_order_cnt_lsb_minus4 = bs_read_ue(b);
    sps->sps_sub_layer_ordering_info_present_flag = bs_read_u1(b);
    for (i = (sps->sps_sub_layer_ordering_info_present_flag ? 0 : sps->sps_max_sub_layers_minus1);i <= sps->sps_max_sub_layers_minus1;i++)
    {
        sps->sps_max_dec_pic_buffering_minus1[i] = bs_read_ue(b);
        sps->sps_max_num_reorder_pics[i] = bs_read_ue(b);
        sps->sps_max_latency_increase_plus1[i] = bs_read_ue(b);
    }
    sps->log2_min_luma_coding_block_size_minus3 = bs_read_ue(b);
    sps->log2_diff_max_min_luma_coding_block_size = bs_read_ue(b);
    sps->log2_min_luma_transform_block_size_minus2 = bs_read_ue(b);
    sps->log2_diff_max_min_luma_transform_block_size = bs_read_ue(b);
    sps->max_transform_hierarchy_depth_inter = bs_read_ue(b);
    sps->max_transform_hierarchy_depth_intra = bs_read_ue(b);
    sps->scaling_list_enabled_flag = bs_read_u1(b);
    if (sps->scaling_list_enabled_flag)
    {
        sps->sps_scaling_list_data_present_flag = bs_read_u1(b);
        if (sps->sps_scaling_list_data_present_flag)
        {
            //TODO: scaling_list_data()
            read_scaling_list_data(h, b);
        }
    }
    sps->amp_enabled_flag = bs_read_u1(b);
    sps->sample_adaptive_offset_enabled_flag = bs_read_u1(b);
    sps->pcm_enabled_flag = bs_read_u1(b);
    if (sps->pcm_enabled_flag)
    {
        sps->pcm_sample_bit_depth_luma_minus1 = bs_read_u(b, 4);
        sps->pcm_sample_bit_depth_chroma_minus1 = bs_read_u(b, 4);
        sps->log2_min_pcm_luma_coding_block_size_minus3 = bs_read_ue(b);
        sps->log2_diff_max_min_pcm_luma_coding_block_size = bs_read_ue(b);
        sps->pcm_loop_filter_disabled_flag = bs_read_u1(b);
    }
    sps->num_short_term_ref_pic_sets = bs_read_ue(b);
    /*
    for (i = 0;i < sps->num_short_term_ref_pic_sets;i++)
    {
        //TODO: st_ref_pic_set(i)
    }
    sps->long_term_ref_pics_present_flag = bs_read_u1(b);
    if (sps->long_term_ref_pics_present_flag)
    {
        sps->num_long_term_ref_pics_sps = bs_read_ue(b);
        for (i = 0;i < sps->num_long_term_ref_pics_sps;i++)
        {
            sps->lt_ref_pic_poc_lsb_sps[i] = bs_read_u(b, sps->log2_max_pic_order_cnt_lsb_minus4 + 4);
            sps->used_by_curr_pic_lt_sps_flag[i] = bs_read_u1(b);
        }
    }
    sps->sps_temporal_mvp_enabled_flag = bs_read_u1(b);
    sps->strong_intra_smoothing_enabled_flag = bs_read_u1(b);
    sps->vui_parameters_present_flag = bs_read_u1(b);
    if (sps->vui_parameters_present_flag)
    {
        //TODO: vui_parameters()
    }
    sps->sps_extension_present_flag = bs_read_u1(b);
    if (sps->sps_extension_present_flag)
    {
        sps->sps_range_extension_flag = bs_read_u1(b);
        sps->sps_multilayer_extension_flag = bs_read_u1(b);
        sps->sps_3d_extension_flag = bs_read_u1(b);
        sps->sps_scc_extension_flag = bs_read_u1(b);
        sps->sps_extension_4bits = bs_read_u(b, 4);
    }
    if (sps->sps_range_extension_flag)
    {
        //TODO: sps_range_extension()
    }
    if (sps->sps_multilayer_extension_flag)
    {
        //TODO: sps_multilayer_extension()
    }
    if (sps->sps_3d_extension_flag)
    {
        //TODO: sps_3d_extension()
    }
    if (sps->sps_scc_extension_flag)
    {
        //TODO: sps_scc_extension()
    }
    if (sps->sps_extension_4bits)
    {
    }
    */
}
//7.3.2.3 PPS
void read_debug_pic_parameter_set_rbsp(h265_stream_t* h, bs_t* b)
{
    int i = 0;
    pps_t* pps = h->pps;
    if (1)
    {
        memset(pps, 0, sizeof(pps_t));
    }
    pps->pps_pic_parameter_set_id = bs_read_ue(b);
    pps->pps_seq_parameter_set_id = bs_read_ue(b);
    pps->dependent_slice_segments_enabled_flag = bs_read_u1(b);
    pps->output_flag_present_flag = bs_read_u1(b);
    pps->num_extra_slice_header_bits = bs_read_u(b, 3);
    pps->sign_data_hiding_enabled_flag = bs_read_u1(b);
    pps->cabac_init_present_flag = bs_read_u1(b);
    pps->num_ref_idx_l0_default_active_minus1 = bs_read_ue(b);
    pps->num_ref_idx_l1_default_active_minus1 = bs_read_ue(b);
    pps->init_qp_minus26 = bs_read_se(b);
    pps->constrained_intra_pred_flag = bs_read_u1(b);
    pps->transform_skip_enabled_flag = bs_read_u1(b);
    pps->cu_qp_delta_enabled_flag = bs_read_u1(b);
    if (pps->cu_qp_delta_enabled_flag)
    {
        pps->diff_cu_qp_delta_depth = bs_read_ue(b);
    }
    pps->pps_cb_qp_offset = bs_read_se(b);
    pps->pps_cr_qp_offset = bs_read_se(b);
    pps->pps_slice_chroma_qp_offsets_present_flag = bs_read_u1(b);
    pps->weighted_pred_flag = bs_read_u1(b);
    pps->weighted_bipred_flag = bs_read_u1(b);
    pps->transquant_bypass_enabled_flag = bs_read_u1(b);
    pps->tiles_enabled_flag = bs_read_u1(b);
    pps->entropy_coding_sync_enabled_flag = bs_read_u1(b);
    if (pps->tiles_enabled_flag)
    {
        pps->num_tile_columns_minus1 = bs_read_ue(b);
        pps->num_tile_rows_minus1 = bs_read_ue(b);
        //FIXME
        pps->column_width_minus1 = (int*)malloc(sizeof(int) * pps->num_tile_columns_minus1);
        pps->row_height_minus1 = (int*)malloc(sizeof(int) * pps->num_tile_rows_minus1);
        pps->uniform_spacing_flag = bs_read_u1(b);
        if (!pps->uniform_spacing_flag)
        {
            for (i = 0;i < pps->num_tile_columns_minus1;i++)
            {
                pps->column_width_minus1[i] = bs_read_ue(b);
            }
            for (i = 0;i < pps->num_tile_rows_minus1;i++)
            {
                pps->row_height_minus1[i] = bs_read_ue(b);
            }
        }
        pps->loop_filter_across_tiles_enabled_flag = bs_read_u1(b);
    }
    pps->pps_loop_filter_across_slices_enabled_flag = bs_read_u1(b);
    pps->deblocking_filter_control_present_flag = bs_read_u1(b);
    if (pps->deblocking_filter_control_present_flag)
    {
        pps->deblocking_filter_override_enabled_flag = bs_read_u1(b);
        pps->pps_deblocking_filter_disabled_flag = bs_read_u1(b);
        if (!pps->pps_deblocking_filter_disabled_flag)
        {
            pps->pps_beta_offset_div2 = bs_read_se(b);
            pps->pps_tc_offset_div2 = bs_read_se(b);
        }
    }
    pps->pps_scaling_list_data_present_flag = bs_read_u1(b);

    if (pps->pps_scaling_list_data_present_flag)
    {
        read_scaling_list_data(h, b);
    }
    pps->lists_modification_present_flag = bs_read_u1(b);
    pps->log2_parallel_merge_level_minus2 = bs_read_ue(b);
    pps->slice_segment_header_extension_present_flag = bs_read_u1(b);
    pps->pps_extension_present_flag = bs_read_u1(b);
    if (pps->pps_extension_present_flag)
    {
        pps->pps_range_extension_flag = bs_read_u1(b);
        pps->pps_multilayer_extension_flag = bs_read_u1(b);
        pps->pps_3d_extension_flag = bs_read_u1(b);
        pps->pps_scc_extension_flag = bs_read_u1(b);
        pps->pps_extension_4bits = bs_read_u(b, 4);
    }
    if (pps->pps_range_extension_flag)
    {
        //FIXME pps_range_extension
    }
    if (pps->pps_multilayer_extension_flag)
    {

    }
    if (pps->pps_3d_extension_flag)
    {
        //FIXME pps_multilayer_extension
    }
    if (pps->pps_scc_extension_flag)
    {
        //FIXME pps_scc_extension
    }
    if (pps->pps_extension_4bits)
    {
        //FIXME
    }
}
void read_debug_vui_parameters(h265_stream_t* h, bs_t* b)
{}
void read_debug_hrd_parameters(h265_stream_t* h, bs_t* b)
{}

void read_access_unit_delimiter_rbsp(h265_stream_t* h, bs_t* b)
{}
//7.3.7  Short-term reference picture set syntax
void read_st_ref_pic_set(h265_stream_t* h, bs_t* b, int  stRpsIdx)
{
    int i = 0;
    int j = 0;
    int RefRpsIdx = 0;
    st_ref_pic_set_t* strps = h->strps;
    sps_t* sps = h->sps;
    if (1)
    {
        memset(strps, 0, sizeof(st_ref_pic_set_t));
    }
    if (stRpsIdx != 0)
    {
        strps->inter_ref_pic_set_prediction_flag = bs_read_u1(b);
    }
    if (strps->inter_ref_pic_set_prediction_flag)
    {
        if (stRpsIdx == sps->num_short_term_ref_pic_sets)
        {
            strps->delta_idx_minus1 = bs_read_ue(b);
            RefRpsIdx = stRpsIdx - (strps->delta_idx_minus1 + 1);
        }
        strps->delta_rps_sign = bs_read_u1(b);
        strps->abs_delta_rps_minus1 = bs_read_ue(b);
        //FIXME
    }
    else {
        strps->num_negative_pics = bs_read_ue(b);
        strps->num_positive_pics = bs_read_ue(b);
        for (i = 0;i < strps->num_negative_pics;i++)
        {
            strps->delta_poc_s0_minus1[i] = bs_read_ue(b);
            strps->used_by_curr_pic_s0_flag[i] = bs_read_u1(b);
        }
        for (i = 0;i < strps->num_positive_pics;i++)
        {
            strps->delta_poc_s1_minus1[i] = bs_read_ue(b);
            strps->used_by_curr_pic_s1_flag[i] = bs_read_u1(b);
        }
    }
}

//7.3.6.2 Reference picture list modification syntax
//ref_pic_list_modification()
void read_ref_pic_lists_modification(h265_stream_t* h, bs_t* b)
{
    int i = 0;
    slice_segment_header_t* ssh = h->ssh;
    ssh->rplm.ref_pic_list_modification_flag_l0 = bs_read_u1(b);
    if (ssh->rplm.ref_pic_list_modification_flag_l0)
    {
        for (i = 0;i <= ssh->num_ref_idx_l0_active_minus1;i++)
        {
            //FIXME
        }
    }
}
//7.3.3
void read_profile_tier_level(h265_stream_t* h, bs_t* b, int profilePresentFlag, int maxNumSubLayersMinus1)
{
    int i = 0;
    int j = 0;
    profile_tier_level_t* ptl = h->ptl;
    if (1)
    {
        memset(ptl, 0, sizeof(profile_tier_level_t));
    }
    if (profilePresentFlag)
    {
        ptl->general_profile_space = bs_read_u(b, 2);
        ptl->general_tier_flag = bs_read_u1(b);
        ptl->general_profile_idc = bs_read_u(b, 5);
        for (j = 0;j < 32;j++)
        {
            ptl->general_profile_compatibility_flag[j] = bs_read_u1(b);
        }
        ptl->general_progressive_source_flag = bs_read_u1(b);
        ptl->general_interlaced_source_flag = bs_read_u1(b);
        ptl->general_non_packed_constraint_flag = bs_read_u1(b);
        ptl->general_frame_only_constraint_flag = bs_read_u1(b);
        if ((ptl->general_profile_idc == 4) || (ptl->general_profile_idc == 5) || (ptl->general_profile_idc == 6) ||
            (ptl->general_profile_idc == 7) || (ptl->general_profile_idc == 8) || (ptl->general_profile_idc == 9) ||
            (ptl->general_profile_idc == 10) || ptl->general_profile_compatibility_flag[4] ||
            ptl->general_profile_compatibility_flag[5] || ptl->general_profile_compatibility_flag[6] ||
            ptl->general_profile_compatibility_flag[7] || ptl->general_profile_compatibility_flag[8] ||
            ptl->general_profile_compatibility_flag[9] || ptl->general_profile_compatibility_flag[10])
        {
            ptl->general_max_12bit_constraint_flag = bs_read_u1(b);
            ptl->general_max_10bit_constraint_flag = bs_read_u1(b);
            ptl->general_max_8bit_constraint_flag = bs_read_u1(b);
            ptl->general_max_422chroma_constraint_flag = bs_read_u1(b);
            ptl->general_max_420chroma_constraint_flag = bs_read_u1(b);
            ptl->general_max_monochrome_constraint_flag = bs_read_u1(b);
            ptl->general_intra_constraint_flag = bs_read_u1(b);
            ptl->general_one_picture_only_constraint_flag = bs_read_u1(b);
            ptl->general_lower_bit_rate_constraint_flag = bs_read_u1(b);
            if ((ptl->general_profile_idc == 5) || (ptl->general_profile_idc == 9) || (ptl->general_profile_idc == 10) ||
                ptl->general_profile_compatibility_flag[5] || ptl->general_profile_compatibility_flag[9] ||
                ptl->general_profile_compatibility_flag[10])
            {
                ptl->general_max_14bit_constraint_flag = bs_read_u1(b);
                ptl->general_reserved_zero_33bits = bs_read_u(b, 33);
            }
            else {
                ptl->general_reserved_zero_34bits = bs_read_u(b, 34);
            }
        }
        else if ((ptl->general_profile_idc == 2) || ptl->general_profile_compatibility_flag[2])
        {
            ptl->general_reserved_zero_7bits = bs_read_u(b, 7);
            ptl->general_one_picture_only_constraint_flag = bs_read_u1(b);
            ptl->general_reserved_zero_35bits = bs_read_u(b, 35);
        }
        else {
            ptl->general_reserved_zero_43bits = bs_read_u(b, 43);
        }
        if (((ptl->general_profile_idc >= 1) && (ptl->general_profile_idc <= 5)) || (ptl->general_profile_idc == 9) ||
            ptl->general_profile_compatibility_flag[1] || ptl->general_profile_compatibility_flag[2] ||
            ptl->general_profile_compatibility_flag[3] || ptl->general_profile_compatibility_flag[4] ||
            ptl->general_profile_compatibility_flag[5] || ptl->general_profile_compatibility_flag[9])
        {
            ptl->general_inbld_flag = bs_read_u1(b);
        }
        else {
            ptl->general_reserved_zero_bit = bs_read_u1(b);
        }
    }
    ptl->general_level_idc = bs_read_u8(b);
    for (i = 0;i < maxNumSubLayersMinus1;i++)
    {
        ptl->sub_layer_profile_present_flag[i] = bs_read_u1(b);
        ptl->sub_layer_level_present_flag[i] = bs_read_u1(b);
    }
    if (maxNumSubLayersMinus1 > 0)
    {
        for (i = maxNumSubLayersMinus1;i < 8;i++)
        {
            ptl->reserved_zero_2bits[i] = bs_read_u(b, 2);
        }
    }
    for (i = 0;i < maxNumSubLayersMinus1;i++)
    {
        if (ptl->sub_layer_profile_present_flag[i])
        {
            ptl->sub_layer_profile_space[i] = bs_read_u(b, 2);
            ptl->sub_layer_tier_flag[i] = bs_read_u1(b);
            ptl->sub_layer_profile_idc[i] = bs_read_u(b, 5);
            for (j = 0;j < 32;j++)
            {
                ptl->sub_layer_profile_compatibility_flag[i][j] = bs_read_u1(b);
            }
            ptl->sub_layer_progressive_source_flag[i] = bs_read_u1(b);
            ptl->sub_layer_interlaced_source_flag[i] = bs_read_u1(b);
            ptl->sub_layer_non_packed_constraint_flag[i] = bs_read_u1(b);
            ptl->sub_layer_frame_only_constraint_flag[i] = bs_read_u1(b);
            if (ptl->sub_layer_profile_idc[i] == 4 || ptl->sub_layer_profile_compatibility_flag[i][4] ||
                ptl->sub_layer_profile_idc[i] == 5 || ptl->sub_layer_profile_compatibility_flag[i][5] ||
                ptl->sub_layer_profile_idc[i] == 6 || ptl->sub_layer_profile_compatibility_flag[i][6] ||
                ptl->sub_layer_profile_idc[i] == 7 || ptl->sub_layer_profile_compatibility_flag[i][7] ||
                ptl->sub_layer_profile_idc[i] == 8 || ptl->sub_layer_profile_compatibility_flag[i][8] ||
                ptl->sub_layer_profile_idc[i] == 9 || ptl->sub_layer_profile_compatibility_flag[i][9] ||
                ptl->sub_layer_profile_idc[i] == 10 || ptl->sub_layer_profile_compatibility_flag[i][10])
            {
                ptl->sub_layer_max_12bit_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_max_10bit_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_max_8bit_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_max_422chroma_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_max_420chroma_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_max_monochrome_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_intra_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_one_picture_only_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_lower_bit_rate_constraint_flag[i] = bs_read_u1(b);
                if (ptl->sub_layer_profile_idc[i] == 5 || ptl->sub_layer_profile_compatibility_flag[i][5])
                {
                    ptl->sub_layer_max_14bit_constraint_flag[i] = bs_read_u1(b);
                    ptl->sub_layer_reserved_zero_33bits[i] = bs_read_u(b, 33);
                }
                else {
                    ptl->sub_layer_reserved_zero_34bits[i] = bs_read_u(b, 34);
                }
            }
            else if (ptl->sub_layer_profile_idc[i] == 2 || ptl->sub_layer_profile_compatibility_flag[i][2])
            {
                ptl->sub_layer_reserved_zero_7bits[i] = bs_read_u(b, 7);
                ptl->sub_layer_one_picture_only_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_reserved_zero_35bits[i] = bs_read_u(b, 35);
            }
            else {
                ptl->sub_layer_reserved_zero_43bits[i] = bs_read_u(b, 43);
            }
            if ((ptl->sub_layer_profile_idc[i] >= 1 && ptl->sub_layer_profile_idc[i] <= 5) ||
                ptl->sub_layer_profile_idc[i] == 9 ||
                ptl->sub_layer_profile_compatibility_flag[1] ||
                ptl->sub_layer_profile_compatibility_flag[2] ||
                ptl->sub_layer_profile_compatibility_flag[3] ||
                ptl->sub_layer_profile_compatibility_flag[4] ||
                ptl->sub_layer_profile_compatibility_flag[5] ||
                ptl->sub_layer_profile_compatibility_flag[9])
            {
                ptl->sub_layer_inbld_flag[i] = bs_read_u1(b);
            }
            else {
                ptl->sub_layer_reserved_zero_bit[i] = bs_read_u1(b);
            }
        }
        if (ptl->sub_layer_level_present_flag[i])
        {
            ptl->sub_layer_level_idc[i] = bs_read_u8(b);
        }
    }
}
//7.3.4
void read_scaling_list_data(h265_stream_t* h, bs_t* b)
{
    int i = 0;
    int sizeId = 0;
    int matrixId = 0;
    int nextCoef = 0;
    int coefNum = 0;
    scaling_list_data_t* sld = h->sld;
    if (1)
    {
        memset(sld, 0, sizeof(scaling_list_data_t));
    }
    for (sizeId = 0;sizeId < 4;sizeId++)
        for (matrixId = 0;matrixId < 6;matrixId += ((sizeId == 3) ? 3 : 1))
        {
            sld->scaling_list_pred_mode_flag[sizeId][matrixId] = bs_read_u1(b);
            if (!sld->scaling_list_pred_mode_flag[sizeId][matrixId])
            {
                sld->scaling_list_pred_matrix_id_delta[sizeId][matrixId] = bs_read_ue(b);
            }
            else {
                nextCoef = 8;
                coefNum = min(64, (1 << (4 + (sizeId << 1))));
                if (sizeId > 1)
                {
                    sld->scaling_list_dc_coef_minus8[sizeId - 2][matrixId] = bs_read_se(b);
                    nextCoef = sld->scaling_list_dc_coef_minus8[sizeId - 2][matrixId] + 8;
                }
                for (i = 0;i < coefNum;i++)
                {
                    sld->scaling_list_delta_coef = bs_read_se(b);
                    nextCoef = (nextCoef + sld->scaling_list_delta_coef + 256) % 256;
                    //FIXME
                    //ScalingList[ sizeId ][ matrixId ][ i ] = nextCoef
                }
            }
        }
}
//7.3.2.10 RBSP slice trailing bits syntax rbsp_slice_segment_trailing_bits( )
void read_debug_rbsp_slice_trailing_bits(h265_stream_t* h, bs_t* b)
{
    read_debug_rbsp_trailing_bits(h, b);
    while (more_rbsp_trailing_data(h, b))
    {
        printf("%d.%d: ", b->p - b->start, b->bits_left); int cabac_zero_word = bs_read_u(b, 16); printf("cabac_zero_word: %d \n", cabac_zero_word);
    }
}

//7.3.2.11 RBSP trailing bits syntax rbsp_trailing_bits( ) 
void read_debug_rbsp_trailing_bits(h265_stream_t* h, bs_t* b)
{
    printf("%d.%d: ", b->p - b->start, b->bits_left); int rbsp_stop_one_bit = bs_read_u(b, 1); printf("rbsp_stop_one_bit: %d \n", rbsp_stop_one_bit);

    while (!bs_byte_aligned(b))
    {
        printf("%d.%d: ", b->p - b->start, b->bits_left); int rbsp_alignment_zero_bit = bs_read_u(b, 1); printf("rbsp_alignment_zero_bit: %d \n", rbsp_alignment_zero_bit);
    }
}

//7.3.6.1 slice_segment_header()
void read_slice_segment_header(h265_stream_t* h, bs_t* b)
{
    int i = 0;
    slice_segment_header_t* ssh = h->ssh;
    nal_t* nal = h->nal;
    pps_t* pps = h->pps;
    sps_t* sps = h->sps;
    int MinCbLog2SizeY = sps->log2_min_luma_coding_block_size_minus3 + 3; //7-10
    int CtbLog2SizeY = MinCbLog2SizeY + sps->log2_diff_max_min_luma_coding_block_size; //7-11
    int CtbSizeY = 1 << CtbLog2SizeY; //7-13
    int PicWidthInCtbsY = ceil(sps->pic_width_in_luma_samples / CtbSizeY); //7-15
    int PicHeightInCtbsY = ceil(sps->pic_height_in_luma_samples / CtbSizeY); //7-17
    int PicSizeInCtbsY = PicWidthInCtbsY * PicHeightInCtbsY; //7-19
    if (1)
    {
        memset(ssh, 0, sizeof(slice_segment_header_t));
    }
    ssh->first_slice_segment_in_pic_flag = bs_read_u1(b);
    if ((nal->nal_unit_type >= NAL_UNIT_CODED_SLICE_BLA_W_LP) && (nal->nal_unit_type <= NAL_UNIT_RESERVED_IRAP_VCL23))
    {
        ssh->no_output_of_prior_pics_flag = bs_read_u1(b);
    }
    ssh->slice_pic_parameter_set_id = bs_read_ue(b);
    if (!ssh->first_slice_segment_in_pic_flag)
    {
        if (pps->dependent_slice_segments_enabled_flag)
        {
            ssh->dependent_slice_segment_flag = bs_read_u1(b);
        }
        ssh->slice_segment_address = bs_read_u(b, (int)(ceil(log2(PicSizeInCtbsY))));
    }

    if (!ssh->dependent_slice_segment_flag)
    {
        for (i = 0;i < pps->num_extra_slice_header_bits;i++)
        {
            ssh->slice_reserved_flag[i] = bs_read_u1(b);
        }
        ssh->slice_type = bs_read_ue(b);
        if (pps->output_flag_present_flag)
        {
            ssh->pic_output_flag = bs_read_u1(b);
        }
        if (sps->separate_colour_plane_flag == 1)
        {
            ssh->colour_plane_id = bs_read_u(b, 2);
        }
        if ((nal->nal_unit_type != NAL_UNIT_CODED_SLICE_IDR_W_RADL) && (nal->nal_unit_type != NAL_UNIT_CODED_SLICE_IDR_N_LP))
        {
            ssh->slice_pic_order_cnt_lsb = bs_read_u(b, (sps->log2_max_pic_order_cnt_lsb_minus4 + 4));
            ssh->short_term_ref_pic_set_sps_flag = bs_read_u1(b);
            /*
            if (!ssh->short_term_ref_pic_set_sps_flag)
            {
                //TODO st_ref_pic_set( num_short_term_ref_pic_sets )
            }
            else if (sps->num_short_term_ref_pic_sets > 1)
            {
                ssh->short_term_ref_pic_set_idx = bs_read_u(b, (sps->num_short_term_ref_pic_sets));
            }
            if (sps->long_term_ref_pics_present_flag)
            {
                if (sps->num_long_term_ref_pics_sps > 0)
                {
                    ssh->num_long_term_sps = bs_read_ue(b);
                }
                ssh->num_long_term_pics = bs_read_ue(b);
                for (i = 0;i < (ssh->num_long_term_sps + ssh->num_long_term_pics);i++)
                {
                    if (i < ssh->num_long_term_sps)
                    {
                        if (sps->num_long_term_ref_pics_sps > 1)
                        {
                            ssh->lt_idx_sps[i] = bs_read_u(b, (sps->num_long_term_ref_pics_sps));
                        }
                    }
                    else {
                        ssh->poc_lsb_lt[i] = bs_read_u(b, (sps->log2_max_pic_order_cnt_lsb_minus4+4));
                        ssh->used_by_curr_pic_lt_flag[i] = bs_read_u1(b);
                    }
                    ssh->delta_poc_msb_present_flag[i] = bs_read_u1(b);
                    if (ssh->delta_poc_msb_present_flag[i])
                    {
                        ssh->delta_poc_msb_cycle_lt[i] = bs_read_ue(b);
                    }
                }
            }
            if (sps->sps_temporal_mvp_enabled_flag)
            {
                ssh->slice_temporal_mvp_enabled_flag = bs_read_u1(b);
            }
            */
        }
        /*
        if (sps->sample_adaptive_offset_enabled_flag)
        {
            ssh->slice_sao_luma_flag = bs_read_u1(b);
            if (sps->chroma_format_idc != 0) //FIXME ChromaArrayType may differ from chroma_format_idc
            {
                ssh->slice_sao_chroma_flag = bs_read_u1(b);
            }
        }
        if ((ssh->slice_type == SH_SLICE_TYPE_P) || (ssh->slice_type == SH_SLICE_TYPE_B))
        {
            ssh->num_ref_idx_active_override_flag = bs_read_u1(b);
            if (ssh->num_ref_idx_active_override_flag)
            {
                ssh->num_ref_idx_l0_active_minus1 = bs_read_ue(b);
                if (ssh->slice_type == SH_SLICE_TYPE_B)
                {
                    ssh->num_ref_idx_l1_active_minus1 = bs_read_ue(b);
                }
            }
            //TODO ref_pic_lists_modification( )
            if (ssh->slice_type == SH_SLICE_TYPE_B)
            {
                ssh->mvd_l1_zero_flag = bs_read_u1(b);
            }
            if(pps->cabac_init_present_flag)
                ssh->cabac_init_flag = bs_read_u1(b);
            if (ssh->slice_temporal_mvp_enabled_flag)
            {
                if(ssh->slice_type == SH_SLICE_TYPE_B)
                    ssh->collocated_from_l0_flag = bs_read_u1(b);
                if((ssh->collocated_from_l0_flag && ssh->num_ref_idx_l0_active_minus1>0) ||
                    (!ssh->collocated_from_l0_flag && ssh->num_ref_idx_l1_active_minus1>0))
                    ssh->collocated_ref_idx = bs_read_ue(b);
            }
            if ((pps->weighted_pred_flag && (ssh->slice_type == SH_SLICE_TYPE_P)) ||
                (pps->weighted_bipred_flag && (ssh->slice_type == SH_SLICE_TYPE_B)))
            {
                //TODO pred_weight_table( )
            }
            ssh->five_minus_max_num_merge_cand = bs_read_ue(b);
            //TODO use_integer_mv_flag
        }
        ssh->slice_qp_delta = bs_read_se(b);
        if (pps->pps_slice_chroma_qp_offsets_present_flag)
        {
            ssh->slice_cb_qp_offset = bs_read_se(b);
            ssh->slice_cr_qp_offset = bs_read_se(b);
        }
        //TODO pps_slice_act_qp_offsets_present_flag
        //TODO chroma_qp_offset_list_enabled_flag
        if(pps->deblocking_filter_override_enabled_flag)
            ssh->deblocking_filter_override_flag = bs_read_u1(b);
        if (ssh->deblocking_filter_override_flag)
        {
            ssh->slice_deblocking_filter_disabled_flag = bs_read_u1(b);
            if (!ssh->slice_deblocking_filter_disabled_flag)
            {
                ssh->slice_beta_offset_div2 = bs_read_se(b);
                ssh->slice_tc_offset_div2 = bs_read_se(b);
            }
        }
        if(pps->pps_loop_filter_across_slices_enabled_flag && (ssh->slice_sao_luma_flag || ssh->slice_sao_chroma_flag ||
            !ssh->slice_deblocking_filter_disabled_flag))
            ssh->slice_loop_filter_across_slices_enabled_flag = bs_read_u1(b);
        */
    }
    /*
    if (pps->tiles_enabled_flag || pps->entropy_coding_sync_enabled_flag)
    {
        ssh->num_entry_point_offsets = bs_read_ue(b);
        if (ssh->num_entry_point_offsets > 0)
        {
            ssh->offset_len_minus1 = bs_read_ue(b);
            for (i = 0;i < ssh->num_entry_point_offsets;i++)
            {
                //TODO  ssh->entry_point_offset_minus1[i]
            }
        }
    }
    if (pps->slice_segment_header_extension_present_flag)
    {
        ssh->slice_segment_header_extension_length = bs_read_ue(b);
        for (i = 0;i < ssh->slice_segment_header_extension_length;i++)
            ssh->slice_segment_header_extension_data_byte[i] = bs_read_u(b, 8);
    }
    */
    //TODO byte_alignment( )
}

void read_debug_slice_rbsp(h265_stream_t* h, bs_t* b)
{
    read_slice_segment_header(h, b);

    //TODO slice data
}

//7.3.1 NAL unit syntax
int read_debug_nal_unit(h265_stream_t* h, uint8_t* buf, int size)
{
    nal_t* nal = h->nal;

    int nal_size = size;
    int rbsp_size = size;
    uint8_t* rbsp_buf = (uint8_t*)calloc(1, rbsp_size);

    if (1)
    {
        int rc = nal_to_rbsp(buf, &nal_size, rbsp_buf, &rbsp_size);

        if (rc < 0) { free(rbsp_buf); return -1; } // handle conversion error
    }

    if (0)
    {
        rbsp_size = size * 3 / 4; // NOTE this may have to be slightly smaller (3/4 smaller, worst case) in order to be guaranteed to fit
    }

    bs_t* b = bs_new(rbsp_buf, rbsp_size);
    //7.3.1.2 NALU header
    //printf("%d.%d: ", b->p - b->start, b->bits_left); 
    nal->forbidden_zero_bit = bs_read_u(b, 1); printf("forbidden_zero_bit: %d \n", nal->forbidden_zero_bit);
    //printf("%d.%d: ", b->p - b->start, b->bits_left); 
    nal->nal_unit_type = bs_read_u(b, 6); printf("nal->nal_unit_type: %d \n", nal->nal_unit_type);
    //printf("%d.%d: ", b->p - b->start, b->bits_left); 
    nal->nuh_layer_id = bs_read_u(b, 6); printf("nal->nuh_layer_id: %d \n", nal->nuh_layer_id);
    //printf("%d.%d: ", b->p - b->start, b->bits_left); 
    nal->nuh_temporal_id_plus1 = bs_read_u(b, 3); printf("nal->nuh_temporal_id_plus1: %d \n", nal->nuh_temporal_id_plus1);

    switch (nal->nal_unit_type)
    {
    case NAL_UNIT_CODED_SLICE_TRAIL_R:
    case NAL_UNIT_CODED_SLICE_TRAIL_N:
    case NAL_UNIT_CODED_SLICE_TSA_R:
    case NAL_UNIT_CODED_SLICE_TSA_N:
    case NAL_UNIT_CODED_SLICE_STSA_R:
    case NAL_UNIT_CODED_SLICE_STSA_N:
    case NAL_UNIT_CODED_SLICE_BLA_W_LP:
    case NAL_UNIT_CODED_SLICE_BLA_W_RADL:
    case NAL_UNIT_CODED_SLICE_BLA_N_LP:
    case NAL_UNIT_CODED_SLICE_IDR_W_RADL:
    case NAL_UNIT_CODED_SLICE_IDR_N_LP:
    case NAL_UNIT_CODED_SLICE_CRA:
    case NAL_UNIT_CODED_SLICE_RADL_N:
    case NAL_UNIT_CODED_SLICE_RADL_R:
    case NAL_UNIT_CODED_SLICE_RASL_N:
    case NAL_UNIT_CODED_SLICE_RASL_R:
        read_debug_slice_rbsp(h, b);
        printSH(h);
        break;
#ifdef HAVE_SEI
    case NAL_UNIT_PREFIX_SEI:
        read_debug_sei_rbsp(h, b);
        break;
#endif

    case NAL_UNIT_VPS:
        read_debug_video_parameter_set_rbsp(h, b);
        printVPS(h);
        break;

    case NAL_UNIT_SPS:
        read_debug_seq_parameter_set_rbsp(h, b);
        printSPS(h);
        break;

    case NAL_UNIT_PPS:
        read_debug_pic_parameter_set_rbsp(h, b);
        printPPS(h);
        break;

    default:
        return -1;
    }

    if (bs_overrun(b)) { bs_free(b); free(rbsp_buf); return -1; }

    if (0)
    {
        // now get the actual size used
        rbsp_size = bs_pos(b);

        int rc = rbsp_to_nal(rbsp_buf, &rbsp_size, buf, &nal_size);
        if (rc < 0) { bs_free(b); free(rbsp_buf); return -1; }
    }

    bs_free(b);
    free(rbsp_buf);

    return nal_size;
}


void printVPS(h265_stream_t* h)
{
    vps_t* vps = h->vps;
    printf("VPS:\n");
    printf("vps_video_parameter_set_id:%d\n", vps->vps_video_parameter_set_id);
    printf("vps_base_layer_internal_flag:%d\n", vps->vps_base_layer_internal_flag);
    printf("vps_base_layer_available_flag:%d\n", vps->vps_base_layer_available_flag);
    printf("vps_max_layers_minus1:%d\n", vps->vps_max_layers_minus1);
    printf("vps_max_sub_layers_minus1:%d\n", vps->vps_max_sub_layers_minus1);
    printf("vps_temporal_id_nesting_flag:%d\n", vps->vps_temporal_id_nesting_flag);
    printPTL(h);
    printf("vps_sub_layer_ordering_info_present_flag:%d\n", vps->vps_sub_layer_ordering_info_present_flag);
    printf("vps_max_layer_id:%d\n", vps->vps_max_layer_id);
    printf("vps_num_layer_sets_minus1:%d\n", vps->vps_num_layer_sets_minus1);
    printf("vps_timing_info_present_flag:%d\n", vps->vps_timing_info_present_flag);
}
void printSPS(h265_stream_t* h)
{
    sps_t* sps = h->sps;
    printf("SPS:\n");
    printf("sps_video_parameter_set_id:%d\n", sps->sps_video_parameter_set_id);
    printf("sps_max_sub_layers_minus1:%d\n", sps->sps_max_sub_layers_minus1);
    printf("sps_temporal_id_nesting_flag:%d\n", sps->sps_temporal_id_nesting_flag);
    printPTL(h);
    printf("sps_seq_parameter_set_id:%d\n", sps->sps_seq_parameter_set_id);
    printf("chroma_format_idc:%d\n", sps->chroma_format_idc);
    printf("pic_width_in_luma_samples:%d\n", sps->pic_width_in_luma_samples);
    printf("pic_height_in_luma_samples:%d\n", sps->pic_height_in_luma_samples);
    printf("conformance_window_flag:%d\n", sps->conformance_window_flag);
}
void printPPS(h265_stream_t* h)
{
    pps_t* pps = h->pps;
    printf("PPS:\n");
    printf("pps_pic_parameter_set_id:%d\n", pps->pps_pic_parameter_set_id);
    printf("pps_seq_parameter_set_id:%d\n", pps->pps_seq_parameter_set_id);
    printf("dependent_slice_segments_enabled_flag:%d\n", pps->dependent_slice_segments_enabled_flag);
    printf("output_flag_present_flag:%d\n", pps->output_flag_present_flag);
    printf("num_extra_slice_header_bits:%d\n", pps->num_extra_slice_header_bits);
    printf("sign_data_hiding_enabled_flag:%d\n", pps->sign_data_hiding_enabled_flag);
    printf("cabac_init_present_flag:%d\n", pps->cabac_init_present_flag);
    printf("num_ref_idx_l0_default_active_minus1:%d\n", pps->num_ref_idx_l0_default_active_minus1);
    printf("num_ref_idx_l1_default_active_minus1:%d\n", pps->num_ref_idx_l1_default_active_minus1);
    printf("init_qp_minus26:%d\n", pps->init_qp_minus26);
    printf("constrained_intra_pred_flag:%d\n", pps->constrained_intra_pred_flag);
    printf("transform_skip_enabled_flag:%d\n", pps->transform_skip_enabled_flag);
    printf("cu_qp_delta_enabled_flag:%d\n", pps->cu_qp_delta_enabled_flag);

    printf("pps_cb_qp_offset:%d\n", pps->pps_cb_qp_offset);
    printf("pps_cr_qp_offset:%d\n", pps->pps_cr_qp_offset);
    printf("pps_slice_chroma_qp_offsets_present_flag:%d\n", pps->pps_slice_chroma_qp_offsets_present_flag);
    printf("weighted_pred_flag:%d\n", pps->weighted_pred_flag);
    printf("weighted_bipred_flag:%d\n", pps->weighted_bipred_flag);
    printf("transquant_bypass_enabled_flag:%d\n", pps->transquant_bypass_enabled_flag);
    printf("tiles_enabled_flag:%d\n", pps->tiles_enabled_flag);
    printf("entropy_coding_sync_enabled_flag:%d\n", pps->entropy_coding_sync_enabled_flag);
    if (pps->tiles_enabled_flag)
    {
        printf("num_tile_columns_minus1:%d\n", pps->num_tile_columns_minus1);
        printf("num_tile_rows_minus1:%d\n", pps->num_tile_rows_minus1);
        printf("uniform_spacing_flag:%d\n", pps->uniform_spacing_flag);
    }
}
void printPTL(h265_stream_t* h)
{
    int j;
    profile_tier_level_t* ptl = h->ptl;
    printf("profile_tier_level( 1, vps_max_sub_layers_minus1 ):\n");
    printf("  general_profile_space:%d\n", ptl->general_profile_space);
    printf("  general_tier_flag:%d\n", ptl->general_tier_flag);
    printf("  general_profile_idc:%d\n", ptl->general_profile_idc);
    printf("  general_profile_compatibility_flag: ");
    for (j = 0;j < 32;j++)
        printf("%d ", ptl->general_profile_compatibility_flag[j]);
    printf("\n");
    printf("  general_progressive_source_flag:%d\n", ptl->general_progressive_source_flag);
    printf("  general_interlaced_source_flag:%d\n", ptl->general_interlaced_source_flag);
    printf("  general_non_packed_constraint_flag:%d\n", ptl->general_non_packed_constraint_flag);
    printf("  general_frame_only_constraint_flag:%d\n", ptl->general_frame_only_constraint_flag);
    printf("  general_level_idc:%d\n", ptl->general_level_idc);
}

void printSH(h265_stream_t* h)
{
    int i = 0;
    slice_segment_header_t* ssh = h->ssh;
    nal_t* nal = h->nal;
    pps_t* pps = h->pps;
    sps_t* sps = h->sps;
    printf("Slice header:\n");
    printf("first_slice_segment_in_pic_flag:%d\n", ssh->first_slice_segment_in_pic_flag);
    if ((nal->nal_unit_type >= NAL_UNIT_CODED_SLICE_BLA_W_LP) && (nal->nal_unit_type <= NAL_UNIT_RESERVED_IRAP_VCL23))
    {
        printf("no_output_of_prior_pics_flag:%d\n", ssh->no_output_of_prior_pics_flag);
    }
    printf("slice_pic_parameter_set_id:%d\n", ssh->slice_pic_parameter_set_id);
    if (!ssh->first_slice_segment_in_pic_flag)
    {
        if (pps->dependent_slice_segments_enabled_flag)
        {
            printf("dependent_slice_segment_flag:%d\n", ssh->dependent_slice_segment_flag);
        }
        printf("slice_segment_address:%d\n", ssh->slice_segment_address);
    }
    if (!ssh->dependent_slice_segment_flag)
    {
        printf("slice_reserved_flag:%d ");
        for (i = 0;i < pps->num_extra_slice_header_bits;i++)
        {
            printf("%d ", ssh->slice_reserved_flag[i]);
        }
        printf("\n");
        printf("slice_type:%d\n", ssh->slice_type);
        if (pps->output_flag_present_flag)
        {
            printf("pic_output_flag:%d\n", ssh->pic_output_flag);
        }
        if (sps->separate_colour_plane_flag == 1)
        {
            printf("colour_plane_id:%d\n", ssh->colour_plane_id);
        }
        if ((nal->nal_unit_type != NAL_UNIT_CODED_SLICE_IDR_W_RADL) && (nal->nal_unit_type != NAL_UNIT_CODED_SLICE_IDR_N_LP))
        {
            printf("slice_pic_order_cnt_lsb:%d\n", ssh->slice_pic_order_cnt_lsb);
            printf("short_term_ref_pic_set_sps_flag:%d\n", ssh->short_term_ref_pic_set_sps_flag);
        }
    }
}