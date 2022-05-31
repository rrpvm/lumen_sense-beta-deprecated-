#pragma once
#include "csgo_struct.h"
class c_base_player;
class csgo
{
public:
	c_user_cmd* cmd;
	c_base_player* local;
	Vector screen_center;
	int count_players = 1;
	float alpha_data[64];
	float box_color_alpha[64];
	float box_background_color_alpha[64];
	float skeleton_color_alpha[64];
	float name_color_alpha[64];
	float old_alpha_data[64];
	float old_curtime_for_alpha[64];
	float fl_rainbow_chams = 0.0f;
	Color c_rainbow__chams = Color(255, 255,0, 0);
	int active_wpn_type = 0;
	float lask_kill_time = 0.0f;
	bool in_prediction = false;
	bool in_load_cfg = false;
	bool auto_select_weapon = false;
private:
	
};
inline csgo csgo_data;