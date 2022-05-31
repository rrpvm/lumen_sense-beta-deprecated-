#include "misc.h"
#include "sdk/sdk.hpp"
#include "sdk/csgo.h"
#include <backend\cfg_system.hpp>
#include "legit.h"
#include "engine_prediction.h"
void misc::gravity_walk(c_user_cmd* cmd)
{
	if (cmd->forwardmove > 0)
	{
		cmd->buttons |= IN_BACK;
		cmd->buttons &= ~IN_FORWARD;
	}

	if (cmd->forwardmove < 0)
	{
		cmd->buttons |= IN_FORWARD;
		cmd->buttons &= ~IN_BACK;
	}

	if (cmd->sidemove < 0)
	{
		cmd->buttons |= IN_MOVERIGHT;
		cmd->buttons &= ~IN_MOVELEFT;
	}

	if (cmd->sidemove > 0)
	{
		cmd->buttons |= IN_MOVELEFT;
		cmd->buttons &= ~IN_MOVERIGHT;
	}
}
void misc::bunny_hop(c_user_cmd* cmd)
{
	if (!csgo_data.local || !g_engine->IsInGame() || csgo_data.local->hp() <= 0)
		return;
	if (!(csgo_data.local->get_flags() & FL_ONGROUND) && GetAsyncKeyState(VK_SPACE) && cfg_vars.misc.auto_strafe)
	{
		float velocity_length = csgo_data.local->get_velocity().Length2D();
		cmd->forwardmove = (10000.f / velocity_length > 450.f) ? 450.f : 10000.f / velocity_length;//forward /backward
		cmd->sidemove = (cmd->mousedx != 0) ? (cmd->mousedx < 0.0f) ? -450.f : 450.f : (cmd->command_number % 2) == 0 ? -450.f : 450.f;//strafe left-right
	}
	static bool bLastJumped = false;
	static bool bShouldFake = false;

	if (!bLastJumped && bShouldFake)
	{
		bShouldFake = false;
		cmd->buttons |= IN_JUMP;
	}
	else if (cmd->buttons & IN_JUMP)
	{
		if (csgo_data.local->get_flags() & FL_ONGROUND)
			bShouldFake = bLastJumped = true;
		else
		{
			cmd->buttons &= ~IN_JUMP;
			bLastJumped = false;
		}
	}
	else
		bShouldFake = bLastJumped = false;
}
void misc::fake_lags(bool& bSendPacket)
{
	if (!csgo_data.local || !g_engine->IsConnected() || !csgo_data.local->is_alive())return;
	
	if (!g_engine->IsInGame() || !csgo_data.local || !cfg_vars.misc.fakelags)
	{
		bSendPacket = true;
		return;
	}	
	int chocked = csgo_data.local->get_chocked_commands();
	if (cfg_vars.misc.fakelag_type == 0) {
		if (chocked <= cfg_vars.misc.fakelag_factor)
		{
			bSendPacket = false;
		}
		else bSendPacket = true;
	}
	else if(cfg_vars.misc.fakelag_type == 1){
		static bool m_on_peek = false;
		if (m_on_peek)
		{
			bSendPacket = !(chocked < cfg_vars.misc.fakelag_factor);
			if (bSendPacket)
				m_on_peek = false;
			return;
		}
		float velocity_length = csgo_data.local->get_velocity().Length();
		if (velocity_length <= 110.f)
			return;
		ICollideable* pCollidable = csgo_data.local->GetCollideable();
		if (!pCollidable)
			return;
		Vector min, max;
		min = pCollidable->OBBMins();
		max = pCollidable->OBBMaxs();
		min += csgo_data.local->get_origin();
		max += csgo_data.local->get_origin();
		Vector center = (min + max) * 0.5f;
		for (int i = 0; i <= g_engine->GetMaxClients(); i++)
		{
			c_base_player* p_entity = static_cast<c_base_player*>(g_entity_list->GetClientEntity(i));
			if (!p_entity || p_entity->hp() <= 0 || p_entity->dormant() || csgo_data.local == p_entity)
				continue;
			if (!p_entity->is_enemy())
				continue;
			c_base_combat_weapon* ent_weapon = p_entity->get_active_weapon();
			if (!ent_weapon || ent_weapon->get_ammo() <= 0)
				continue;

			CCSWeaponInfo* weapon_data = ent_weapon->GetCSWeaponData();
			if (!weapon_data || weapon_data->iWeaponType == WEAPONTYPE_KNIFE || weapon_data->iWeaponType >= WEAPONTYPE_C4)
				continue;

		/*	Vector eye_pos = p_entity->get_eye_position_f();
			Vector direction;
			direction = p_entity->GetEyeAngles().angle_vector();
			direction.NormalizeInPlace();
			Vector hit_point;
			bool hit = utils::get().IntersectionBoundingBox(eye_pos, direction, min, max, &hit_point);

			if (hit && eye_pos.DistTo(hit_point) <= weapon_data->flRange)
			{
				Ray_t ray;
				trace_t tr;
				CTraceFilter filter;
				filter.pSkip = p_entity;
				ray.Init(eye_pos, hit_point);

				g_trace_gay->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &tr);
				tr.DidHit();
				if (tr.contents & CONTENTS_WINDOW) //skip windows XPPPP
				{ 																						// at this moment, we dont care about local player
					filter.pSkip = tr.hit_entity;
					ray.Init(tr.endpos, hit_point);
					g_trace_gay->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &tr);
				}

				if (tr.fraction == 1.0f || tr.hit_entity == csgo_data.local)
				{
					m_on_peek = true;
					break;
				}
			}*/
			if (p_entity->can_see(csgo_data.local, csgo_data.local->get_hitbox_position(HITBOX_HEAD)))
			{
				m_on_peek = true;
				break;
			}
		}
	}
	if (bSendPacket)
	{
		if (csgo_data.local->SetupBones(last_net_matrix, 128, 0x00000100, g_global_vars->curtime))
		{
			last_net_origin = csgo_data.local->get_origin();
		}
	}
}
void misc::radar_hack()
{
	for (int i = 1; i < 64; i++)
	{
		c_base_player* player = static_cast<c_base_player*>(g_entity_list->GetClientEntity(i));
		if (!player || player->hp() <= 0 || !player->is_enemy())continue;
		*player->get_p_bSpotted() = true;
	}
}
void misc::slow_walk(c_user_cmd* cmd)
{
	if (!csgo_data.local)return;
	if (csgo_data.local->hp() <= 0)return;
	if (csgo_data.local->get_velocity().Length2D() > cfg_vars.misc.slow_walk_power)
	{
		g_legitbot.stop_movement(cmd);
	}
}

