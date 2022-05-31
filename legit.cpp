#include "legit.h"
#include "sdk/sdk.hpp"
#include <random>
#include <sdk\csgo.h>
#include <backend\cfg_system.hpp>
#include "interfaces/ConVar.h"
#include "engine_prediction.h"
void legit::triggerbot(c_user_cmd* cmd)
{
	static DWORD detect_time = GetTickCount();
	c_base_combat_weapon* weapon = csgo_data.local->get_active_weapon();
	if (!weapon)return;
	if (!GetAsyncKeyState(cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->trigger_key) && cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->trigger_key != 0)
	{
		detect_time = GetTickCount();
		return;
	}
	if (csgo_data.local->hp() <= 0)return;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = csgo_data.local;
	CGameTrace trace;	
	Vector eyes, angles, distance;
	eyes = csgo_data.local->get_eye_position_f();
	angles = cmd->viewangles;
	angles = angles.angle_vector();
	angles *= weapon->GetCSWeaponData()->flRange;

	// end point = our eye position + shooting range.
	distance = eyes + angles;
	ray.Init(eyes, distance);

	g_trace_gay->TraceRay(ray, 0x46004003, &filter,&trace);
	c_base_entity* entity = reinterpret_cast<c_base_entity*>(trace.hit_entity);
	if (!entity)
		return;
	if (entity->GetClientClass()->m_ClassID != ClassId_CCSPlayer)//40 == player
	{
		detect_time = GetTickCount();
		return;
	}
		
	c_base_player* player = reinterpret_cast<c_base_player*>(entity);
	auto valid_player = [&player]() -> bool{
		// nullptr check
		if (!player)
			return false;

		// rest in pepperoni.
		if (player->hp() <= 0)
			return false;

		// target is dormant.
		if (player->dormant())
			return false;

		// target is not an enemy.
		if (!player->is_enemy())
			return false;

		return true;
	};
	float chance_to_hit = (100.0f - float(cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->trigger_chance - 1)) * 0.65f * 0.01125f;//99.f == chance
	bool d = !(weapon->get_inaccuracy() >= chance_to_hit) && valid_player();
	if (d)
	{		
		if (GetTickCount() - detect_time < cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->trigger_delay)
		{
			return;
		}
		if (weapon->GetCSWeaponData()->bFullAuto)
		{			
			cmd->buttons |= IN_ATTACK;
		}
		else {
			float flServerTime = csgo_data.local->get_tick_base() * g_global_vars->interval_per_tick;
			float flNextPrimaryAttack = weapon->get_next_primary_attack();
			bool bBulletTime = flNextPrimaryAttack > flServerTime;
			if (bBulletTime || csgo_data.local->get_fired_shots() == 0)
			{				
				cmd->buttons |= IN_ATTACK;
			}
			else cmd->buttons &= ~IN_ATTACK;
		}
	}
	else {
		detect_time = GetTickCount();
	}
}
void legit::rcs_standalone(c_user_cmd* cmd)
{
	if (is_aim)return;	
	c_base_combat_weapon* weapon = csgo_data.local->get_active_weapon();
	if (!weapon)return;
	CCSWeaponInfo* weapon_data = weapon->GetCSWeaponData();
	if (!weapon_data)return;
	if (!weapon_data->bFullAuto)return;
	static Vector vOldPunch = { 0.0f, 0.0f, 0.0f };
	Vector vNewPunch = csgo_data.local->get_aim_punch_angle();
	vNewPunch *= Vector(cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->rcs_x / 52.f, cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->rcs_y / 52.f, 0.f);
	vNewPunch -= vOldPunch;
	if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->randomize_rcs) vNewPunch = vNewPunch * utils::random_between_two_float(0.7f, 0.91f);
	vNewPunch *= 0.1f;	
	vNewPunch += vOldPunch;
	Vector vFinal = cmd->viewangles - (vNewPunch - vOldPunch);
	vFinal.normalize();
	vFinal.clamp();
	if (csgo_data.local->get_fired_shots() <= cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->rcs_delay)
	{
		vOldPunch = vNewPunch;
		return;
	}
	cmd->viewangles +=  (vNewPunch - vOldPunch);
	cmd->viewangles.normalize();
	cmd->viewangles.clamp();
	if(!cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->shadow_rcs)g_engine->SetViewAngles(&QAngle(vFinal));

	vOldPunch = vNewPunch;
}
void legit::aimbot(c_user_cmd* cmd, bool& b_send_packet)
{	
	float _time_t = csgo_data.in_prediction ? c_prediction_system::get().backup_curtime : g_global_vars->curtime;
	if (csgo_data.lask_kill_time + cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->kill_delay/1000.f > _time_t)return;
	c_base_combat_weapon* weapon = csgo_data.local->get_active_weapon();
	if (!weapon)return;
	CCSWeaponInfo* wpn_data = weapon->GetCSWeaponData();
	if (!wpn_data)return;
	if (wpn_data->iWeaponType == WEAPONTYPE_C4 || wpn_data->iWeaponType == WEAPONTYPE_GRENADE || wpn_data->iWeaponType == WEAPONTYPE_KNIFE)return;
	Vector oldView = cmd->viewangles;
	float oldSidemove = cmd->sidemove;
	float oldForwardmove = cmd->forwardmove;
	c_base_player* best_target = get_best_target(cmd);//goodbay perfomance cuz we  call very often
	if (!best_target)
	{
		angle_before_aim = cmd->viewangles;
		aim_time = 0;
		entity_aim = 0;
		is_aim = false;		
		return;
	}
	if (cfg_vars.visuals.show_aim_target) {
		static float beam_render_time = csgo_data.in_prediction ? c_prediction_system::get().backup_curtime : g_global_vars->curtime;
		if (g_global_vars->curtime  - beam_render_time > .49f)
		{
			g_render_beam->draw_beam_ring(.5f, 85.f, cfg_vars.visuals.aim_light_color, best_target->get_origin(), 5.f);
			beam_render_time = csgo_data.in_prediction ? c_prediction_system::get().backup_curtime : g_global_vars->curtime;
		}
	}
	if (csgo_data.active_wpn_type == WEAPONTYPE_SNIPER_RIFLE && !csgo_data.local->is_scoped() && can_scope && cfg_vars.misc.auto_scope)
	{
		//if (csgo_data.local->get_velocity().Length2D() > (1.f - cfg_vars.legitbot.wpn_set_table[csgo_data.local_data.weapon_settings_type]->auto_stop_power) * 320.f)stop_movement(cmd);
		cmd->buttons &= ~IN_ATTACK;
		cmd->buttons |= IN_ATTACK2;
		return;
	}
	static ConVar* weapon_recoil = g_cvar->FindVar("weapon_recoil_scale");
	float recoild_scale = weapon_recoil->GetFloat();
	float flServerTime = csgo_data.local->get_tick_base() * g_global_vars->interval_per_tick;	
	float flNextPrimaryAttack = weapon->get_next_primary_attack();
	bool bBulletTime = flNextPrimaryAttack > flServerTime;

	if (GetAsyncKeyState(VK_LBUTTON) && cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_stop)
	{
		if (csgo_data.local->get_velocity().Length2D() > (1.f - cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_stop_power) * 320.f && cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_stop)
			stop_movement(cmd);
	}
	if ((GetAsyncKeyState(cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->aimbot_key) || cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->aimbot_key == 0) && !cfg_vars.show_menu && !cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_shoot)
	{		
		if (aim_player != old_aim_player)
		{
			last_switched_entity_time = g_global_vars->curtime;
			old_entity_aim = entity_aim;
			old_aim_player = aim_player;
			aim_time = 0;
			angle_before_aim = cmd->viewangles;
		}		
		Vector aimbot_angles = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->extrapolate ? best_target->GetPredicted(best_target->get_hitbox_position(get_hitbox_num_from_cfg())) : best_target->get_hitbox_position(get_hitbox_num_from_cfg());
		aimbot_angles = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->nearest ? get_best_bone_nearest(cmd, best_target) :	utils::get().calculate_aim_angles(aimbot_angles, csgo_data.local->get_eye_position_f());
		is_aim = true;
		if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->p_silent && csgo_data.local->get_fired_shots() < 1)//psilent
		{
			stop_movement(cmd);
			if (!bBulletTime && (cmd->buttons & IN_ATTACK) )
			{							
				cmd->viewangles = aimbot_angles;
				cmd->viewangles.clamp();
				b_send_packet = false;
			}
			else {				
				cmd->viewangles = oldView;
				cmd->sidemove = oldSidemove;
				cmd->forwardmove = oldForwardmove;
				g_engine->SetViewAngles(&QAngle(cmd->viewangles));	
				b_send_packet = true;
			}
		}
		else {					
			aimbot_angles -= csgo_data.local->get_aim_punch_angle() * recoild_scale;
			aimbot_angles.normalize();
			smooth(cmd->viewangles, aimbot_angles);
			cmd->viewangles.clamp();
			if (!cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->silent)g_engine->SetViewAngles(&QAngle(cmd->viewangles));
		}
	}
	else if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_shoot)
	{	
		Vector aimbot_angles = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->extrapolate ? best_target->GetPredicted(best_target->get_hitbox_position(get_hitbox_num_from_cfg())) : best_target->get_hitbox_position(get_hitbox_num_from_cfg());
		aimbot_angles = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->nearest ? get_best_bone_nearest(cmd, best_target) : utils::get().calculate_aim_angles(aimbot_angles, csgo_data.local->get_eye_position_f());
		if (weapon->get_ammo() <= 0)return;
		if (csgo_data.local->get_velocity().Length2D() > (1.f - cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_stop_power) * 320.f && cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_stop)
		stop_movement(cmd);	
		if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_stop && csgo_data.local->get_velocity().Length2D() > (1.f - cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_stop_power) * 320.f)return;		
		if (wpn_data->bFullAuto)
		{			
			//if(!bBulletTime)cmd->viewangles = aimbot_angles - csgo_data.local->get_aim_punch_angle() * 2.f;;
			cmd->viewangles = aimbot_angles - csgo_data.local->get_aim_punch_angle() * recoild_scale;
			cmd->viewangles.normalize();
			cmd->viewangles.clamp();
			cmd->buttons |= IN_ATTACK;				
		}
		else {
			if (bBulletTime || csgo_data.local->get_fired_shots() == 0)
			{
				cmd->viewangles = aimbot_angles - csgo_data.local->get_aim_punch_angle() * recoild_scale;
				cmd->viewangles.normalize();
				cmd->viewangles.clamp();
				cmd->buttons |= IN_ATTACK;
			}
			else {
				cmd->buttons &= ~IN_ATTACK;
				//cmd->viewangles = oldView;
				//cmd->sidemove = oldSidemove;
				//cmd->forwardmove = oldForwardmove;
				//g_engine->SetViewAngles(&QAngle(oldView));
			}
		}
		if (!cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->silent)
		{
			g_engine->SetViewAngles(&QAngle(cmd->viewangles));
		}
	}
	else {
		is_aim = false;
		angle_before_aim = cmd->viewangles;
		aim_time = 0;
	}

	cmd->viewangles.normalize();

}
void legit::get_delta_time()
{
	static float oldServerTime = csgo_data.local->get_tick_base() * g_global_vars->interval_per_tick;
	float serverTime = csgo_data.local->get_tick_base() * g_global_vars->interval_per_tick;
	delta_time = serverTime - oldServerTime;
	oldServerTime = serverTime;
}
int timeToTicks(float time) noexcept
{
	return static_cast<int>(0.5f + time / g_global_vars->interval_per_tick);
}
float getLerp() noexcept
{
	static ConVar* updateRate = g_cvar->FindVar("cl_updaterate");
	static ConVar* maxUpdateRate = g_cvar->FindVar("sv_maxupdaterate");
	static ConVar* interp = g_cvar->FindVar("cl_interp");
	static ConVar* interpRatio = g_cvar->FindVar("cl_interp_ratio");
	static ConVar* minInterpRatio = g_cvar->FindVar("sv_client_min_interp_ratio");
	static ConVar* maxInterpRatio = g_cvar->FindVar("sv_client_max_interp_ratio");
	static ConVar* maxUnlag = g_cvar->FindVar("sv_maxunlag");
	float ratio = std::clamp(interpRatio->GetFloat(), minInterpRatio->GetFloat(), maxInterpRatio->GetFloat());

	return max(interp->GetFloat(), (ratio / ((maxUpdateRate) ? maxUpdateRate->GetFloat() : updateRate->GetFloat())));
}
float serverTime(c_user_cmd* cmd) noexcept
{
	static int tick;
	static c_user_cmd* lastCmd;

	if (cmd) {
		if (csgo_data.local && (!lastCmd || lastCmd->hasbeenpredicted))
			tick = csgo_data.local->get_tick_base();
		else
			tick++;
		lastCmd = cmd;
	}
	return tick * g_global_vars->interval_per_tick;
}
 bool valid(float simtime) noexcept
{
	static ConVar* maxUnlag = g_cvar->FindVar("sv_maxunlag");
	auto network = g_engine->GetNetChannelInfo();
	if (!network)
		return false;
	float delta = std::clamp(network->get_latency(0) + network->get_latency(1) + getLerp(), 0.f, maxUnlag->GetFloat()) - (serverTime(csgo_data.cmd) - simtime);
	return std::fabsf(delta) <= 0.2f;
}
void legit::back_track_update()
{
	if (!g_engine->IsInGame() || !g_engine->IsConnected() ||  !cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->backtrack || !csgo_data.local || csgo_data.local->hp() <= 0) {
		for (auto& record : records)
			record.clear();

		return;
	}
	for (int i = 1; i <= g_engine->GetMaxClients(); i++) {
		c_base_player* entity = static_cast<c_base_player*>(g_entity_list->GetClientEntity(i));
		if (!entity || entity == csgo_data.local || entity->dormant() || !entity->is_alive_funk() || !entity->is_enemy()) {
			records[i].clear();
			continue;
		}

		if (!records[i].empty() && (records[i].front().simulation_time == entity->get_sim_time()))
			continue;

		backtrack_data record{ };
		record.position = entity->get_origin();
		record.simulation_time = entity->get_sim_time();

		entity->SetupBones(record.matrix, 256, 0x7FF00, g_global_vars->curtime);

		records[i].push_front(record);

		while (records[i].size() > 3 && records[i].size() > static_cast<size_t>(timeToTicks(static_cast<float>(200.f) / 1000.f)))
			records[i].pop_back();

		if (auto invalid = std::find_if(std::cbegin(records[i]), std::cend(records[i]), [](const backtrack_data& rec) { return !valid(rec.simulation_time); }); invalid != std::cend(records[i]))
			records[i].erase(invalid, std::cend(records[i]));
	}

}
constexpr auto radiansToDegrees = [](float radians) constexpr noexcept { return radians * 180.0f / static_cast<float>(3.14f); };
Vector calculateRelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngles) noexcept
{
	Vector delta = destination - source;
	Vector angles{ radiansToDegrees(atan2f(-delta.z, std::hypotf(delta.x, delta.y))) - viewAngles.x,
				   radiansToDegrees(atan2f(delta.y, delta.x)) - viewAngles.y };
	angles.normalize();
	return angles;
}
void legit::back_track(c_user_cmd* cmd)
{
	
	if (!cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->backtrack)
		return;

	if (!(cmd->buttons & IN_ATTACK))
		return;

	if (!csgo_data.local)
		return;

	Vector localPlayerEyePosition = csgo_data.local->get_eye_position_f();

	float  bestFov{ 255.f };
	c_base_player* bestTarget = nullptr;
	int bestTargetIndex{ };
	Vector bestTargetOrigin{ };
	int bestRecord{ };

	const auto aimPunch = csgo_data.local->get_aim_punch_angle();

	for (int i = 1; i <= g_engine->GetMaxClients(); i++) {
		c_base_player* entity = static_cast<c_base_player*>(g_entity_list->GetClientEntity(i));
		if (!entity || entity == csgo_data.local || entity->dormant() || entity->hp() <= 0
			|| !entity->is_enemy())
			continue;

		Vector origin = entity->get_abs_origin();

		Vector angle = calculateRelativeAngle(localPlayerEyePosition, origin, cmd->viewangles);
		float fov = std::hypotf(angle.x, angle.y);
		if (fov < bestFov) {
			bestFov = fov;
			bestTarget = entity;
			bestTargetIndex = i;
			bestTargetOrigin = origin;
		}
	}

	if (bestTarget) {
		if (records[bestTargetIndex].size() <= 3)
			return;

		bestFov = 255.f;

		for (size_t i = 0; i < records[bestTargetIndex].size(); i++) {
			const backtrack_data& record = records[bestTargetIndex][i];
			if (!valid(record.simulation_time))
				continue;
			Vector angle = calculateRelativeAngle(localPlayerEyePosition, record.position, cmd->viewangles);
			float fov = std::hypotf(angle.x, angle.y);
			if (fov < bestFov) {
				bestFov = fov;
				bestRecord = i;
			}
		}
	}
	if (bestRecord) {
		const auto& record = records[bestTargetIndex][bestRecord];
		//bestTarget->set_abs_origin(record.position);
		//bestTarget->get_origin() = record.position;
		cmd->tick_count = timeToTicks(record.simulation_time + getLerp());
	}
}

