#pragma once

#include "utils/Vector.h"
#include <vector>
#include <Windows.h>
#include  "utils/Color.h"
#include <sdk\csgo_struct.h>
class visuals {
public:
	void on_paint_traverse();
	void get_perfomance(c_base_player* pl, int i);
	void draw_box(c_base_player* pl,int i);
	void draw_line(c_base_player* pl,int i);
	void draw_hp(c_base_player* pl,int i);
	void draw_name(c_base_player* pl,int i);
	void draw_flags(c_base_player* pl,int i);
	void draw_weapons(c_base_player* pl, int i);
	void draw_scope_lines();
	void do_dlight(c_base_player* pl,int i);
	void trails(c_base_player* pl,int i);
	void local_trails();
	void glow(int i);
	void sound_esp(c_base_player* pl,int i);
	void draw_aim_fov(int i);
	void bone_esp(c_base_player* pl,int i);
	void model_changer(ClientFrameStage_t stage);
	void draw_spread();
	void third_person();
	void remove_smoke();
	void reduce_flash();
	void draw_distance(c_base_player* pl, int i);
	struct vis_data {
		Vector pos2D;
		Vector head2D;
		Vector pos3d_left;
		Vector pos3d_right;
		int width;
		int height;
		bool can_see_through_view_matrix;		
	};
	vis_data visuals_data[64];
	DWORD verdana;
	DWORD verdana_big;	
	DWORD icons;
private:
};
inline visuals g_visuals;