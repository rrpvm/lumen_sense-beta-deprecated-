#include "cfg_system.hpp"
#include <iostream>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")
#include <filesystem>
#define SetLabel(table,str,val, name) {SetupValue(cfg_vars.str.table.name, val,cfg_vars.str.table.table_name, #name);}
void cfg_system::Setup()
{
	ints.clear();
	floats.clear();
	bools.clear();
	ints.reserve(300);
	floats.reserve(300);
	bools.reserve(300);
	/*VISUALS*/
	SetupValue(cfg_vars.visuals.enabled, false, "visuals", "enabled");
	SetupValue(cfg_vars.visuals.box, false, "visuals", "box");
	SetupValue(cfg_vars.visuals.weapon_esp, false, "visuals", "weapon_esp");
	SetupValue(cfg_vars.visuals.hp, false, "visuals", "hp");
	SetupValue(cfg_vars.visuals.line, false, "visuals", "line");
	SetupValue(cfg_vars.visuals.name, false, "visuals", "name");
	SetupValue(cfg_vars.visuals.team, false, "visuals", "team");
	SetupValue(cfg_vars.visuals.skeleton, false, "visuals", "skeleton");
	SetupValue(cfg_vars.visuals.rainbow_esp, false, "visuals", "rainbow_esp");
	SetupValue(cfg_vars.visuals.dlight, false, "visuals", "dlight");
	SetupValue(cfg_vars.visuals.dlight_rainbow, false, "visuals", "dlight_rainbow");
	SetupValue(cfg_vars.visuals.trails, false, "visuals", "trails");
	SetupValue(cfg_vars.visuals.rainbow_trails, false, "visuals", "rainbow_trails");
	SetupValue(cfg_vars.visuals.trails_on_rails, false, "visuals", "trails_on_rails");
	SetupValue(cfg_vars.visuals.rainbow_trails_on_rails, false, "visuals", "rainbow_trails_on_rails");
	SetupValue(cfg_vars.visuals.chams, false, "visuals", "chams");
	SetupValue(cfg_vars.visuals.backtrack_chams, false, "visuals", "backtrack_chams");
	SetupValue(cfg_vars.visuals.hands_chams, false, "visuals", "hands_chams");
	SetupValue(cfg_vars.visuals.weapon_chams, false, "visuals", "weapon_chams");
	SetupValue(cfg_vars.visuals.draw_spread_val, false, "visuals", "draw_spread");
	SetupValue(cfg_vars.visuals.sleeves_chams, false, "visuals", "sleeves_chams");
	SetupValue(cfg_vars.visuals.ignore_z, false, "visuals", "ignore_z");//could make crashes
	SetupValue(cfg_vars.visuals.wireframe, false, "visuals", "wireframe");
	SetupValue(cfg_vars.visuals.smoke_wireframe, false, "visuals", "smoke_wireframe");
	SetupValue(cfg_vars.visuals.animated_chams, false, "visuals", "animated_chams");
	SetupValue(cfg_vars.visuals.force_crosshair, false, "visuals", "force_crosshair");
	SetupValue(cfg_vars.visuals.remove_scope, false, "visuals", "remove_scope");
	SetupValue(cfg_vars.visuals.glow, false, "visuals", "glow");
	SetupValue(cfg_vars.visuals.chams_glow_contrast, false, "visuals", "chams_glow_contrast");
	SetupValue(cfg_vars.visuals.draw_aim_fov, false, "visuals", "draw_aim_fov");
	SetupValue(cfg_vars.visuals.hand_wireframe, false, "visuals", "hand_wireframe");
	SetupValue(cfg_vars.visuals.weapon_wireframe, false, "visuals", "weapon_wireframe");
	SetupValue(cfg_vars.visuals.no_hands, false, "visuals", "no_hands");
	SetupValue(cfg_vars.visuals.draw_bullet_tracers, false, "visuals", "draw_bullet_tracers");
	SetupValue(cfg_vars.visuals.box_background, false, "visuals", "box_background");
	SetupValue(cfg_vars.visuals.kill_effect, false, "visuals", "kill_effect");
	SetupValue(cfg_vars.visuals.remove_3d_sky, false, "visuals", "remove_3d_sky");
	SetupValue(cfg_vars.visuals.remove_shadows, false, "visuals", "remove_shadows");
	SetupValue(cfg_vars.visuals.weapon_esp_icons, false, "visuals", "weapon_esp_icons");
	SetupValue(cfg_vars.visuals.remove_smoke, false, "visuals", "remove_smoke");
	SetupValue(cfg_vars.visuals.sound_esp, false, "visuals", "sound_esp");
	SetupValue(cfg_vars.visuals.show_aim_target, false, "visuals", "show_aim_target");
	SetupValue(cfg_vars.visuals.draw_distance, false, "visuals", "draw_distance");
	SetupValue(cfg_vars.visuals.hit_ring, false, "visuals", "hit_ring");
	SetupValue(cfg_vars.visuals.player_chams_type, 0, "visuals", "player_chams_type");
	SetupValue(cfg_vars.visuals.chams_speed_rainbow, 0, "visuals", "chams_speed_rainbow");
	SetupValue(cfg_vars.visuals.hands_chams_type, 0, "visuals", "hands_chams_type");
	SetupValue(cfg_vars.visuals.weapon_chams_type, 0, "visuals", "weapon_chams_type");
	SetupValue(cfg_vars.visuals.sleeves_chams_type, 0, "visuals", "sleeves_chams_type");
	SetupValue(cfg_vars.visuals.box_type, 0, "visuals", "box_type");
	SetupValue(cfg_vars.visuals.hp_position, 0, "visuals", "hp_position");
	SetupValue(cfg_vars.visuals.name_position, 0, "visuals", "name_position");
	SetupValue(cfg_vars.visuals.dlight_radius, 100, "visuals", "dlight_radius");
	SetupValue(cfg_vars.visuals.dlight_type, 0, "visuals", "dlight_type");
	SetupValue(cfg_vars.visuals.bright, 0, "visuals", "bright");
	SetupValue(cfg_vars.visuals.night_5_kopeek, 0, "visuals", "night_5_kopeek");
	SetupValue(cfg_vars.visuals.sound_esp_material, 0, "visuals", "sound_esp_material");
	SetupValue(cfg_vars.visuals.glow_thikness, 90, "visuals", "glow_thikness");
	SetupValue(cfg_vars.visuals.bullet_tracer_life, 3.f, "visuals", "bullet_tracer_life");
	SetupValue(cfg_vars.visuals.bullet_tracer_width, 4.5f, "visuals", "bullet_tracer_width");
	SetupValue(cfg_vars.visuals.bullet_tracer_fade, .5f, "visuals", "bullet_tracer_fade");
	SetupValue(cfg_vars.visuals.fl_sound_esp_life, 1.5f, "visuals", "fl_sound_esp_life");
	SetupValue(cfg_vars.visuals.fl_sound_esp_width, 3.f, "visuals", "fl_sound_esp_width");
	SetupValue(cfg_vars.visuals.fl_sound_esp_radius, 3.f, "visuals", "fl_sound_esp_radius");
	SetupValue(cfg_vars.visuals.local_trails_life, 1.0f, "visuals", "local_trails_life");
	SetupValue(cfg_vars.visuals.trails_life, 1.0f, "visuals", "trails_life");
	SetupValue(cfg_vars.visuals.tracer_type, 0, "visuals", "tracer_type");
	SetupValue(cfg_vars.visuals.local_trail_type, 0, "visuals", "local_trail_type");
	SetupValue(cfg_vars.visuals.trail_type, 0, "visuals", "trail_type");
	SetupValue(cfg_vars.visuals.pearlsecent, 12, "visuals", "pearlsecent");
	SetupValue(cfg_vars.visuals.phongboost, 1, "visuals", "phongboost");
	SetupValue(cfg_vars.visuals.rimlight, 1, "visuals", "rimlight");
	SetupValue(cfg_vars.visuals.rimlight_boost, 1, "visuals", "rimlight_boost");
	SetupValue(cfg_vars.visuals.envmaplightscale, 1.f, "visuals", "envmaplightscale");
	SetupValue(cfg_vars.visuals.rimlightexponent, 1, "visuals", "rimlightexponent");
	SetupValue(cfg_vars.visuals.alpha_reducing, 50.f, "visuals", "alpha_reducing");
	SetupValue(cfg_vars.visuals.model_color, Color(255,255,255,255), "visuals", "model_color");
	SetupValue(cfg_vars.visuals.chams_invisible, Color(255,255,255,255), "visuals", "chams_invisible");
	SetupValue(cfg_vars.visuals.weapon_color, Color(255,255,255,255), "visuals", "weapon_color");
	SetupValue(cfg_vars.visuals.hand_color, Color(255,255,255,255), "visuals", "hand_color");
	SetupValue(cfg_vars.visuals.dlight_col, Color(255,255,255,255), "visuals", "dlight_col");
	SetupValue(cfg_vars.visuals.box_color, Color(255,0,255,0), "visuals", "box_color");
	SetupValue(cfg_vars.visuals.post_effect_col, Color(255,255,255,255), "visuals", "post_effect_col");
	SetupValue(cfg_vars.visuals.local_trails_col, Color(255,255,255,255), "visuals", "local_trails_col");
	SetupValue(cfg_vars.visuals.skeleton_color, Color(255,255,255,255), "visuals", "skeleton_color");
	SetupValue(cfg_vars.visuals.glow_color, Color(255,255,255,255), "visuals", "glow_color");
	SetupValue(cfg_vars.visuals.name_esp_color, Color(255,255,255,255), "visuals", "name_esp");
	SetupValue(cfg_vars.visuals.line_esp_color, Color(255,255,255,255), "visuals", "line_esp");
	SetupValue(cfg_vars.visuals.sleeves_c_color, Color(255,255,255,255), "visuals", "sleeves_c_color");
	SetupValue(cfg_vars.visuals.bullet_tracers_col, Color(255,255,255,255), "visuals", "bullet_tracers_col");
	SetupValue(cfg_vars.visuals.chams_contrast, Color(255,255,255,255), "visuals", "chams_contrast");
	SetupValue(cfg_vars.visuals.trails_col, Color(255,255,255,255), "visuals", "trails_col");
	SetupValue(cfg_vars.visuals.box_background_color, Color(55,255,255,255), "visuals", "box_background_color");
	SetupValue(cfg_vars.visuals.hp_esp_background_col, Color(189, 46, 46), "visuals", "hp_esp_background_col");
	SetupValue(cfg_vars.visuals.hp_esp_col, Color(48, 194, 46), "visuals", "hp_esp_col");
	SetupValue(cfg_vars.visuals.aim_light_color, Color(48, 194, 46), "visuals", "aim_light_color");
	SetupValue(cfg_vars.visuals.spread_color, Color(120,120,120), "visuals", "spread_color");
	/*MISC*/
	SetupValue(cfg_vars.misc.enabled,false, "misc", "enabled");
	SetupValue(cfg_vars.misc.fakelags,false, "misc", "fakelags");
	SetupValue(cfg_vars.misc.bunnyhope,false, "misc", "bunnyhope");
	SetupValue(cfg_vars.misc.auto_strafe,false, "misc", "auto_strafe");
	SetupValue(cfg_vars.misc.radar_hack,false, "misc", "radar_hack");
	SetupValue(cfg_vars.misc.moon_walk,false, "misc", "moon_walk");
	SetupValue(cfg_vars.misc.hit_sound,false, "misc", "hit_sound");
	SetupValue(cfg_vars.misc.hit_marker,false, "misc", "hit_marker");
	SetupValue(cfg_vars.misc.auto_scope,false, "misc", "auto_scope");
	SetupValue(cfg_vars.misc.slow_walk,false, "misc", "slow_walk");
	SetupValue(cfg_vars.misc.kill_spam,false, "misc", "kill_spam");
	SetupValue(cfg_vars.misc.thirdperson,false, "misc", "third person");
	SetupValue(cfg_vars.misc.extend_backtrack,false, "misc", "extend_backtrack");
	SetupValue(cfg_vars.misc.fakelag_factor,0, "misc", "fakelag_factor");
	SetupValue(cfg_vars.misc.fakelag_type,1, "misc", "fakelag_factor");
	SetupValue(cfg_vars.misc.slow_walk_power,0.0f, "misc", "slow_walk_power");
	SetupValue(cfg_vars.misc.viewmodel_fov,68, "misc", "viewmodel_fov");
	SetupValue(cfg_vars.misc.fov,68, "misc", "fov");
	SetupValue(cfg_vars.misc.third_person_distance,100, "misc", "third_person_distance");
	SetupValue(cfg_vars.misc.viewmodel_offset_x,1, "misc", "viewmodel_offset_x");
	SetupValue(cfg_vars.misc.viewmodel_offset_y,2, "misc", "viewmodel_offset_y");
	SetupValue(cfg_vars.misc.viewmodel_offset_z,1, "misc", "viewmodel_offset_z");
	SetupValue(cfg_vars.misc.curr_hitsound,0, "misc", "curr_hitsound");
	SetupValue(cfg_vars.misc.thirdperson_key,0, "misc", "thirdperson_key");
	SetupValue(cfg_vars.show_menu,1, "menu", "show_menu");



	SetupValue(cfg_vars.legitbot.current_pick,0, "legitbot", "pick");

	cfg_vars.legitbot.pistol_legit_wpn_data = legit_weapon_settings("pistol_settings");
	SetLabel(pistol_legit_wpn_data, legitbot, false, enabled);
	SetLabel(pistol_legit_wpn_data, legitbot, false, p_silent);
	SetLabel(pistol_legit_wpn_data, legitbot, false, silent);
	SetLabel(pistol_legit_wpn_data, legitbot, false, rcs_standalone);
	SetLabel(pistol_legit_wpn_data, legitbot, false, visibility_check);
	SetLabel(pistol_legit_wpn_data, legitbot, false, backtrack);
	SetLabel(pistol_legit_wpn_data, legitbot, false, extrapolate);
	SetLabel(pistol_legit_wpn_data, legitbot, false, engine_predict);
	SetLabel(pistol_legit_wpn_data, legitbot, false, nearest);
	SetLabel(pistol_legit_wpn_data, legitbot, false, shadow_rcs);
	SetLabel(pistol_legit_wpn_data, legitbot, false, randomize_rcs);
	SetLabel(pistol_legit_wpn_data, legitbot, false, auto_shoot);
	SetLabel(pistol_legit_wpn_data, legitbot, false, auto_stop);
	SetLabel(pistol_legit_wpn_data, legitbot, false, trigger);
	SetLabel(pistol_legit_wpn_data, legitbot, 0, nearest_hitboxes);
	SetLabel(pistol_legit_wpn_data, legitbot, 0, target_method);
	SetLabel(pistol_legit_wpn_data, legitbot, 0, curr_hitbox);
	SetLabel(pistol_legit_wpn_data, legitbot, 1, fov);
	SetLabel(pistol_legit_wpn_data, legitbot, 1, p_silent_fov);
	SetLabel(pistol_legit_wpn_data, legitbot, 0, smooth_type);
	SetLabel(pistol_legit_wpn_data, legitbot, 100, rcs_x);
	SetLabel(pistol_legit_wpn_data, legitbot, 100, rcs_y);
	SetLabel(pistol_legit_wpn_data, legitbot, 100, trigger_chance);
	SetLabel(pistol_legit_wpn_data, legitbot, 1, rcs_delay);
	SetLabel(pistol_legit_wpn_data, legitbot, 0, dist_type);
	SetLabel(pistol_legit_wpn_data, legitbot, 1.f, smooth);
	SetLabel(pistol_legit_wpn_data, legitbot, 1, kill_delay);
	SetLabel(pistol_legit_wpn_data, legitbot, 1, switch_target_delay);
	SetLabel(pistol_legit_wpn_data, legitbot, 0.85f, auto_stop_power);
	SetLabel(pistol_legit_wpn_data, legitbot, 0, trigger_key);
	SetLabel(pistol_legit_wpn_data, legitbot, 0, aimbot_key);
	SetLabel(pistol_legit_wpn_data, legitbot, 0, auto_shot_key);
	SetLabel(pistol_legit_wpn_data, legitbot, 1, trigger_delay);
	
	cfg_vars.legitbot.machine_legit_wpn_data = legit_weapon_settings("machine_settings");
	SetLabel(machine_legit_wpn_data, legitbot, false, enabled);
	SetLabel(machine_legit_wpn_data, legitbot, false, p_silent);
	SetLabel(machine_legit_wpn_data, legitbot, false, silent);
	SetLabel(machine_legit_wpn_data, legitbot, false, rcs_standalone);
	SetLabel(machine_legit_wpn_data, legitbot, false, visibility_check);
	SetLabel(machine_legit_wpn_data, legitbot, false, backtrack);
	SetLabel(machine_legit_wpn_data, legitbot, false, extrapolate);
	SetLabel(machine_legit_wpn_data, legitbot, false, engine_predict);
	SetLabel(machine_legit_wpn_data, legitbot, false, nearest);
	SetLabel(machine_legit_wpn_data, legitbot, false, shadow_rcs);
	SetLabel(machine_legit_wpn_data, legitbot, false, randomize_rcs);
	SetLabel(machine_legit_wpn_data, legitbot, false, auto_shoot);
	SetLabel(machine_legit_wpn_data, legitbot, false, auto_stop);
	SetLabel(machine_legit_wpn_data, legitbot, false, trigger);
	SetLabel(machine_legit_wpn_data, legitbot, 0, nearest_hitboxes);
	SetLabel(machine_legit_wpn_data, legitbot, 0, target_method);
	SetLabel(machine_legit_wpn_data, legitbot, 0, curr_hitbox);
	SetLabel(machine_legit_wpn_data, legitbot, 1, fov);
	SetLabel(machine_legit_wpn_data, legitbot, 1, p_silent_fov);
	SetLabel(machine_legit_wpn_data, legitbot, 0, smooth_type);
	SetLabel(machine_legit_wpn_data, legitbot, 100, rcs_x);
	SetLabel(machine_legit_wpn_data, legitbot, 100, rcs_y);
	SetLabel(machine_legit_wpn_data, legitbot, 100, trigger_chance);
	SetLabel(machine_legit_wpn_data, legitbot, 1, rcs_delay);
	SetLabel(machine_legit_wpn_data, legitbot, 0, dist_type);
	SetLabel(machine_legit_wpn_data, legitbot, 1.f, smooth);
	SetLabel(machine_legit_wpn_data, legitbot, 1, kill_delay);
	SetLabel(machine_legit_wpn_data, legitbot, 1, switch_target_delay);
	SetLabel(machine_legit_wpn_data, legitbot, 0.85f, auto_stop_power);
	SetLabel(machine_legit_wpn_data, legitbot, 0, trigger_key);
	SetLabel(machine_legit_wpn_data, legitbot, 0, aimbot_key);
	SetLabel(machine_legit_wpn_data, legitbot, 0, auto_shot_key);
	SetLabel(machine_legit_wpn_data, legitbot, 1, trigger_delay);

	cfg_vars.legitbot.shotgun_legit_wpn_data = legit_weapon_settings("shotgun_settings");
	SetLabel(shotgun_legit_wpn_data, legitbot, false, enabled);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, p_silent);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, silent);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, rcs_standalone);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, visibility_check);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, backtrack);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, extrapolate);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, engine_predict);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, nearest);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, shadow_rcs);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, randomize_rcs);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, auto_shoot);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, auto_stop);
	SetLabel(shotgun_legit_wpn_data, legitbot, false, trigger);
	SetLabel(shotgun_legit_wpn_data, legitbot, 0, nearest_hitboxes);
	SetLabel(shotgun_legit_wpn_data, legitbot, 0, target_method);
	SetLabel(shotgun_legit_wpn_data, legitbot, 0, curr_hitbox);
	SetLabel(shotgun_legit_wpn_data, legitbot, 1, fov);
	SetLabel(shotgun_legit_wpn_data, legitbot, 1, p_silent_fov);
	SetLabel(shotgun_legit_wpn_data, legitbot, 0, smooth_type);
	SetLabel(shotgun_legit_wpn_data, legitbot, 100, rcs_x);
	SetLabel(shotgun_legit_wpn_data, legitbot, 100, rcs_y);
	SetLabel(shotgun_legit_wpn_data, legitbot, 100, trigger_chance);
	SetLabel(shotgun_legit_wpn_data, legitbot, 1, rcs_delay);
	SetLabel(shotgun_legit_wpn_data, legitbot, 0, dist_type);
	SetLabel(shotgun_legit_wpn_data, legitbot, 1.f, smooth);
	SetLabel(shotgun_legit_wpn_data, legitbot, 1, kill_delay);
	SetLabel(shotgun_legit_wpn_data, legitbot, 1, switch_target_delay);
	SetLabel(shotgun_legit_wpn_data, legitbot, 0, trigger_key);
	SetLabel(shotgun_legit_wpn_data, legitbot, 0, aimbot_key);
	SetLabel(shotgun_legit_wpn_data, legitbot, 0, auto_shot_key);
	SetLabel(shotgun_legit_wpn_data, legitbot, 1, trigger_delay);
	SetLabel(shotgun_legit_wpn_data, legitbot, 0.85f, auto_stop_power);


	cfg_vars.legitbot.rifle_legit_wpn_data = legit_weapon_settings("rifle_settings");
	SetLabel(rifle_legit_wpn_data, legitbot, false, enabled);
	SetLabel(rifle_legit_wpn_data, legitbot, false, p_silent);
	SetLabel(rifle_legit_wpn_data, legitbot, false, silent);
	SetLabel(rifle_legit_wpn_data, legitbot, false, rcs_standalone);
	SetLabel(rifle_legit_wpn_data, legitbot, false, visibility_check);
	SetLabel(rifle_legit_wpn_data, legitbot, false, backtrack);
	SetLabel(rifle_legit_wpn_data, legitbot, false, extrapolate);
	SetLabel(rifle_legit_wpn_data, legitbot, false, engine_predict);
	SetLabel(rifle_legit_wpn_data, legitbot, false, nearest);
	SetLabel(rifle_legit_wpn_data, legitbot, false, shadow_rcs);
	SetLabel(rifle_legit_wpn_data, legitbot, false, randomize_rcs);
	SetLabel(rifle_legit_wpn_data, legitbot, false, auto_shoot);
	SetLabel(rifle_legit_wpn_data, legitbot, false, auto_stop);
	SetLabel(rifle_legit_wpn_data, legitbot, false, trigger);
	SetLabel(rifle_legit_wpn_data, legitbot, 0, nearest_hitboxes);
	SetLabel(rifle_legit_wpn_data, legitbot, 0, target_method);
	SetLabel(rifle_legit_wpn_data, legitbot, 0, curr_hitbox);
	SetLabel(rifle_legit_wpn_data, legitbot, 1, fov);
	SetLabel(rifle_legit_wpn_data, legitbot, 1, p_silent_fov);
	SetLabel(rifle_legit_wpn_data, legitbot, 0, smooth_type);
	SetLabel(rifle_legit_wpn_data, legitbot, 100, rcs_x);
	SetLabel(rifle_legit_wpn_data, legitbot, 100, rcs_y);
	SetLabel(rifle_legit_wpn_data, legitbot, 100, trigger_chance);
	SetLabel(rifle_legit_wpn_data, legitbot, 1, rcs_delay);
	SetLabel(rifle_legit_wpn_data, legitbot, 0, dist_type);
	SetLabel(rifle_legit_wpn_data, legitbot, 1.f, smooth);
	SetLabel(rifle_legit_wpn_data, legitbot, 1, kill_delay);
	SetLabel(rifle_legit_wpn_data, legitbot, 1, switch_target_delay);
	SetLabel(rifle_legit_wpn_data, legitbot, 0, trigger_key);
	SetLabel(rifle_legit_wpn_data, legitbot, 0, aimbot_key);
	SetLabel(rifle_legit_wpn_data, legitbot, 0, auto_shot_key);
	SetLabel(rifle_legit_wpn_data, legitbot, 1, trigger_delay);
	SetLabel(rifle_legit_wpn_data, legitbot, 0.85f, auto_stop_power);

	cfg_vars.legitbot.sniper_legit_wpn_data = legit_weapon_settings("sniper_settings");
	SetLabel(sniper_legit_wpn_data, legitbot, false, enabled);
	SetLabel(sniper_legit_wpn_data, legitbot, false, p_silent);
	SetLabel(sniper_legit_wpn_data, legitbot, false, silent);
	SetLabel(sniper_legit_wpn_data, legitbot, false, rcs_standalone);
	SetLabel(sniper_legit_wpn_data, legitbot, false, visibility_check);
	SetLabel(sniper_legit_wpn_data, legitbot, false, backtrack);
	SetLabel(sniper_legit_wpn_data, legitbot, false, extrapolate);
	SetLabel(sniper_legit_wpn_data, legitbot, false, engine_predict);
	SetLabel(sniper_legit_wpn_data, legitbot, false, nearest);
	SetLabel(sniper_legit_wpn_data, legitbot, false, shadow_rcs);
	SetLabel(sniper_legit_wpn_data, legitbot, false, randomize_rcs);
	SetLabel(sniper_legit_wpn_data, legitbot, false, auto_shoot);
	SetLabel(sniper_legit_wpn_data, legitbot, false, auto_stop);
	SetLabel(sniper_legit_wpn_data, legitbot, false, trigger);
	SetLabel(sniper_legit_wpn_data, legitbot, 0, nearest_hitboxes);
	SetLabel(sniper_legit_wpn_data, legitbot, 0, target_method);
	SetLabel(sniper_legit_wpn_data, legitbot, 0, curr_hitbox);
	SetLabel(sniper_legit_wpn_data, legitbot, 1, fov);
	SetLabel(sniper_legit_wpn_data, legitbot, 1, p_silent_fov);
	SetLabel(sniper_legit_wpn_data, legitbot, 0, smooth_type);
	SetLabel(sniper_legit_wpn_data, legitbot, 100, rcs_x);
	SetLabel(sniper_legit_wpn_data, legitbot, 100, rcs_y);
	SetLabel(sniper_legit_wpn_data, legitbot, 100, trigger_chance);
	SetLabel(sniper_legit_wpn_data, legitbot, 1, rcs_delay);
	SetLabel(sniper_legit_wpn_data, legitbot, 0, dist_type);
	SetLabel(sniper_legit_wpn_data, legitbot, 1.f, smooth);
	SetLabel(sniper_legit_wpn_data, legitbot, 1, kill_delay);
	SetLabel(sniper_legit_wpn_data, legitbot, 1, switch_target_delay);
	SetLabel(sniper_legit_wpn_data, legitbot, 0, trigger_key);
	SetLabel(sniper_legit_wpn_data, legitbot, 0, aimbot_key);
	SetLabel(sniper_legit_wpn_data, legitbot, 0, auto_shot_key);
	SetLabel(sniper_legit_wpn_data, legitbot, 1, trigger_delay);
	SetLabel(sniper_legit_wpn_data, legitbot, 0.85f, auto_stop_power);
}