Vector legit::get_best_bone_nearest(c_user_cmd* cmd, c_base_player* player)
{
	static std::vector<int>hitbox_list = { HITBOX_HEAD, HITBOX_NECK,HITBOX_PELVIS, HITBOX_BELLY, HITBOX_LOWER_CHEST, HITBOX_UPPER_CHEST, HITBOX_RIGHT_THIGH, HITBOX_LEFT_THIGH };
	float min_distance = FLT_MAX;
	Vector my_eyes = csgo_data.local->get_eye_position_f();
	Vector aim_pucnh = csgo_data.local->get_aim_punch_angle();
	Vector best_ang;
	static ConVar* pVar = g_cvar->FindVar("weapon_recoil_scale");

	for (int hitbox : hitbox_list)
	{		
		Vector position = player->get_hitbox_position(hitbox);
		position = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->extrapolate ? player->GetPredicted(position) : position;
		if (!csgo_data.local->can_see(player, position) && cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->visibility_check)
		{
			continue;
		}
		const Vector angle = utils::get().calculate_aim_angles(position, my_eyes);
		Vector buffer = angle;
		buffer -= aim_pucnh * pVar->GetFloat();
		buffer.normalize();
		Vector delta = cmd->viewangles - buffer;
		delta.normalize();
		float length = delta.Length();
		if (length < min_distance)
		{
			best_ang = angle;
			min_distance = length;
		}
	}
	return best_ang;
}
void legit::clear_data()
{
	entity_aim = 1;
	is_aim = false;
	aim_time = 0.0f;
	delta_time = 0.0f;
	old_entity_aim = 1;
	last_switched_entity_time = 0.0f;
	can_scope = false;
}
int legit::get_hitbox_num_from_cfg()
{
	switch (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->curr_hitbox)
	{
	case 0:
		return HITBOX_HEAD;
		break;
	case 1:
		return HITBOX_NECK;
		break;
	case 2:
		return  HITBOX_UPPER_CHEST;
		break;
	case 3:
		return HITBOX_PELVIS;
		break;
	case 4:
		return HITBOX_LOWER_CHEST;
		break;
	default:
		return HITBOX_HEAD;
		break;
	}
	return 0;
}
c_base_player* legit::get_best_target(c_user_cmd* cmd)
{
	static ConVar* weapon_r = g_cvar->FindVar("weapon_recoil_scale");
	if (!weapon_r)return nullptr;
	float weapon_recoil = weapon_r->GetFloat();
	c_base_player* best_player = nullptr;
	Vector best_angles;
	Vector curr_angles;//for all
	Vector diff_angles;//for all	
	float distance = FLT_MAX;//for distance/hp	
	float _time_t = csgo_data.in_prediction ? c_prediction_system::get().backup_curtime : g_global_vars->curtime;
	if (last_switched_entity_time + cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->switch_target_delay / 1000.f > _time_t)  return update_best_target(cmd);
	int hit_num = get_hitbox_num_from_cfg();
	Vector eye = csgo_data.local->get_origin() + csgo_data.local->get_eye_position();
	int method = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->target_method;
	for (int j = 1; j < 64; j++)
	{
		c_base_player* player = static_cast<c_base_player*>(g_entity_list->GetClientEntity(j));
		if (!player || player->hp() <= 0 || player->dormant() || !player->is_enemy() || player == csgo_data.local)continue;
		Vector hitbox_pos = player->GetPredicted(player->get_hitbox_position(hit_num));
		if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->visibility_check && !csgo_data.local->can_see(player, hitbox_pos) && !cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->nearest) continue;
		curr_angles = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->nearest ? get_best_bone_nearest(cmd, player) :  utils::get().calculate_aim_angles(hitbox_pos, eye);
		if (!curr_angles.IsValid())
		{
			continue;
		}
		float fov = get_fov(j, curr_angles);
		float curr_distance = FLT_MAX;//for distance/hp		
		if (method == 0)
		{			
			Vector my_angles = cmd->viewangles;
			my_angles += csgo_data.local->get_aim_punch_angle() * weapon_recoil;
			diff_angles = my_angles - curr_angles;
			diff_angles.normalize();			
			if (fabs(diff_angles.y) < fov && fabs(diff_angles.x) < fov)
			{
				curr_distance = diff_angles.Length();
			}
		}
		else if (method == 1)
		{

		}

		if (curr_distance < distance)
		{
			distance = curr_distance;
			best_player = player;
			entity_aim = j;
			aim_player = player;
			can_scope = false;// this + 
			if (diff_angles.y <= fov / 2.f && !csgo_data.local->is_scoped())can_scope = true;		//		 +this = genius code insted if/else
		}
	}
	return best_player;
}
c_base_player* legit::update_best_target(c_user_cmd* cmd)
{
	if (!aim_player || aim_player->hp() <= 0)return nullptr;
	int hit_num = get_hitbox_num_from_cfg();
	Vector hitbox_pos = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->extrapolate ? aim_player->GetPredicted(aim_player->get_hitbox_position(hit_num)) : aim_player->get_hitbox_position(hit_num);
	Vector curr_angles = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->nearest ? get_best_bone_nearest(cmd, aim_player) : utils::get().calculate_aim_angles(hitbox_pos, csgo_data.local->get_eye_position_f());
	if (!cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->nearest)
	{
		if (csgo_data.local->can_see(aim_player, hitbox_pos))
		{
			aim_time = 0.f;
			aim_player = nullptr;
			angle_before_aim = cmd->viewangles;
			last_switched_entity_time = 0;
			return nullptr;
		}
	}
	if (!curr_angles.IsValid()) {
		aim_time = 0.f;
		aim_player = nullptr;
		angle_before_aim = cmd->viewangles;
		last_switched_entity_time = 0;
		return nullptr;
	}
	Vector diff_angles = cmd->viewangles - curr_angles;
	diff_angles.normalize();
	float fov;
	if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->p_silent && csgo_data.local->get_fired_shots() < 1)fov = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->p_silent_fov;
	else fov = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->fov;
	fov = csgo_data.local->is_scoped() ? fov / 15.f : fov / 3.f;
	if (fabs(diff_angles.y) < fov && fabs(diff_angles.x) < fov)
	{
		return aim_player;
	}
}
float legit::get_fov(int i, Vector ang)
{
	if (!csgo_data.local || !csgo_data.cmd)return 0.0f;
	float fov = 0.0f;
	if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->dist_type == 0)
	{
		if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->p_silent && csgo_data.local->get_fired_shots() < 1)fov = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->p_silent_fov;
		else fov = cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->fov;
		fov = csgo_data.local->is_scoped() ? fov / 7.5f : fov / 3.f;
	}
	else {
		QAngle view_ang;
		g_engine->GetViewAngles(&view_ang);
		Vector angles_delta = ang - Vector(view_ang.pitch, view_ang.yaw, view_ang.roll);
		angles_delta.normalize();
		return sin(DEG2RAD(angles_delta.Length()) / 2.0f) * 180.0f / 5.f;
	}
	
	return fov;
}
void legit::stop_movement(c_user_cmd* cmd)
{
	if (csgo_data.local->get_movement_type() != MOVETYPE_WALK)
		return; 
	Vector hvel = csgo_data.local->get_velocity();
	hvel.z = 0;
	float speed = hvel.Length2D();
	if (speed < 1.f)
	{
		cmd->forwardmove = 0.f;
		cmd->sidemove = 0.f;
		return;
	}	
	float accel = 5.5f;//convar
	float maxSpeed = 320.f;
	float max_accelspeed = accel * g_global_vars->interval_per_tick * maxSpeed;

	float wishspeed{};	
	if (speed - max_accelspeed <= -1.f)
	{		
		wishspeed = max_accelspeed / (speed / (accel * g_global_vars->interval_per_tick));
	}
	else
	{
		wishspeed = max_accelspeed;
	}
	Vector ndir = (hvel * -1.f).VectorAngles();
	ndir.y = cmd->viewangles.y - ndir.y; // Relative to local view
	ndir = ndir.angle_vector(); // Back to vector, y'all

	cmd->forwardmove = ndir.x * wishspeed;
	cmd->sidemove = ndir.y * wishspeed;
}
void legit::smooth(Vector& src, Vector dst)
{
	if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->smooth == 1.0f) { src = dst; return; }
	Vector delta = angle_before_aim - dst;
	delta.normalize();
	switch (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->smooth_type)
	{
	case 0: {
		float percent = (aim_time / (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->smooth / 10.f));
		if (percent > 1.0f)percent = 1.f;
		aim_time += delta_time + utils::random_between_two_float(-0.02f, 0.02f);
		src = angle_before_aim - (delta * percent);
		src.normalize();
		src.clamp();
		break;
	}
	case 1: {
		delta = src - dst;
		delta.normalize();
		delta /= cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->smooth;
		delta *= utils::random_between_two_float(0.01f, 0.05f);
		delta.normalize();
		src -= delta;
		src.clamp();
		break;
	}
	case 2:
	{
		delta = src - dst;
		delta.normalize();
		aim_time += delta_time + utils::random_between_two_float(-0.05f, 0.01f);
		float percent = (aim_time / (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->smooth / 10.f));
		if (percent > 1.0f)percent = 1.f;
		src.x = src.x - (delta.x + utils::random_between_two_float(-0.2f, 0.05f)) * (percent/25.f);
		src.y = src.y - (delta.y + utils::random_between_two_float(-0.2f, 0.05f)) * percent;	
		src.normalize();
		src.clamp();
		break;
	}
	}

}