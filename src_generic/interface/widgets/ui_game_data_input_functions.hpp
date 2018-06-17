#pragma once

#include <precompile.h>

namespace Yelo
{
	namespace Enums
	{
		enum game_data_input_reference_function : short
		{
			_game_data_input_reference_function_null,
			_game_data_input_reference_function_player_settings_menu_update_desc,
			_game_data_input_reference_function_unused,
			_game_data_input_reference_function_playlist_settings_menu_update_desc,
			_game_data_input_reference_function_gametype_select_menu_update_desc,
			_game_data_input_reference_function_multiplayer_type_menu_update_desc,
			_game_data_input_reference_function_solo_level_select_update,
			_game_data_input_reference_function_difficulty_menu_update_desc,
			_game_data_input_reference_function_build_number_textbox_only,
			_game_data_input_reference_function_server_list_update,
			_game_data_input_reference_function_network_pregame_status_update,
			_game_data_input_reference_function_splitscreen_pregame_status_update,
			_game_data_input_reference_function_net_splitscreen_prejoin_players,
			_game_data_input_reference_function_mp_profile_list_update,
			_game_data_input_reference_function__3wide_player_profile_list_update,
			_game_data_input_reference_function_plyr_prof_edit_select_menu_upd8,
			_game_data_input_reference_function_player_profile_small_menu_update,
			_game_data_input_reference_function_game_settings_lists_text_update,
			_game_data_input_reference_function_solo_game_objective_text,
			_game_data_input_reference_function_color_picker_update,
			_game_data_input_reference_function_game_settings_lists_pic_update,
			_game_data_input_reference_function_main_menu_fake_animate,
			_game_data_input_reference_function_mp_level_select_update,
			_game_data_input_reference_function_get_active_plyr_profile_name,
			_game_data_input_reference_function_get_edit_plyr_profile_name,
			_game_data_input_reference_function_get_edit_game_settings_name,
			_game_data_input_reference_function_get_active_plyr_profile_color,
			_game_data_input_reference_function_mp_set_textbox_map_name,
			_game_data_input_reference_function_mp_set_textbox_game_ruleset,
			_game_data_input_reference_function_mp_set_textbox_teams_noteams,
			_game_data_input_reference_function_mp_set_textbox_score_limit,
			_game_data_input_reference_function_mp_set_textbox_score_limit_type,
			_game_data_input_reference_function_mp_set_bitmap_for_map,
			_game_data_input_reference_function_mp_set_bitmap_for_ruleset,
			_game_data_input_reference_function_mp_set_textbox,
			_game_data_input_reference_function_mp_edit_profile_set_rule_text,
			_game_data_input_reference_function_system_link_status_check,
			_game_data_input_reference_function_mp_game_directions,
			_game_data_input_reference_function_teams_no_teams_bitmap_update,
			_game_data_input_reference_function_warn_if_diff_will_nuke_saved_game,
			_game_data_input_reference_function_dim_if_no_net_cable,
			_game_data_input_reference_function_pause_game_set_textbox_inverted,
			_game_data_input_reference_function_dim_unless_two_controllers,
			_game_data_input_reference_function_controls_update_menu,
			_game_data_input_reference_function_video_menu_update,
			_game_data_input_reference_function_gamespy_screen_update,
			_game_data_input_reference_function_common_button_bar_update,
			_game_data_input_reference_function_gamepad_update_menu,
			_game_data_input_reference_function_server_settings_update,
			_game_data_input_reference_function_audio_menu_update,
			_game_data_input_reference_function_mp_prof_vehicles_update,
			_game_data_input_reference_function_solo_map_list_update,
			_game_data_input_reference_function_mp_map_list_update,
			_game_data_input_reference_function_gt_select_list_update,
			_game_data_input_reference_function_gt_edit_list_update,
			_game_data_input_reference_function_load_game_list_update,
			_game_data_input_reference_function_checking_for_updates,
			_game_data_input_reference_function_direct_ip_connect_update,
			_game_data_input_reference_function_network_settings_update,

			k_number_of_game_data_input_reference_functions,
		};
	};

	namespace TagGroups
	{
		struct game_data_input_reference
		{
			Enums::game_data_input_reference_function function;
			unsigned short : 16;
			tag_string : 8 * sizeof(tag_string) * 1;
		}; static_assert( sizeof(game_data_input_reference) == 0x24 ); // max count: 64
	};
};
