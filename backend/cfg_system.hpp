#pragma once
#include <Windows.h>
#include <string>
#include <fstream>
#include <vector>
#include "../utils/Color.h"

template< typename T >
class ConfigItem
{
	std::string category, name;
	T* value;
public:
	ConfigItem(std::string category, std::string name, T* value)
	{
		this->category = category;
		this->name = name;
		this->value = value;
	}
};
template< typename T >
class ConfigValue
{
public:
	ConfigValue(std::string category_, std::string name_, T* value_)
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};
struct label_wpn_stg_b {
	label_wpn_stg_b() {};
	label_wpn_stg_b(bool value, const char* _name) { val = &value; name = _name; };
	bool* val;
	const char* name;
};
struct legit_weapon_settings {
	std::vector<label_wpn_stg_b>bools;
	bool enabled = false;
	bool p_silent = false;
	bool silent = false;
	bool trigger = false;
	bool rcs_standalone = false;
	bool visibility_check;
	bool backtrack;
	bool extrapolate;
	bool engine_predict;
	bool nearest;
	bool shadow_rcs;
	bool randomize_rcs;
	bool auto_shoot;
	bool auto_stop;
	int nearest_hitboxes;
	int target_method;
	int curr_hitbox;
	int fov =1;
	int p_silent_fov = 1;
	int smooth_type = 1;
	int trigger_key = 0;
	int aimbot_key = 0;
	int auto_shot_key = 0;
	int trigger_delay = 1;
	float kill_delay = 1.f;
	float switch_target_delay = 1.f;
	float smooth = 1.0f;
	float auto_stop_power = 0.85f;
	int rcs_x = 100;
	int rcs_y = 100;
	int rcs_delay = 1;
	int trigger_chance = 100;
	int dist_type;
	const char* table_name;
	legit_weapon_settings(const char* table_name) {
		this->table_name = table_name;		
	}
	legit_weapon_settings() {};
};
class cfg_system
{
protected:
	std::vector< ConfigValue< int >* > ints;
	std::vector< ConfigValue< bool >* > bools;
	std::vector< ConfigValue< float >* > floats;
	std::vector< ConfigValue< Color >* > colors;
private:
	void SetupValue(int&, int, std::string, std::string);
	void SetupValue(bool&, bool, std::string, std::string);
	void SetupValue(float&, float, std::string, std::string);
	void SetupValue(Color&, Color, std::string, std::string);
//	void SetupValue(legit_weapon_settings&, std::string, std::string);
public:
	cfg_system()
	{
		Setup();
		cfg_list = get_cfg_list();
	}

	void Setup();
	void Create(std::string cfg_name);
	void Save(std::string cfg_name);
	void Load(std::string cfg_name);
	std::vector<std::string>get_cfg_list();
	std::vector<std::string>cfg_list;
	const char* cfg_list_char[64] = {" ", ""};
	std::string current_cfg_name = "";
	std::string buffer_create_cfg = "";
};
inline cfg_system cfg;
struct Variables
{
	Variables()
	{ }
	struct _Legitbot
	{
		legit_weapon_settings pistol_legit_wpn_data = legit_weapon_settings("pistol_settings");
		legit_weapon_settings machine_legit_wpn_data = legit_weapon_settings("machine_settings");//мп9 , юмп и прочая ересь
		legit_weapon_settings shotgun_legit_wpn_data = legit_weapon_settings("shotgun_settings");
		legit_weapon_settings rifle_legit_wpn_data = legit_weapon_settings("rifle_settings");
		legit_weapon_settings sniper_legit_wpn_data = legit_weapon_settings("sniper_settings");
		legit_weapon_settings none = legit_weapon_settings("none");
		legit_weapon_settings none2 = legit_weapon_settings("none2");


