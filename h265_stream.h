/**
 * @h265_stream.h
 * reading bitstream of H.265
 * @author hanyi <13141211944@163.com>
 */

#ifndef _H265_STREAM_H
#define _H265_STREAM_H        1

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "bs.h"

#ifdef __cplusplus
extern "C" {
#endif


    /**
   Video Parameter Set
   @see 7.3.2.1 Video parameter set RBSP syntax
*/
    typedef struct {
        int vps_video_parameter_set_id;
        int vps_base_layer_internal_flag;
        int vps_base_layer_available_flag;
        int vps_max_layers_minus1;
        int vps_max_sub_layers_minus1;
        int vps_temporal_id_nesting_flag;
        int vps_reserved_0xffff_16bits;
        int vps_sub_layer_ordering_info_present_flag;
        int vps_max_dec_pic_buffering_minus1[7];
        int vps_max_num_reorder_pics[7];
        int vps_max_latency_increase_plus1[7];
        int vps_max_layer_id;
        int vps_num_layer_sets_minus1;
        int layer_id_included_flag[1024][1];
        int vps_timing_info_present_flag;
        int vps_num_units_in_tick;
        int vps_time_scale;
        int vps_poc_proportional_to_timing_flag;
        int vps_num_ticks_poc_diff_one_minus1;
        int vps_num_hrd_parameters;
        int hrd_layer_set_idx[1024];
        int cprms_present_flag[1024];
        int vps_extension_flag;
        int vps_extension_data_flag;
    } vps_t;


    /**
       Sequence Parameter Set
       @see 7.3.2.2 Sequence parameter set RBSP syntax
       @see read_seq_parameter_set_rbsp
       @see write_seq_parameter_set_rbsp
       @see debug_sps
    */
    typedef struct
    {
        int sps_video_parameter_set_id;
        int sps_max_sub_layers_minus1;
        int sps_temporal_id_nesting_flag;
        //TODO profile_tier_level
        int sps_seq_parameter_set_id;
        int chroma_format_idc;
        int separate_colour_plane_flag;
        int pic_width_in_luma_samples;
        int pic_height_in_luma_samples;
        int conformance_window_flag;
        int conf_win_left_offset;
        int conf_win_right_offset;
        int conf_win_top_offse;
        int conf_win_bottom_offset;
        int bit_depth_luma_minus8;
        int bit_depth_chroma_minus8;
        int log2_max_pic_order_cnt_lsb_minus4;
        int sps_sub_layer_ordering_info_present_flag;
        int sps_max_dec_pic_buffering_minus1[7];
        int sps_max_num_reorder_pics[7];
        int sps_max_latency_increase_plus1[7];
        int log2_min_luma_coding_block_size_minus3;
        int log2_diff_max_min_luma_coding_block_size;
        int log2_min_luma_transform_block_size_minus2;
        int log2_diff_max_min_luma_transform_block_size;
        int max_transform_hierarchy_depth_inter;
        int max_transform_hierarchy_depth_intra;
        int scaling_list_enabled_flag;
        int sps_scaling_list_data_present_flag;
        //TODO: scaling_list_data
        int amp_enabled_flag;
        int sample_adaptive_offset_enabled_flag;
        int pcm_enabled_flag;
        int pcm_sample_bit_depth_luma_minus1;
        int pcm_sample_bit_depth_chroma_minus1;
        int log2_min_pcm_luma_coding_block_size_minus3;
        int log2_diff_max_min_pcm_luma_coding_block_size;
        int pcm_loop_filter_disabled_flag;
        int num_short_term_ref_pic_sets;

        //TODO: st_ref_pic_set( i )

        int long_term_ref_pics_present_flag;
        int num_long_term_ref_pics_sps;
        int lt_ref_pic_poc_lsb_sps[33];
        int used_by_curr_pic_lt_sps_flag[33];
        int sps_temporal_mvp_enabled_flag;
        int strong_intra_smoothing_enabled_flag;
        int vui_parameters_present_flag;
        //TODO: vui
        int sps_extension_present_flag;
        int sps_range_extension_flag;
        int sps_multilayer_extension_flag;
        int sps_3d_extension_flag;
        int sps_scc_extension_flag;
        int sps_extension_4bits;

        //TODO: sps_range_extension( )
        // 7.3.2.2.2
        struct {
            int transform_skip_rotation_enabled_flag;
            int transform_skip_context_enabled_flag;
            int implicit_rdpcm_enabled_flag;
            int explicit_rdpcm_enabled_flag;
            int extended_precision_processing_flag;
            int intra_smoothing_disabled_flag;
            int high_precision_offsets_enabled_flag;
            int persistent_rice_adaptation_enabled_flag;
            int cabac_bypass_alignment_enabled_flag;
        } sps_range_extension;
        //TODO: sps_multilayer_extension( )
        // F.7.3.2.2.4
        struct {
            int inter_view_mv_vert_constraint_flag;
        } sps_multilayer_extension;
        //TODO: sps_3d_extension( )
        // I.7.3.2.2.5
        struct {
            int iv_di_mc_enabled_flag[2];
            int iv_mv_scal_enabled_flag[2];
            int log2_ivmc_sub_pb_size_minus3[2];
            int iv_res_pred_enabled_flag[2];
            int depth_ref_enabled_flag[2];
            int vsp_mc_enabled_flag[2];
            int dbbp_enabled_flag[2];
            int tex_mc_enabled_flag[2];
            int log2_texmc_sub_pb_size_minus3[2];
            int intra_contour_enabled_flag[2];
            int intra_dc_only_wedge_enabled_flag[2];
            int cqt_cu_part_pred_enabled_flag[2];
            int inter_dc_only_enabled_flag[2];
            int skip_intra_enabled_flag[2];
        } sps_3d_extension;
        //TODO: sps_scc_extension( )
        //7.3.2.2.3
        int sps_extension_data_flag;

    } sps_t;


    /**
   Picture Parameter Set
   @see 7.3.2.3 Picture parameter set RBSP syntax
    */
    typedef struct {
        int pps_pic_parameter_set_id;
        int pps_seq_parameter_set_id;
        int dependent_slice_segments_enabled_flag;
        int output_flag_present_flag;
        int num_extra_slice_header_bits;
        int sign_data_hiding_enabled_flag;
        int cabac_init_present_flag;
        int num_ref_idx_l0_default_active_minus1;
        int num_ref_idx_l1_default_active_minus1;
        int init_qp_minus26;
        int constrained_intra_pred_flag;
        int transform_skip_enabled_flag;
        int cu_qp_delta_enabled_flag;
        int diff_cu_qp_delta_depth;
        int pps_cb_qp_offset;
        int pps_cr_qp_offset;
        int pps_slice_chroma_qp_offsets_present_flag;
        int weighted_pred_flag;
        int weighted_bipred_flag;
        int transquant_bypass_enabled_flag;
        int tiles_enabled_flag;
        int entropy_coding_sync_enabled_flag;
        int num_tile_columns_minus1;
        int num_tile_rows_minus1;
        int uniform_spacing_flag;
        //TODO: column_width_minus1[] row_height_minus1[]
        //FIXME
        int* column_width_minus1;
        int* row_height_minus1;

        int loop_filter_across_tiles_enabled_flag;
        int pps_loop_filter_across_slices_enabled_flag;
        int deblocking_filter_control_present_flag;
        int deblocking_filter_override_enabled_flag;
        int pps_deblocking_filter_disabled_flag;
        int pps_beta_offset_div2;
        int pps_tc_offset_div2;
        int pps_scaling_list_data_present_flag;
        int lists_modification_present_flag;
        int log2_parallel_merge_level_minus2;
        int slice_segment_header_extension_present_flag;
        int pps_extension_present_flag;
        int pps_range_extension_flag;
        int pps_multilayer_extension_flag;
        int pps_3d_extension_flag;
        int pps_scc_extension_flag;
        int pps_extension_4bits;
        //TODO: pps_range_extension( )
        //TODO: pps_multilayer_extension( ) 
        //TODO: pps_3d_extension( )
        //TODO: pps_scc_extension( )
        int pps_extension_data_flag;
    } pps_t;

    //7.3.3
    typedef struct {
        int general_profile_space;
        int general_tier_flag;
        int general_profile_idc;
        int general_profile_compatibility_flag[32];
        int general_progressive_source_flag;
        int general_interlaced_source_flag;
        int general_non_packed_constraint_flag;
        int general_frame_only_constraint_flag;
        int general_max_12bit_constraint_flag;
        int general_max_10bit_constraint_flag;
        int general_max_8bit_constraint_flag;
        int general_max_422chroma_constraint_flag;
        int general_max_420chroma_constraint_flag;
        int general_max_monochrome_constraint_flag;
        int general_intra_constraint_flag;
        int general_one_picture_only_constraint_flag;
        int general_lower_bit_rate_constraint_flag;
        int general_max_14bit_constraint_flag;
        int general_reserved_zero_33bits;
        int general_reserved_zero_34bits;
        int general_reserved_zero_7bits;
        int general_reserved_zero_35bits;
        int general_reserved_zero_43bits;
        int general_inbld_flag;
        int general_reserved_zero_bit;
        int general_level_idc;
        int sub_layer_profile_present_flag[6];
        int sub_layer_level_present_flag[6];
        int reserved_zero_2bits[2];
        int sub_layer_profile_space[6];
        int sub_layer_tier_flag[6];
        int sub_layer_profile_idc[6];
        int sub_layer_profile_compatibility_flag[6][32];
        int sub_layer_progressive_source_flag[6];
        int sub_layer_interlaced_source_flag[6];
        int sub_layer_non_packed_constraint_flag[6];
        int sub_layer_frame_only_constraint_flag[6];
        int sub_layer_max_12bit_constraint_flag[6];
        int sub_layer_max_10bit_constraint_flag[6];
        int sub_layer_max_8bit_constraint_flag[6];
        int sub_layer_max_422chroma_constraint_flag[6];
        int sub_layer_max_420chroma_constraint_flag[6];
        int sub_layer_max_monochrome_constraint_flag[6];
        int sub_layer_intra_constraint_flag[6];
        int sub_layer_one_picture_only_constraint_flag[6];
        int sub_layer_lower_bit_rate_constraint_flag[6];
        int sub_layer_max_14bit_constraint_flag[6];
        int sub_layer_reserved_zero_33bits[6];
        int sub_layer_reserved_zero_34bits[6];
        int sub_layer_reserved_zero_7bits[6];
        int sub_layer_reserved_zero_35bits[6];
        int sub_layer_reserved_zero_43bits[6];
        int sub_layer_inbld_flag[6];
        int sub_layer_reserved_zero_bit[6];
        int sub_layer_level_idc[6];
    } profile_tier_level_t;

    //7.3.7 Short-term reference picture set syntax
    //st_ref_pic_set
    typedef struct {
        int inter_ref_pic_set_prediction_flag;
        int delta_idx_minus1;
        int delta_rps_sign;
        int abs_delta_rps_minus1;
        int used_by_curr_pic_flag[16];
        int use_delta_flag[16];
        int num_negative_pics;
        int num_positive_pics;
        int delta_poc_s0_minus1[16];
        int used_by_curr_pic_s0_flag[16];
        int delta_poc_s1_minus1[16];
        int used_by_curr_pic_s1_flag[16];
    } st_ref_pic_set_t;

    //E.2.1
    typedef struct
    {
        int aspect_ratio_info_present_flag;
        int aspect_ratio_idc;
        int sar_width;
        int sar_height;
        int overscan_info_present_flag;
        int overscan_appropriate_flag;
        int video_signal_type_present_flag;
        int video_format;
        int video_full_range_flag;
        int colour_description_present_flag;
        int colour_primaries;
        int transfer_characteristics;
        int matrix_coeffs;
        int chroma_loc_info_present_flag;
        int chroma_sample_loc_type_top_field;
        int chroma_sample_loc_type_bottom_field;
        int neutral_chroma_indication_flag;
        int field_seq_flag;
        int frame_field_info_present_flag;
        int default_display_window_flag;
        int def_disp_win_left_offset;
        int def_disp_win_right_offset;
        int def_disp_win_top_offset;
        int def_disp_win_bottom_offset;
        int vui_timing_info_present_flag;
        int vui_num_units_in_tick;
        int vui_time_scale;
        int vui_poc_proportional_to_timing_flag;
        int vui_num_ticks_poc_diff_one_minus1;
        int vui_hrd_parameters_present_flag;
        int bitstream_restriction_flag;
        int tiles_fixed_structure_flag;
        int motion_vectors_over_pic_boundaries_flag;
        int restricted_ref_pic_lists_flag;
        int min_spatial_segmentation_idc;
        int max_bytes_per_pic_denom;
        int max_bits_per_min_cu_denom;
        int log2_max_mv_length_horizontal;
        int log2_max_mv_length_vertical;
    } vui_t;

    //E.2.2
    typedef struct
    {
        int nal_hrd_parameters_present_flag;
        int vcl_hrd_parameters_present_flag;
        int sub_pic_hrd_params_present_flag;
        int tick_divisor_minus2;
        int du_cpb_removal_delay_increment_length_minus1;
        int sub_pic_cpb_params_in_pic_timing_sei_flag;
        int dpb_output_delay_du_length_minus1;
        int bit_rate_scale;
        int cpb_size_scale;
        int cpb_size_du_scale;
        int initial_cpb_removal_delay_length_minus1;
        int au_cpb_removal_delay_length_minus1;
        int dpb_output_delay_length_minus1;
        int fixed_pic_rate_general_flag[7];
        int fixed_pic_rate_within_cvs_flag[7];
        int elemental_duration_in_tc_minus1[7];
        int low_delay_hrd_flag[7];
        int cpb_cnt_minus1[7];
        //TODO: E.2.3 sub_layer_hrd_parameters()
        int bit_rate_value_minus1[32];
        int cpb_size_value_minus1[32];
        int cpb_size_du_value_minus1[32];
        int bit_rate_du_value_minus1[32];
        int cbr_flag[32];
    } hrd_t;

    //7.3.4: scaling_list_data( )
    typedef struct {
        int scaling_list_pred_mode_flag[4][6];
        int scaling_list_pred_matrix_id_delta[4][6];
        int scaling_list_dc_coef_minus8[2][6];
        int scaling_list_delta_coef;
    } scaling_list_data_t;

    /**
   Access unit delimiter
   @see 7.3.2.5 NAL unit syntax
   @see read_nal_unit
   @see write_nal_unit
   @see debug_nal
*/
    typedef struct
    {
        int pic_type;
    } aud_t;

    /**
      Slice Header
      @see 7.3.6.1 Slice header syntax
      @see read_slice_header_rbsp
      @see write_slice_header_rbsp
      @see debug_slice_header_rbsp
    */
    typedef struct
    {
        int first_slice_segment_in_pic_flag;
        int no_output_of_prior_pics_flag;
        int slice_pic_parameter_set_id;
        int dependent_slice_segment_flag;
        int slice_segment_address;
        int slice_reserved_flag[2];
        int slice_type;
        int pic_output_flag;
        int colour_plane_id;
        int slice_pic_order_cnt_lsb;
        int short_term_ref_pic_set_sps_flag;
        //TODO 7.3.7 st_ref_pic_set( num_short_term_ref_pic_sets )

        int short_term_ref_pic_set_idx;
        int num_long_term_sps;
        int num_long_term_pics;
        //FIXME
        int* lt_idx_sps;
        int* poc_lsb_lt;
        int* used_by_curr_pic_lt_flag;
        int* delta_poc_msb_present_flag;
        int* delta_poc_msb_cycle_lt;

        int slice_temporal_mvp_enabled_flag;
        int slice_sao_luma_flag;
        int slice_sao_chroma_flag;
        int num_ref_idx_active_override_flag;
        int num_ref_idx_l0_active_minus1;
        int num_ref_idx_l1_active_minus1;

        //TODO 7.3.6.2 ref_pic_lists_modification( )
        //7.3.6.2 Reference picture list modification syntax
        //ref_pic_list_modification()
        struct
        {
            int ref_pic_list_modification_flag_l0;
            int list_entry_l0[14];
            int ref_pic_list_modification_flag_l1;
            int list_entry_l1[14];
        } rplm;

        int mvd_l1_zero_flag;
        int cabac_init_flag;
        int collocated_from_l0_flag;
        int collocated_ref_idx;

        //TODO 7.6.3.6 pred_weight_table( )

        int five_minus_max_num_merge_cand;
        int use_integer_mv_flag;
        int slice_qp_delta;
        int slice_cb_qp_offset;
        int slice_cr_qp_offset;
        int slice_act_y_qp_offset;
        int slice_act_cb_qp_offset;
        int slice_act_cr_qp_offset;
        int cu_chroma_qp_offset_enabled_flag;
        int deblocking_filter_override_flag;
        int slice_deblocking_filter_disabled_flag;
        int slice_beta_offset_div2;
        int slice_tc_offset_div2;
        int slice_loop_filter_across_slices_enabled_flag;
        int num_entry_point_offsets;
        int offset_len_minus1;
        //FIXME
        int* entry_point_offset_minus1;
        int slice_segment_header_extension_length;
        int slice_segment_header_extension_data_byte[256];

    } slice_segment_header_t;

    typedef struct
    {
        int rbsp_size;
        uint8_t* rbsp_buf;
    } slice_data_rbsp_t;

    /**
   Network Abstraction Layer (NAL) unit
   @see 7.3.1 NAL unit syntax
   @see read_nal_unit
   @see write_nal_unit
   @see debug_nal
*/
    typedef struct
    {
        int forbidden_zero_bit;
        int nal_unit_type;
        int nuh_layer_id;
        int nuh_temporal_id_plus1;

        //uint8_t* rbsp_buf;
        //int rbsp_size;
    } nal_t;

    /**
   H265 stream
   Contains data structures for all NAL types that can be handled by this library.
   When reading, data is read into those, and when writing it is written from those.
   The reason why they are all contained in one place is that some of them depend on others, we need to
   have all of them available to read or write correctly.
 */
    typedef struct
    {
        nal_t* nal;
        vps_t* vps;
        sps_t* sps;
        pps_t* pps;
        aud_t* aud;
        st_ref_pic_set_t* strps;

        profile_tier_level_t* ptl;
        vui_t* vui;
        hrd_t* hrd;
        scaling_list_data_t* sld;
        slice_segment_header_t* ssh;
        slice_data_rbsp_t* slice_data;

        vps_t* vps_table[16];
        sps_t* sps_table[32];
        pps_t* pps_table[256];
    } h265_stream_t;

    h265_stream_t* h265_new();
    void h265_free(h265_stream_t* h);

    int find_nal_unit(uint8_t* buf, int size, int* nal_start, int* nal_end);

    int rbsp_to_nal(const uint8_t* rbsp_buf, const int* rbsp_size, uint8_t* nal_buf, int* nal_size);
    int nal_to_rbsp(const uint8_t* nal_buf, int* nal_size, uint8_t* rbsp_buf, int* rbsp_size);

    int read_debug_nal_unit(h265_stream_t* h, uint8_t* buf, int size);
    void read_debug_video_parameter_set_rbsp(h265_stream_t* h, bs_t* b);
    void read_debug_seq_parameter_set_rbsp(h265_stream_t* h, bs_t* b);
    void read_st_ref_pic_set(h265_stream_t* h, bs_t* b, int  stRpsIdx);
    void read_ref_pic_lists_modification(h265_stream_t* h, bs_t* b);
    void read_profile_tier_level(h265_stream_t* h, bs_t* b, int profilePresentFlag, int maxNumSubLayersMinus1);
    void read_scaling_list_data(h265_stream_t* h, bs_t* b);
    void read_debug_vui_parameters(h265_stream_t* h, bs_t* b);
    void read_debug_hrd_parameters(h265_stream_t* h, bs_t* b);
    void read_debug_pic_parameter_set_rbsp(h265_stream_t* h, bs_t* b);
    void read_access_unit_delimiter_rbsp(h265_stream_t* h, bs_t* b);
    void read_scaling_list_data(h265_stream_t* h, bs_t* b);
    void read_slice_segment_header(h265_stream_t* h, bs_t* b);
    void read_debug_slice_rbsp(h265_stream_t* h, bs_t* b);

    void read_debug_rbsp_trailing_bits(h265_stream_t* h, bs_t* b);
    void read_debug_rbsp_trailing_bits(h265_stream_t* h, bs_t* b);
    int more_rbsp_trailing_data(h265_stream_t* h, bs_t* b);

    void printVPS(h265_stream_t* h);
    void printSPS(h265_stream_t* h);
    void printPPS(h265_stream_t* h);
    void printPTL(h265_stream_t* h);
    void printSH(h265_stream_t* h);

    // TODO: Existing names used for the different NAL unit types can be altered to better reflect the names in the spec.
//       However, the names in the spec are not yet stable at this point. Once the names are stable, a cleanup
//       effort can be done without use of macros to alter the names used to indicate the different NAL unit types.
    //Table 7-1 NAL unit type codes
    enum NalUnitType
    {
        NAL_UNIT_CODED_SLICE_TRAIL_N = 0, // 0
        NAL_UNIT_CODED_SLICE_TRAIL_R,     // 1

        NAL_UNIT_CODED_SLICE_TSA_N,       // 2
        NAL_UNIT_CODED_SLICE_TSA_R,       // 3

        NAL_UNIT_CODED_SLICE_STSA_N,      // 4
        NAL_UNIT_CODED_SLICE_STSA_R,      // 5

        NAL_UNIT_CODED_SLICE_RADL_N,      // 6
        NAL_UNIT_CODED_SLICE_RADL_R,      // 7

        NAL_UNIT_CODED_SLICE_RASL_N,      // 8
        NAL_UNIT_CODED_SLICE_RASL_R,      // 9

        NAL_UNIT_RESERVED_VCL_N10,
        NAL_UNIT_RESERVED_VCL_R11,
        NAL_UNIT_RESERVED_VCL_N12,
        NAL_UNIT_RESERVED_VCL_R13,
        NAL_UNIT_RESERVED_VCL_N14,
        NAL_UNIT_RESERVED_VCL_R15,

        NAL_UNIT_CODED_SLICE_BLA_W_LP,    // 16
        NAL_UNIT_CODED_SLICE_BLA_W_RADL,  // 17
        NAL_UNIT_CODED_SLICE_BLA_N_LP,    // 18
        NAL_UNIT_CODED_SLICE_IDR_W_RADL,  // 19
        NAL_UNIT_CODED_SLICE_IDR_N_LP,    // 20
        NAL_UNIT_CODED_SLICE_CRA,         // 21
        NAL_UNIT_RESERVED_IRAP_VCL22,
        NAL_UNIT_RESERVED_IRAP_VCL23,

        NAL_UNIT_RESERVED_VCL24,
        NAL_UNIT_RESERVED_VCL25,
        NAL_UNIT_RESERVED_VCL26,
        NAL_UNIT_RESERVED_VCL27,
        NAL_UNIT_RESERVED_VCL28,
        NAL_UNIT_RESERVED_VCL29,
        NAL_UNIT_RESERVED_VCL30,
        NAL_UNIT_RESERVED_VCL31,

        NAL_UNIT_VPS,                     // 32
        NAL_UNIT_SPS,                     // 33
        NAL_UNIT_PPS,                     // 34
        NAL_UNIT_ACCESS_UNIT_DELIMITER,   // 35
        NAL_UNIT_EOS,                     // 36
        NAL_UNIT_EOB,                     // 37
        NAL_UNIT_FILLER_DATA,             // 38
        NAL_UNIT_PREFIX_SEI,              // 39
        NAL_UNIT_SUFFIX_SEI,              // 40

        NAL_UNIT_RESERVED_NVCL41,
        NAL_UNIT_RESERVED_NVCL42,
        NAL_UNIT_RESERVED_NVCL43,
        NAL_UNIT_RESERVED_NVCL44,
        NAL_UNIT_RESERVED_NVCL45,
        NAL_UNIT_RESERVED_NVCL46,
        NAL_UNIT_RESERVED_NVCL47,
        NAL_UNIT_UNSPECIFIED_48,
        NAL_UNIT_UNSPECIFIED_49,
        NAL_UNIT_UNSPECIFIED_50,
        NAL_UNIT_UNSPECIFIED_51,
        NAL_UNIT_UNSPECIFIED_52,
        NAL_UNIT_UNSPECIFIED_53,
        NAL_UNIT_UNSPECIFIED_54,
        NAL_UNIT_UNSPECIFIED_55,
        NAL_UNIT_UNSPECIFIED_56,
        NAL_UNIT_UNSPECIFIED_57,
        NAL_UNIT_UNSPECIFIED_58,
        NAL_UNIT_UNSPECIFIED_59,
        NAL_UNIT_UNSPECIFIED_60,
        NAL_UNIT_UNSPECIFIED_61,
        NAL_UNIT_UNSPECIFIED_62,
        NAL_UNIT_UNSPECIFIED_63,
        NAL_UNIT_INVALID,
    };
    //7.4.7 Table 7-7. Name association to slice_type
#define SH_SLICE_TYPE_B        0        // B (B slice)
#define SH_SLICE_TYPE_P        1        // P (P slice)
#define SH_SLICE_TYPE_I        2        // I (I slice)

//Appendix E. Table E-1  Meaning of sample aspect ratio indicator
#define SAR_Unspecified  0           // Unspecified
#define SAR_1_1        1             //  1:1
#define SAR_12_11      2             // 12:11
#define SAR_10_11      3             // 10:11
#define SAR_16_11      4             // 16:11
#define SAR_40_33      5             // 40:33
#define SAR_24_11      6             // 24:11
#define SAR_20_11      7             // 20:11
#define SAR_32_11      8             // 32:11
#define SAR_80_33      9             // 80:33
#define SAR_18_11     10             // 18:11
#define SAR_15_11     11             // 15:11
#define SAR_64_33     12             // 64:33
#define SAR_160_99    13             // 160:99
#define SAR_4_3       14
#define SAR_3_2       15
#define SAR_2_1       16
                                     // 17..254           Reserved
#define SAR_Extended      255        // Extended_SAR

// file handle for debug output
    extern FILE* h265_dbgfile;

#ifdef __cplusplus
}
#endif

#endif