void cfg_system::Create(std::string cfg_name)
{
	if(!this->current_cfg_name.empty())this->Save(this->current_cfg_name);
	this->Setup();
	this->Save(cfg_name);
	this->Load(this->current_cfg_name);
}

void cfg_system::SetupValue(int& value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue< int >(category, name, &value));
}

void cfg_system::SetupValue(float& value, float def, std::string category, std::string name)
{
	value = def;
	floats.emplace_back(new ConfigValue< float >(category, name, &value));
}

void cfg_system::SetupValue(Color& value, Color def, std::string category, std::string name)
{
	value = def;
	colors.emplace_back(new ConfigValue< Color >(category, name, &value));
}
void cfg_system::SetupValue(bool& value, bool def, std::string category, std::string name)
{
	value = def;
	bools.emplace_back(new ConfigValue< bool >(category, name, &value));
}

void cfg_system::Save(std::string cfg_name)
{
	static CHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path)))//ny docs
	{
		folder = std::string(path) + "\\LumenSense\\";
		file = std::string(path) + "\\LumenSense\\" + cfg_name + ".cfg";
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());

	for (auto value : colors) {//or save in DWORD as D3DCOLOR ; r = dword & 0xFFFF << 0; g = dword & 0xFFFF << 8 and etc.
		std::string buffer = "";
		buffer += std::to_string(value->value->r); buffer += ",";
		buffer += std::to_string(value->value->g); buffer += ",";
		buffer += std::to_string(value->value->b); buffer += ",";
		buffer += std::to_string(value->value->a);
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), buffer.c_str(), file.c_str());
	}
	
}