		legit_weapon_settings* wpn_set_table[7] = { &none ,&pistol_legit_wpn_data, &machine_legit_wpn_data, &shotgun_legit_wpn_data,&rifle_legit_wpn_data, &sniper_legit_wpn_data, &none2 };
		int current_pick = 0;
	} legitbot;
	struct _Visuals
	{
		bool enabled;
		bool box;
		bool rainbow_esp;
		bool hp;
		bool line;
		bool name;
		bool weapon_esp;
		bool team;
		bool skeleton;
		bool draw_spread_val;
		bool dlight;
		bool dlight_rainbow;
		bool trails;
		bool rainbow_trails;
		bool trails_on_rails;
		bool rainbow_trails_on_rails;
		bool chams;
		bool sleeves_chams;
		bool backtrack_chams;
		bool hands_chams;
		bool weapon_chams;
		bool ignore_z;
		bool wireframe;
		bool hand_wireframe;
		bool weapon_wireframe;
		bool no_hands;
		bool chams_glow_contrast;
		bool smoke_wireframe;
		bool animated_chams;
		bool force_crosshair;
		bool remove_scope;
		bool glow;
		bool draw_aim_fov;
		bool draw_bullet_tracers;
		bool box_background;
		bool kill_effect;
		bool remove_3d_sky;
		bool remove_shadows;
		bool weapon_esp_icons;
		bool remove_smoke;
		bool sound_esp;
		bool show_aim_target;
		bool draw_distance;
		bool hit_ring;
		int player_chams_type;
		int hands_chams_type;
		int weapon_chams_type;
		int sleeves_chams_type;
		int box_type;
		int hp_position;
		int name_position;
		int dlight_radius;
		int dlight_type;
		int bright;
		int chams_speed_rainbow = 0;
		float night_5_kopeek;//meme
		float bullet_tracer_life;
		float bullet_tracer_width;
		float bullet_tracer_fade;
		float local_trails_life = 1.f;
		float trails_life = 1.f;
		float alpha_reducing = 50.f;
		int pearlsecent = 12;
		int phong = 1;
		int phongboost = 1;
		int phong_exponent = 1;
		int rimlight = 1;
		int rimlight_boost = 1;
		float envmaplightscale = 1.f;
		int rimlightexponent = 1;
		int glow_thikness;
		int tracer_type;
		int local_trail_type;
		int trail_type;
		int sound_esp_material = 0;
		float fl_sound_esp_life = 1.5f;
		float fl_sound_esp_width = 3.f;
		float fl_sound_esp_radius = 85.f;
		Color glow_color;
		Color chams_invisible;
		Color chams_contrast;
		Color model_color;
		Color weapon_color;
		Color hand_color;
		Color sleeves_c_color;
		Color dlight_col;
		Color box_color;
		Color post_effect_col = Color(255, 0, 0, 0);
		Color local_trails_col;
		Color trails_col = Color(255,255,0,255);
		Color skeleton_color;
		Color name_esp_color;
		Color line_esp_color;
		Color bullet_tracers_col;
		Color box_background_color;
		Color reflectivity;
		Color sound_esp_color = Color(255,255,0,255);
		Color hp_esp_background_col = Color(189, 46, 46);
		Color hp_esp_col = Color(48, 194, 46);
		Color aim_light_color = Color(48, 194, 46);
		Color spread_color = Color(120, 120, 120);
	} visuals;
	struct _Misc
	{
		bool kill_spam;
		bool enabled;
		bool fakelags;
		bool bunnyhope;
		bool auto_strafe;
		bool moon_walk;
		bool radar_hack;
		bool hit_sound;
		bool hit_marker;
		bool auto_scope;
		bool slow_walk;
		bool thirdperson;
		bool extend_backtrack;
		float slow_walk_power;
		float third_person_distance;
		float third_person_offset_x = 0.0f;
		int fakelag_factor;
		int viewmodel_fov;
		int fov;
		int viewmodel_offset_x;
		int viewmodel_offset_y;
		int viewmodel_offset_z;
		int thirdperson_key;
		int curr_hitsound;
		int fakelag_type;
	} misc;
	struct _Indicators {
		bool enabled;
		bool chocked;
	} indicators;
	bool show_menu;
};
inline Variables cfg_vars;
/*
struct Variables
{
	Variables()
	{ }
	struct _Legitbot
	{
		bool enabled;
		bool p_silent;
		bool silent;
		bool rcs_standalone;
		bool visibility_check;
		bool backtrack;
		bool extrapolate;
		bool engine_predict;
		bool nearest;
		bool shadow_rcs;
		bool randomize_rcs;
		bool auto_shoot;
		bool auto_stop;
		int nearest_hitboxes;
		int target_method;
		int curr_hitbox;
		int fov;
		int p_silent_fov;
		int smooth_type;
		float smooth;
		int rcs_x;
		int rcs_y;
		int rcs_delay;
		int dist_type;
	} legitbot;
	struct _Visuals
	{
		bool enabled;
		bool box;
		bool rainbow_esp;
		bool hp;
		bool line;
		bool name;
		bool team;
		bool skeleton;
		bool dlight;
		bool dlight_rainbow;
		bool trails;
		bool trails_on_rails;
		bool chams;
		bool backtrack_chams;
		bool hands_chams;
		bool weapon_chams;
		bool ignore_z;
		bool wireframe;
		bool smoke_wireframe;
		bool animated_chams;
		bool force_crosshair;
		bool remove_scope;
		bool glow;
		bool draw_aim_fov;
		int player_chams_type;
		int hands_chams_type;
		int weapon_chams_type;
		int box_type;
		int hp_position;
		int name_position;
		int dlight_radius;
		int dlight_type;
		int bright;
		float night_5_kopeek;//meme
		int glow_thikness;
		Color glow_color;
		Color chams_invisible;
		Color model_color;
		Color weapon_color;
		Color hand_color;
		Color dlight_col;
		Color box_color;
		Color post_effect_col;
		Color local_trails_col;
		Color skeleton_color;
	} visuals;
	struct _Misc
	{
		bool enabled;
		bool fakelags;
		bool bunnyhope;
		bool auto_strafe;
		bool moon_walk;
		bool radar_hack;
		bool hit_sound;
		bool hit_marker;
		int fakelag_factor;
		int viewmodel_fov;
		int fov;
		int viewmodel_offset_x;
		int viewmodel_offset_y;
		int viewmodel_offset_z;
		int curr_hitsound;
	} misc;
	bool show_menu;
}; 
*/