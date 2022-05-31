#include "g_event.h"
#include "sdk/sdk.hpp"
#include "backend/cfg_system.hpp"
#include "backend/bring_sound.h"
#include "backend/roblox_sound.h"
#include "backend/hentai_sound.h"
#include "backend/wot_hitsound.h"
#include "visuals.hpp"
#include <playsoundapi.h>
#include <Windows.h>
#include "sdk/csgo.h"
#include "../LumenSense/utils/blyat_sound.h"
#pragma comment(lib, "Winmm.lib") 
void game_event_handler::FireGameEvent(i_game_event* event)
{	
	if (!event || !csgo_data.local || !g_engine->IsInGame())return;
	const char* name = event->get_name();
	if (!strcmp(name, "bullet_impact")) {
		c_base_player* firing_player = static_cast<c_base_player*>(g_entity_list->GetClientEntity(g_engine->GetPlayerForUserID(event->get_int("userid"))));
		if (firing_player && firing_player == csgo_data.local) {
			last_firebullet_position = Vector(event->get_int("x"), event->get_int("y"), event->get_int("z"));
			if (cfg_vars.visuals.draw_bullet_tracers) {
				g_render_beam->draw_beam_custom((firing_player->get_origin() + firing_player->get_eye_position()) - Vector(2,0,0), last_firebullet_position, 2, cfg_vars.visuals.bullet_tracer_life, cfg_vars.visuals.bullet_tracer_width, cfg_vars.visuals.bullet_tracer_width, cfg_vars.visuals.bullet_tracer_fade, cfg_vars.visuals.tracer_type, cfg_vars.visuals.bullet_tracers_col);
			}
		}
	}
	if (!strcmp(name, "player_hurt")) {
		IClientEntity* attacker = g_entity_list->GetClientEntity(g_engine->GetPlayerForUserID(event->get_int("attacker")));
		c_base_player* target = static_cast<c_base_player*>(g_entity_list->GetClientEntity(g_engine->GetPlayerForUserID(event->get_int("userid"))));
		if (!attacker || !target) return;
		if (attacker != csgo_data.local)return;
		Ray_t ray;
		CTraceFilter filter;
		filter.pSkip = csgo_data.local;
		CGameTrace trace;
		c_base_player* attack = static_cast<c_base_player*>(attacker);
		ray.Init(attack->get_eye_position() + attack->get_origin(), last_firebullet_position);
		g_trace_gay->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &trace);
		Vector hitted = target->get_hitbox_position(trace.hitbox);
		int recivied_dmg = event->get_int("dmg_health");
		hurt_data buffer;
		buffer.dmg = recivied_dmg;
		buffer.hurted_player = target;
		buffer.hit_time = g_global_vars->curtime;
		buffer.position = hitted;
		hit_data[target->get_index()].emplace_back(buffer);
		if (cfg_vars.misc.hit_sound)
		{
			switch (cfg_vars.misc.curr_hitsound)
			{
			case 0:
				PlaySoundA(wot, NULL, SND_ASYNC | SND_MEMORY);
				break;
			case 1:
				PlaySoundA(bring_hitsound, NULL, SND_ASYNC | SND_MEMORY);
				break;
			case 2:
				PlaySoundA(roblox_sound, NULL, SND_ASYNC | SND_MEMORY);
				break;
			case 3:
				PlaySoundA(hitsound, NULL, SND_ASYNC | SND_MEMORY);
				break;	
			case 4:
				PlaySoundA(blyat_hit, NULL, SND_ASYNC | SND_MEMORY);
				break;
			}
		}
		if( cfg_vars.visuals.hit_ring)g_render_beam->draw_beam_ring(1.1f, 450.f, Color(255, 55, 44), target->get_origin(), 10.f);		
	}
	if (!strcmp(name, "player_death")) {
		c_base_player* hurt = static_cast<c_base_player*>(g_entity_list->GetClientEntity(g_engine->GetPlayerForUserID(event->get_int("attacker"))));
		if (hurt && csgo_data.local && hurt == csgo_data.local)
		{
			csgo_data.lask_kill_time = g_global_vars->curtime;
			if(cfg_vars.visuals.kill_effect)csgo_data.local->p_health_shot_expirationTime() = g_global_vars->curtime + 1.f;
			if(cfg_vars.misc.kill_spam)g_engine->ExecuteClientCmd("say slapped by LumenSense. think about it. 1 $ cheat owned you");
		}
	}
	if (!strcmp(name, "round_end")) {
		/*reset all*/
		for (int j = 0; j < 64; j++) {
			hit_data[j].clear();
			csgo_data.alpha_data[j] = 0.0f;
			csgo_data.old_alpha_data[j] = 0.0f;
			csgo_data.old_curtime_for_alpha[j] = 0.0f;
		}
	}
	if (!strcmp(name, "player_footstep")) {	
		c_base_player* idiot_which_loud = static_cast<c_base_player*>(g_entity_list->GetClientEntity(g_engine->GetPlayerForUserID(event->get_int("userid"))));
		if (idiot_which_loud && idiot_which_loud->is_enemy() && cfg_vars.visuals.sound_esp && idiot_which_loud->hp() > 0 && !idiot_which_loud->dormant())
		{
			g_render_beam->draw_beam_ring(cfg_vars.visuals.sound_esp_material,idiot_which_loud->get_origin(), cfg_vars.visuals.fl_sound_esp_radius, cfg_vars.visuals.fl_sound_esp_life, cfg_vars.visuals.fl_sound_esp_width, .5f, cfg_vars.visuals.sound_esp_color);
		} 		
	}
}
void game_event_handler::Init()
{
	g_event_manager->add_listener(this, "player_hurt", false);
	g_event_manager->add_listener(this, "player_death", false);
	g_event_manager->add_listener(this, "bullet_impact", false);
	g_event_manager->add_listener(this, "round_end", false);
	g_event_manager->add_listener(this, "player_footstep", false);
}

