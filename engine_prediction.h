#pragma once
#include "utils/singleton.h"
#include <cstdint>
#include "interfaces/CInput.h"
#include "sdk/csgo_struct.h"


/*
class c_move_data {
public:
	bool first_run_of_functions : 1;
	bool game_code_moved_player : 1;
	bool no_air_control : 1;
	unsigned long player_handle{};
	int impulse_command{};
	Vector view_angles;
	Vector abs_view_angles;
	int	buttons{};
	int	old_buttons{};
	float forward_move{};
	float side_move{};
	float up_move{};
	float max_speed{};
	float client_max_speed{};
	Vector velocity;
	Vector old_velocity;
	float somefloat{};
	Vector angles;
	Vector old_angles;
	float out_step_height{};
	Vector out_wish_velocity;
	Vector out_jump_velocity;
	Vector constraint_center;
	float constraint_radius{};
	float constraint_width{};
	float constraint_speed_factor{};
	bool constraint_past_radius{};

	Vector abs_origin;
};*/
class c_game_movement {
public:
	virtual	~c_game_movement() = default;
	virtual void process_movement(c_base_player* player, c_move_data* data) = 0;
	virtual void reset() = 0;
	virtual void start_track_prediction_errors(c_base_player* player) = 0;
	virtual void finish_track_prediction_errors(c_base_player* player) = 0;
};
class c_prediction_system : public singleton<c_prediction_system>
{
	using md5_pseudo_random = uint32_t(__thiscall*)(uint32_t);

	struct prediction_info
	{
		c_user_cmd* cmd{};
		float curtime{};
		c_move_data move_data{};
		int32_t flags{};
		CBaseHandle ground_entity{};
	};

public:
	c_prediction_system();

	void initial(c_base_player* local, c_user_cmd* cmd, int32_t seq);
	void repredict(c_base_player* local, c_user_cmd* cmd);
	void restore() const;

	prediction_info animation_info[150]{};
	c_user_cmd original_cmd{};
	int32_t unpredicted_flags{};
	Vector unpredicted_velocity{};

	c_move_data move_data{}, unpredicted_move_data{};
	float backup_curtime{}, backup_frametime{};
private:
	uint32_t* prediction_random_seed;
	c_base_player** prediction_player;
	md5_pseudo_random _md5_pseudo_random;

	c_user_cmd* last_cmd{};
	int32_t tick_base{}, seq_diff{};

	
	int backup_tickbase{};
	Vector backup_origin{};
	Vector backup_aim_punch{}, backup_aim_punch_velocity{}, backup_view_offset{};
	float backup_accuracy_penalty{}, backup_recoil_index{}, backup_duck_amount{};
};