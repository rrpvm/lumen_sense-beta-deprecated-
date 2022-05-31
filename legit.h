#pragma once
#include "interfaces/CUserCmd.h"
#include "sdk/csgo_struct.h"
#include <deque>
class legit
{
public:
	struct backtrack_data
	{
		float simulation_time;
		Vector position;
		Vector head_pos;
		matrix3x4_t matrix[128];
	};
public:
	legit() {
		static bool once = true;
		if (once)
		{
			srand(GetTickCount64() + 1338);
			once = false;
		}
	};
	void triggerbot(c_user_cmd* cmd);
	void rcs_standalone(c_user_cmd* cmd);
	void aimbot(c_user_cmd* cmd, bool& b_send_packet);
	void clear_data();
	void stop_movement(c_user_cmd* cmd);
	void get_delta_time();
	void back_track_update();
	void back_track(c_user_cmd* cmd);
	Vector get_best_bone_nearest(c_user_cmd* cmd, c_base_player* player);
	float take_fov(int i, Vector ang) { return get_fov(i,  ang); }		
	int entity_aim = 0;	
	int best_backtrack_target = 0;
	int best_tick_backtrack = 0;
	std::deque<backtrack_data>records[65];	
private:
	
	int get_hitbox_num_from_cfg();
	bool is_aim = false;
	c_base_player* get_best_target(c_user_cmd* cmd);
	c_base_player* update_best_target(c_user_cmd* cmd);
	float get_fov(int i, Vector ang);
	void smooth(Vector& src, Vector dst);
	float aim_time = 0.0f;
	Vector angle_before_aim;
	float delta_time = 0.0f;
	c_base_player* aim_player = nullptr;
	c_base_player* old_aim_player = nullptr;
	int old_entity_aim = 0;
	float last_switched_entity_time = 0.f;
	bool can_scope = false;
	
};

inline legit g_legitbot;