void game_event_handler::shutdown()
{
	g_event_manager->remove_listener(this);
}

void game_event_handler::on_paint()
{	
	if (!cfg_vars.misc.hit_marker)return;
	for (int j = 0; j < 64; j++) {
		for (int i = 0; i < hit_data[j].size(); ++i)
		{
			hurt_data data = hit_data[j].at(i);
			if (data.hit_time + 1.f < g_global_vars->curtime)
			{
				hit_data[j].erase(hit_data[j].begin() + i);
			}
		}
	}
	int marker_size = 6;
	for (int j = 0; j < 64; j++) {
		for (int i = 0; i < hit_data[j].size(); i++)
		{
			hurt_data& b = hit_data[j].at(i);
			Vector d2_hitbox;
			if (!move_hit_damage_vector_data[j].IsValid())
			{
				move_hit_damage_vector_data[j] = Vector(utils::random_between_two_float(-5.f, 3.f), utils::random_between_two_float(-10.f, -3.f), 0);
			}
			Vector move = move_hit_damage_vector_data[j];	
			utils::get().WorldToScreen(b.position, d2_hitbox);
			Vector buffer = d2_hitbox;
			buffer += (move * ((g_global_vars->curtime - b.hit_time) / .6f * 15.f));
			g_surface->DrawT(buffer.x, buffer.y, Color(255, 225, 0, 75), g_visuals.verdana_big, true, std::to_string(b.dmg).c_str());

			g_surface->DrawSetColor(210, 210, 211, 255);
			g_surface->DrawLine(d2_hitbox.x - marker_size, d2_hitbox.y - marker_size, d2_hitbox.x - (marker_size / 4.f), d2_hitbox.y - (marker_size / 4.f));//left up
			g_surface->DrawLine(d2_hitbox.x - marker_size, d2_hitbox.y + marker_size, d2_hitbox.x - (marker_size / 4.f), d2_hitbox.y + (marker_size / 4.f));
			g_surface->DrawLine(d2_hitbox.x + (marker_size / 4.f), d2_hitbox.y + (marker_size / 4.f), d2_hitbox.x + marker_size, d2_hitbox.y + marker_size);
			g_surface->DrawLine(d2_hitbox.x + (marker_size / 4.f), d2_hitbox.y - marker_size / 4.f, d2_hitbox.x + marker_size, d2_hitbox.y - marker_size);//right up
		}
	}	
}
