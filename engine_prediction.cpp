#include "engine_prediction.h"
#include "utils/utils.h"
#include "sdk/sdk.hpp"
#include "interfaces/IConVar.h"
#include "interfaces/ConVar.h"
#include <sdk\csgo.h>
c_prediction_system::c_prediction_system()
{
	prediction_random_seed = *reinterpret_cast<uint32_t**>(reinterpret_cast<uint32_t>(utils::get().PatternScan(GetModuleHandle("client.dll"), "A3 ? ? ? ? 66 0F 6E 86")) + 1);
	prediction_player = *reinterpret_cast<c_base_player***>(reinterpret_cast<uint32_t>(utils::get().PatternScan(GetModuleHandle("client.dll"), "89 35 ? ? ? ? F3 0F 10 48")) + 2);
	_md5_pseudo_random = reinterpret_cast<md5_pseudo_random>(utils::get().PatternScan(GetModuleHandle("client.dll"), "55 8B EC 83 E4 F8 83 EC 70 6A"));
}
/*
void c_prediction_system::initial(c_base_player* local, c_user_cmd* cmd, int32_t seq)
{
	static c_move_data data{ };
	if (!csgo_data.local)return;
	c_base_combat_weapon* weapon = csgo_data.local->get_active_weapon();
	if (!weapon)return;
	csgo_data.in_prediction = true;
	//csgo_data.local->SetCurrentCommand(cmd);



	// backup globals.
	backup_curtime = g_global_vars->curtime;
	backup_frametime = g_global_vars->frametime;

	if (!last_cmd || last_cmd->hasbeenpredicted)
		seq_diff = seq - local->get_tick_base();
	tick_base = max(local->get_tick_base(), seq - seq_diff);
	last_cmd = cmd;

	// set globals appropriately.
	g_global_vars->curtime = ticks_to_time(local->get_tick_base() * g_global_vars->interval_per_tick);
	g_global_vars->frametime = g_global_vars->interval_per_tick;
	// set target player ( host ).
	backup_tickbase = local->get_tick_base();
	backup_origin = local->get_origin();
	backup_aim_punch = local->get_aim_punch_angle();
	//backup_aim_punch_velocity = local->get_punch_angle_vel();
	backup_view_offset = local->get_eye_position();
	backup_accuracy_penalty = weapon->get_accuracy_penalty();
	*prediction_random_seed = _md5_pseudo_random(cmd->command_number) & 0x7FFFFFFF;
	// backup flags, velocity and user cmd.
	original_cmd = *cmd;
	unpredicted_flags = local->get_flags();
	unpredicted_velocity = local->get_velocity();
	local->run_pre_think();
	local->run_think();
	memset(&move_data, 0, sizeof(c_move_data));
	g_pPrediction->SetupMove(local, cmd, g_pMoveHelper, &move_data);
	unpredicted_move_data = move_data;
	animation_info[cmd->command_number % 150];
	animation_info[cmd->command_number % 150].cmd = cmd;
	animation_info[cmd->command_number % 150].curtime = g_global_vars->curtime;
	animation_info[cmd->command_number % 150].move_data = unpredicted_move_data;
	animation_info[cmd->command_number % 150].flags = local->get_flags();
	animation_info[cmd->command_number % 150].ground_entity = local->h_ground_entity();

	*prediction_player = local;
	move_data = animation_info[cmd->command_number % 150].move_data;
	g_global_vars->curtime = animation_info[cmd->command_number % 150].curtime;
	*prediction_random_seed = _md5_pseudo_random(cmd->command_number) & 0x7FFFFFFF;
	local->get_aim_punch_angle() = backup_aim_punch;
	local->get_eye_position() = backup_view_offset;
	weapon->get_accuracy_penalty() = backup_accuracy_penalty;

	g_pMoveHelper->set_host(local);
	g_pMovement->StartTrackPredictionErrors(local);
	move_data.forward_move = cmd->forwardmove;
	move_data.side_move = cmd->sidemove;
	move_data.up_move = cmd->upmove;
	move_data.buttons = cmd->buttons;
	move_data.view_angles = cmd->viewangles;
	move_data.angles = cmd->viewangles;
	move_data.impulse_command = cmd->impulse;

	g_pPrediction->SetupMove(local, cmd, g_pMoveHelper, &data);
	g_pPrediction->FinishMove(local, animation_info[cmd->command_number % 150].cmd, &move_data);
	local->get_flags() = animation_info[cmd->command_number % 150].flags;
	local->h_ground_entity() = animation_info[cmd->command_number % 150].ground_entity;


	g_pMovement->ProcessMovement(local, &move_data);
	local->get_tick_base() = backup_tickbase;
	local->get_velocity() = (local->get_origin() - backup_origin) * (1.f / g_global_vars->interval_per_tick);

	g_pMoveHelper->unknown_func();
	g_pMovement->FinishTrackPredictionErrors(local);
	g_pMoveHelper->set_host(nullptr);
	weapon->UpdateAccuracyPenalty();
	
	// finish movement.
	prediction()->finish_move(local, cmd, &move_data);
	// run movement.
	g_pMovement->ProcessMovement(local, &data);
	g_pPrediction->FinishMove(local, cmd, &data);
	g_pMovement->FinishTrackPredictionErrors(local);

	// reset target player ( host ).
	g_pMoveHelper->set_host(nullptr);
}*/
void c_prediction_system::initial(c_base_player* local, c_user_cmd* cmd, int32_t seq)
{
	if (!local)return;
	c_base_combat_weapon* weapon = local->get_active_weapon();
	if (!weapon)return;
	weapon->UpdateAccuracyPenalty();
	static c_move_data data{ };
	csgo_data.in_prediction = true;
	// CPrediction::StartCommand	
	c_user_cmd* buffer = csgo_data.local->get_current_command();
	buffer = cmd;
	// backup globals.
	backup_curtime = g_global_vars->curtime;
	backup_frametime = g_global_vars->frametime;

	// CPrediction::RunCommand

	// set globals appropriately.
	g_global_vars->curtime = ticks_to_time(local->get_tick_base() * g_global_vars->interval_per_tick);
	g_global_vars->frametime = g_global_vars->interval_per_tick;

	// set target player ( host ).
	*prediction_random_seed = _md5_pseudo_random(cmd->command_number) & 0x7FFFFFFF;
	g_pPrediction->SetupMove(local, cmd, g_pMoveHelper, &data);
	g_pMoveHelper->set_host(local);
	g_pMovement->StartTrackPredictionErrors(local);
	move_data.forward_move = cmd->forwardmove;
	move_data.side_move = cmd->sidemove;
	move_data.up_move = cmd->upmove;
	move_data.buttons = cmd->buttons;
	move_data.view_angles = cmd->viewangles;
	move_data.angles = cmd->viewangles;
	move_data.impulse_command = cmd->impulse;
	// setup input.
	// run movement.
	g_pMovement->ProcessMovement(local, &data);
	g_pPrediction->FinishMove(local, cmd, &data);
	g_pMovement->FinishTrackPredictionErrors(local);
	// reset target player ( host ).
	g_pMoveHelper->set_host(nullptr);
}
void c_prediction_system::repredict(c_base_player* local, c_user_cmd* cmd)
{
	//static const auto sv_footsteps = g_Cvar->FindVar("sv_footsteps");
	/*const auto weapon = local->get_m_hActiveWeapon();
	if (!weapon)return;

	// backup footsteps.
	//const auto backup_footsteps = sv_footsteps->GetInt();
	//sv_footsteps->AddFlags(1 << 14) &= ~(1 << 14); // FCVAR_CHEAT
	//sv_footsteps->flags &= ~(1 << 8); // FCVAR_NOTIFY
	//sv_footsteps->SetValue(0);

	// select correct data.
	move_data = animation_info[cmd->command_number % 150].move_data;
	g_GlobalVars->curtime = animation_info[cmd->command_number % 150].curtime;

	// setup prediction parameters.
	*prediction_random_seed = _md5_pseudo_random(cmd->command_number) & 0x7FFFFFFF; // error here / FIX ME /

	// set player data.	
	local->set_aim_punch(backup_aim_punch);	
	local->set_eye_pos(backup_view_offset);
	//weapon->get_accuracy_penalty() = backup_accuracy_penalty;
	//weapon->get_recoil_index() = backup_recoil_index;
	//local->get_duck_amount() = backup_duck_amount;

	// start of prediction.
	g_pMoveHelper->set_host(local);
	g_pMovement->StartTrackPredictionErrors(local);

	// correct move data.
	move_data.forward_move = cmd->forwardmove;
	move_data.side_move = cmd->sidemove;
	move_data.up_move = cmd->upmove;
	move_data.buttons = cmd->buttons;
	move_data.view_angles = cmd->viewangles;
	move_data.angles = cmd->viewangles;
	move_data.impulse_command = cmd->impulse;

	// restore player state.
	
	g_pPrediction->FinishMove(local, animation_info[cmd->command_number % 150].cmd, &move_data);
	local->set_flags(animation_info[cmd->command_number % 150].flags);
	//local->get_ground_entity() = animation_info[cmd->command_number % 150].ground_entity;

	// run movement.
	g_pMovement->ProcessMovement(local, &move_data);

	// finish movement.
	g_pPrediction->FinishMove(local, cmd, &move_data);

	// correct player data.
	local->set_tick_base(  backup_tickbase);
	local->get_velocity() = (local->get_origin() - backup_origin) * (1.f / g_GlobalVars->interval_per_tick);

	// end of prediction.
	g_pMoveHelper->unknown_func();
	g_pMovement->FinishTrackPredictionErrors(local);
	g_pMoveHelper->set_host(nullptr);

	// update weapon accuracy.
//	weapon->update_accuracy();

	// restore footsteps.
	//sv_footsteps->SetValue(backup_footsteps);*/
}

void c_prediction_system::restore() const
{
	if (!csgo_data.cmd || !csgo_data.local)
		return;

	*prediction_random_seed = -1;
	*prediction_player = nullptr;
	g_global_vars->curtime = backup_curtime;
	g_global_vars->frametime = backup_frametime;
}