void cfg_system::Load(std::string name)
{
	static CHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path)))//ny docs
	{
		folder = std::string(path) + "\\LumenSense\\";
		file = folder + name + ".cfg";
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
	for (auto value : colors) {	
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		std::string buffer = value_l;
		int curr_pixel_option = 0;
		std::string sub_buffer = "";
		Color b = Color(0,00,0);
		for (int i = 0; i < buffer.size(); i++)
		{
			if (buffer[i] == ',')
			{
				curr_pixel_option++;
				sub_buffer = "";
			}
			else {
				sub_buffer += buffer[i];
				*b[curr_pixel_option] = std::stof(sub_buffer);
			}
		}
		*value->value = b;
	}
}
namespace fs = std::filesystem;
std::vector<std::string> cfg_system::get_cfg_list()
{
	static CHAR path[MAX_PATH];
	std::string folder;
	std::vector<std::string>cfg_name_list;
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path)))//my docs
	{
		folder = std::string(path) + "\\LumenSense\\";		
	}
	for (auto& p : fs::directory_iterator(folder))
	{
		if (p.exists())
		{
			auto _path = p.path();
			auto _extension = _path.extension();
			std::string buffer_ext= _extension.generic_string();
			if (!strcmp(buffer_ext.c_str(), ".cfg"))
			{
				std::string clipped_path = _path.generic_string();
				for (int i = 0; i < folder.size(); i++)
				{
					clipped_path.erase(clipped_path.begin());
				}
				for (int i = 0; i < strlen(".cfg"); i++)
				{
					clipped_path.pop_back();
				}
				/*for (int  i = 0; i < clipped_path.size(); i++)
				{
					if (clipped_path[i] == '\ ')
					{
						clipped_path.erase(clipped_path.begin() + i);
					}
				}*/
				cfg_name_list.push_back(clipped_path);
			}
		}
	}
	return cfg_name_list;
}