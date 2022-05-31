#include <d3d9.h>
#pragma comment ( lib,"d3d9.lib")
#include "hook_manager.hpp"
#include "../sdk/sdk.hpp"
#include "../chams.h"
#include <backend\cfg_system.hpp>
#include "../sdk/csgo_struct.h"
#include "../menu.h"
#include "../../imgui/impl/imgui_impl_dx9.h"
#include "../../imgui/impl/imgui_impl_win32.h"
#include "../../imgui/imgui.h"
#include "../visuals.hpp"
#include "../sdk/csgo.h"
#include "../g_event.h"
#include "../legit.h"
#include "../misc.h"
#include <interfaces\ConVar.h>
#include "../engine_prediction.h"
#include "../interfaces/IVModelRender.hpp"
#include <intrin.h>
#include <MinHook\MinHook.h>
#include "../fake_ping.h"
#define alpha_contoller(src, dst, origin)src = dst; if(src > origin.a)src = origin.a;
long __stdcall	hk_end_scene(IDirect3DDevice9* p_device);
long __stdcall	hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
int __stdcall	hk_isboxvisible(const Vector& mins, const Vector& maxs);
bool __fastcall hkSvCheatsGetBool(PVOID  _this, void* edx);
bool __fastcall Hooked_SendNetMsg(NetworkChannel* pNetChan, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice);
void __stdcall	hkFrameStageNotify(ClientFrameStage_t stage);
void __stdcall	hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket);
int __fastcall	Hooked_SendDatagram(void* netchan, void*, void* datagram);
int __fastcall	hk_do_post_effect(void* _this, int edx, int a1);
void __fastcall hkDrawModelExecute(void* _this,int edx,  IMatRenderContext* ctx, const DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
void __fastcall hkPaintTraverse(void* edx, int ecx, unsigned int panel, bool f, bool d);
void __fastcall hkLockCursor(void* edx, void* ecx);
void __fastcall hkCreateMove_Proxy(void* _this, int, int sequence_number, float input_sample_frametime, bool active);
void __fastcall Hooked_ProcessPacket(NetworkChannel* ecx, void* edx, void* packet, bool header);
typedef long(__stdcall* o_reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef long(__stdcall* o_end_scene)(IDirect3DDevice9*);
typedef int(__thiscall* send_datagram_fn)(void* ecx, void* datagram);
typedef int(__thiscall* o_box_visible)(const Vector&, const Vector&);
typedef bool(__thiscall* o_get_bool)(ConVar* ecx);
typedef void(__thiscall* fnDrawModelExecure)(void*, IMatRenderContext*, const DrawModelState_t&, ModelRenderInfo_t&, matrix3x4_t*);
typedef void(__thiscall* fnNetProcessPacket)(NetworkChannel* ecx, void* packet, bool header);
typedef bool(__thiscall* fnSetnetMsg)(NetworkChannel* pNetChan, INetMessage& msg, bool bForceReliable, bool bVoice);
typedef void(__thiscall* oFrameStageNotify)(void* ecx, ClientFrameStage_t stage);
typedef void(__thiscall* oPaintTraverse)(void* ecx, unsigned int panel, bool f, bool d);
typedef void(__thiscall* oLockFn)(void* ecx);
typedef void(__fastcall* oCreateMove)(void* ecx, int edx, int, float, bool);

int __stdcall hk_isboxvisible(const Vector & mins, const Vector & maxs)
{
	static o_box_visible o_box = hook_manager_.engine_hook->get_original<o_box_visible>(32);
	static DWORD beam_cull_check = (utils::get().pattern_scan("client.dll", "85 C0 0F 84 ? ? ? ? 8B 0D ? ? ? ? 8D 54 24 6C"));
	if ((DWORD)_ReturnAddress() == beam_cull_check) // ring beams
	{
		return 1;
	}
	return o_box(mins, maxs);
}
void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
	//static oCreateMove oMoves = hook_manager_.client_dll_hook->get_original<oCreateMove>(22);
	static oCreateMove oMoves = hook_manager_.client_dll_hook->get_manual_orig<oCreateMove, 22>();
	oMoves(g_base_client_dll, 0, sequence_number, input_sample_frametime, active);
	if (!g_input)return;
	auto cmd = g_input->get_user_cmd(sequence_number);
	auto verified = g_input->get_verified_cmd(sequence_number);
	if (!cmd || !cmd->command_number)
		return;

	if (!csgo_data.local || !g_engine->IsInGame() || csgo_data.local->hp() <= 0) {
		g_legitbot.clear_data();
		return;
	}
	NetworkChannel* pNetChannel = reinterpret_cast<NetworkChannel*>(g_pClientState->m_NetChannel);
	if (pNetChannel != nullptr && !hook_manager_.net_channel_hook->ovftable && pNetChannel != NULL  && g_pClientState && g_pClientState->m_NetChannel != NULL)
	{
		hook_manager_.net_channel_hook->init(pNetChannel);
		hook_manager_.net_channel_hook->hook(Hooked_ProcessPacket, 39);
		hook_manager_.net_channel_hook->hook(Hooked_SendNetMsg, 40);
		hook_manager_.net_channel_hook->hook(Hooked_SendDatagram, 46);
	}	
	static ConVar* nadeVar{ g_cvar->FindVar("cl_grenadepreview") };
	nadeVar->m_fnChangeCallbacks.m_Size = 0;
	nadeVar->SetValue(true);
	csgo_data.cmd = cmd;
	if (cfg_vars.misc.moon_walk)g_misc.gravity_walk(cmd);
	if (cfg_vars.misc.bunnyhope)g_misc.bunny_hop(cmd);
	if (cfg_vars.misc.slow_walk)g_misc.slow_walk(cmd);
	if (cfg_vars.misc.radar_hack)g_misc.radar_hack();

	g_legitbot.get_delta_time();
	csgo_data.in_prediction = false;
	if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->engine_predict)c_prediction_system::get().initial(csgo_data.local, cmd, sequence_number);
	if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->enabled)g_legitbot.aimbot(cmd, bSendPacket);
	if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->trigger)g_legitbot.triggerbot(cmd);
	if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->rcs_standalone)g_legitbot.rcs_standalone(cmd);
	if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->backtrack)g_legitbot.back_track(cmd);
	if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->engine_predict)c_prediction_system::get().restore();
	if (cmd->buttons & IN_SCORE)
	{
		g_base_client_dll->DispatchUserMessage(50, 0, 0, nullptr);//CS_UM_ServerRankRevealAll = 50
	}

	if (!g_engine->IsVoiceRecording() && !(cmd->buttons & IN_ATTACK))g_misc.fake_lags(bSendPacket);


	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();
}
long __stdcall hk_end_scene(IDirect3DDevice9* _p_device)
{
	DWORD colorwrite, srgbwrite;
	IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
	_p_device->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
	_p_device->GetVertexDeclaration(&vertDec);
	_p_device->GetVertexShader(&vertShader);
	g_menu.init(_p_device);
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	g_menu.render();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	pixel_state->Apply();
	pixel_state->Release();
	_p_device->SetVertexDeclaration(vertDec);
	_p_device->SetVertexShader(vertShader);
	static ConVar* crosshair = g_cvar->FindVar("weapon_debug_spread_show");
	static ConVar* mat_ambient_r = g_cvar->FindVar("mat_ambient_light_r");
	static ConVar* mat_ambient_g = g_cvar->FindVar("mat_ambient_light_g");
	static ConVar* mat_ambient_b = g_cvar->FindVar("mat_ambient_light_b");
	static ConVar* viewmodel = g_cvar->FindVar("viewmodel_fov");
	static ConVar* viewmodel_offset_x = g_cvar->FindVar("viewmodel_offset_x");
	static ConVar* viewmodel_offset_y = g_cvar->FindVar("viewmodel_offset_y");
	static ConVar* viewmodel_offset_z = g_cvar->FindVar("viewmodel_offset_z");
	static ConVar* nightmode_za_5_kopeek = g_cvar->FindVar("mat_force_tonemap_scale");
	static ConVar* bright = g_cvar->FindVar("mat_fullbright");//not used
	static ConVar* sky = g_cvar->FindVar("r_3dsky");	
	static ConVar* r_shadow = g_cvar->FindVar("r_shadows");
	r_shadow->SetValue(!cfg_vars.visuals.remove_shadows);
	static ConVar* cl_csm_shadows = g_cvar->FindVar("cl_csm_shadows");
	cl_csm_shadows->SetValue(!cfg_vars.visuals.remove_shadows);
	static ConVar* cl_foot_contact_shadows = g_cvar->FindVar("cl_foot_contact_shadows"); 
	cl_foot_contact_shadows->SetValue(!cfg_vars.visuals.remove_shadows);
	static ConVar* cl_csm_viewmodel_shadows = g_cvar->FindVar("cl_csm_viewmodel_shadows");
	cl_csm_viewmodel_shadows->SetValue(!cfg_vars.visuals.remove_shadows);
	static ConVar* cl_csm_rope_shadows = g_cvar->FindVar("cl_csm_rope_shadows");
	cl_csm_rope_shadows->SetValue(!cfg_vars.visuals.remove_shadows);
	nightmode_za_5_kopeek->SetValue(cfg_vars.show_menu? 0.2f : 1.0f);
	ConVar* crosshair2 = g_cvar->FindVar("weapon_debug_spread_gap");
	crosshair2->SetValue(999999999);
	viewmodel->m_fnChangeCallbacks.m_Size = 0;
	viewmodel_offset_x->m_fnChangeCallbacks.m_Size = 0;
	viewmodel_offset_y->m_fnChangeCallbacks.m_Size = 0;
	viewmodel_offset_z->m_fnChangeCallbacks.m_Size = 0;
	sky->SetValue(!cfg_vars.visuals.remove_3d_sky);	
	static bool once = true;
	if (once)
	{
		cfg_vars.misc.viewmodel_fov = viewmodel->GetInt();
		cfg_vars.misc.viewmodel_offset_x = viewmodel_offset_x->GetInt();
		cfg_vars.misc.viewmodel_offset_y = viewmodel_offset_y->GetInt();
		cfg_vars.misc.viewmodel_offset_z = viewmodel_offset_z->GetInt();
		cfg_vars.visuals.post_effect_col.r = mat_ambient_r->GetFloat();
		cfg_vars.visuals.post_effect_col.g = mat_ambient_g->GetFloat();
		cfg_vars.visuals.post_effect_col.b = mat_ambient_b->GetFloat();
		once = false;
	}
	crosshair->SetValue(cfg_vars.visuals.force_crosshair);
	viewmodel->SetValue(cfg_vars.misc.viewmodel_fov);
	viewmodel_offset_x->SetValue(cfg_vars.misc.viewmodel_offset_x);
	viewmodel_offset_y->SetValue(cfg_vars.misc.viewmodel_offset_y);
	viewmodel_offset_z->SetValue(cfg_vars.misc.viewmodel_offset_z);
	mat_ambient_r->SetValue(cfg_vars.visuals.post_effect_col.r / 255.f);
	mat_ambient_g->SetValue(cfg_vars.visuals.post_effect_col.g / 255.f);
	mat_ambient_b->SetValue(cfg_vars.visuals.post_effect_col.b / 255.f);
	bright->SetValue(cfg_vars.visuals.bright);
	static o_end_scene o_t = hook_manager_.d3d9_hook->getOriginal__std<long, 42>(_p_device);
	return o_t(_p_device);
}
long __stdcall hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	o_reset o_reset_t = hook_manager_.d3d9_hook->getOriginal__std<long, 16>(device, pPresentationParameters);
	ImGui_ImplDX9_InvalidateDeviceObjects();	
	auto hr = o_reset_t(device, pPresentationParameters);
	if (hr >= 0)
		ImGui_ImplDX9_CreateDeviceObjects();
	return hr;
}
void __stdcall hkFrameStageNotify(ClientFrameStage_t stage)
{	
	static oFrameStageNotify ofunc = hook_manager_.client_dll_hook->get_manual_orig<oFrameStageNotify,37>();
	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
	{
		if (g_pClientState != nullptr) {
			if (hook_manager_.net_channel_hook && hook_manager_.net_channel_hook->ovftable && hook_manager_.net_channel_hook->new_table) {
				if (g_pClientState->m_NetChannel != NULL && g_engine->IsInGame() && csgo_data.local && csgo_data.local->hp() > 0) {
					uintptr_t* vtable = *(uintptr_t**)((NetworkChannel*)g_pClientState->m_NetChannel);
					if (vtable && vtable != hook_manager_.net_channel_hook->new_table) {
						hook_manager_.net_channel_hook->unhook();
					}
				}
			}
		}
	}
	if (stage == FRAME_RENDER_START && csgo_data.local)
	{
		g_legitbot.back_track_update();
		for (int i = 1; i <= g_global_vars->max_clients; i++)
		{
			if (i == g_engine->GetLocalPlayer()) continue;

			IClientEntity* pCurEntity = g_entity_list->GetClientEntity(i);
			if (!pCurEntity) continue;

			*(int*)((uintptr_t)pCurEntity + 0xA30) = g_global_vars->framecount; //we'll skip occlusion checks now
			*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;//clear occlusion flags
		}

		csgo_data.fl_rainbow_chams += g_global_vars->frametime / 4.f * float(cfg_vars.visuals.chams_speed_rainbow / 10.f);
		if (csgo_data.fl_rainbow_chams > 1.0f)csgo_data.fl_rainbow_chams = 0.0f;
		Color::ColorConvertHSVtoRGB(csgo_data.fl_rainbow_chams, 1.f, 1.f, csgo_data.c_rainbow__chams.r, csgo_data.c_rainbow__chams.g, csgo_data.c_rainbow__chams.b);
	}
	if (stage == FRAME_START && g_engine->IsInGame()) {
		g_visuals.remove_smoke();
		csgo_data.local = static_cast<c_base_player*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));	
		g_visuals.third_person();
		int w, h;
		g_engine->GetScreenSize(w, h);
		csgo_data.screen_center.x = w / 2.f;
		csgo_data.screen_center.y = h / 2.f;
		csgo_data.screen_center.z = 0;
		for (int aye = 1; aye < 64; ++aye)//aye = kruto; aye - stilno, modno, molodezhno
		{
			c_base_player* aye_check = static_cast<c_base_player*>(g_entity_list->GetClientEntity(aye));
			if (aye_check)
			{
				bool should_change = false;
				csgo_data.old_alpha_data[aye] = aye_check->dormant() ? 0.f : 255.f;
				if (csgo_data.old_alpha_data[aye] != csgo_data.alpha_data[aye])should_change = true;
				else {
					csgo_data.old_curtime_for_alpha[aye] = g_global_vars->curtime;
				}
				if (should_change)
				{
					if (csgo_data.old_alpha_data[aye] <= 0.0f)
					{
						float percent = (g_global_vars->curtime - csgo_data.old_curtime_for_alpha[aye]) / .5f;
						float to_change = fabs(percent);
						csgo_data.alpha_data[aye] = 255.f - (255.f * to_change);
						alpha_contoller(csgo_data.box_color_alpha[aye], csgo_data.alpha_data[aye], cfg_vars.visuals.box_color);
						alpha_contoller(csgo_data.box_background_color_alpha[aye], csgo_data.alpha_data[aye], cfg_vars.visuals.box_background_color);
						alpha_contoller(csgo_data.skeleton_color_alpha[aye], csgo_data.alpha_data[aye], cfg_vars.visuals.skeleton_color);
						alpha_contoller(csgo_data.name_color_alpha[aye], csgo_data.alpha_data[aye], cfg_vars.visuals.name_esp_color);
						/*csgo_data.box_color_alpha[aye] = csgo_data.alpha_data[aye];
						if (csgo_data.box_color_alpha[aye] > cfg_vars.visuals.box_color.a)csgo_data.box_color_alpha[aye] = cfg_vars.visuals.box_color.a;*/
					}
					else {
						float get_percent = csgo_data.alpha_data[aye] / 255.f;
						float percent = (g_global_vars->curtime - csgo_data.old_curtime_for_alpha[aye]) / .5f;
						float to_change = fabs(percent - get_percent);
						csgo_data.alpha_data[aye] =  (255.f * to_change);
						alpha_contoller(csgo_data.box_color_alpha[aye], csgo_data.alpha_data[aye], cfg_vars.visuals.box_color);
						alpha_contoller(csgo_data.box_background_color_alpha[aye], csgo_data.alpha_data[aye], cfg_vars.visuals.box_background_color);
						alpha_contoller(csgo_data.skeleton_color_alpha[aye], csgo_data.alpha_data[aye], cfg_vars.visuals.skeleton_color);
						alpha_contoller(csgo_data.name_color_alpha[aye], csgo_data.alpha_data[aye], cfg_vars.visuals.name_esp_color);
						/*csgo_data.box_color_alpha[aye] = csgo_data.alpha_data[aye];
						if (csgo_data.box_color_alpha[aye] > cfg_vars.visuals.box_color.a)csgo_data.box_color_alpha[aye] = cfg_vars.visuals.box_color.a;*/
					}					
				}
				else {
					csgo_data.box_color_alpha[aye] = cfg_vars.visuals.box_color.a;
					csgo_data.box_background_color_alpha[aye] = cfg_vars.visuals.box_background_color.a;
					csgo_data.skeleton_color_alpha[aye] = cfg_vars.visuals.skeleton_color.a;
					csgo_data.name_color_alpha[aye] = cfg_vars.visuals.name_esp_color.a;

				}
				if (csgo_data.alpha_data[aye] < 0.f)csgo_data.alpha_data[aye] = 0.0f;
				if (csgo_data.alpha_data[aye] > 255.f)csgo_data.alpha_data[aye] = 255.f;				
			}
		}
		if (csgo_data.local) {
			c_base_combat_weapon* weapon = csgo_data.local->get_active_weapon();
			if (weapon) {
				CCSWeaponInfo* data = weapon->GetCSWeaponData();
				if (data) {
					csgo_data.active_wpn_type = data->iWeaponType;
					if (csgo_data.active_wpn_type >= 7 )csgo_data.active_wpn_type = 0;
					if(csgo_data.auto_select_weapon)cfg_vars.legitbot.current_pick = csgo_data.active_wpn_type - 1;
				}
			}
		}
		if (GetAsyncKeyState(cfg_vars.misc.thirdperson_key) & 1)cfg_vars.misc.thirdperson = !cfg_vars.misc.thirdperson;
		static DWORD now = GetTickCount();
		if (GetTickCount() - now > 125)
		{
			now = GetTickCount();
			if (g_input_system->IsButtonDown(g_input_system->VirtualKeyToButtonCode(cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_shot_key)))
			{
				cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_shoot = !cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_shoot;
			}
		}	
	}		
	if (!g_engine->IsInGame())
	{
		if (hook_manager_.net_channel_hook->ovftable)
		{
			hook_manager_.net_channel_hook->unhook();
		}
		csgo_data.local = nullptr;
		csgo_data.lask_kill_time = 0.0f;		
		for (int i = 0; i < 64; i++)
		{
			csgo_data.old_curtime_for_alpha[i] = 0.0f;
			g_event_handler.hit_data[i].clear();				
		}
	}	
	//g_visuals.model_changer(stage);
	return ofunc(g_base_client_dll,stage);
}
void __fastcall hkPaintTraverse(void* edx, int ecx, unsigned int panel, bool f, bool d)
{
	static oPaintTraverse oTraverse = hook_manager_.panel_hook->get_original<oPaintTraverse>(41);
	static int old_width = 0;
	static int old_height = 0;
	static unsigned int panelId = 0;
	if (!strcmp("HudZoom", g_vgui_panel->GetName(panel)) && cfg_vars.visuals.remove_scope)//remove scope
		return;
	oTraverse(g_vgui_panel, panel, f, d);
	if (panelId == 0) {
		const auto panelName = g_vgui_panel->GetName(panel);
		if (!strcmp(panelName, "FocusOverlayPanel")) {
			panelId = panel;
		}
	}
	else if (panelId == panel)
	{
		static bool once = false; if (!once) {			
			g_visuals.verdana = g_surface->CreateFont_();
			g_visuals.verdana_big = g_surface->CreateFont_();
			g_visuals.icons = g_surface->CreateFont_();
			g_surface->SetFontGlyphSet(g_visuals.verdana, "Verdana", 12, 300, 0, 0, FONTFLAG_OUTLINE);
			g_surface->SetFontGlyphSet(g_visuals.verdana_big, "Verdana", 26, 600, 0, 0, FONTFLAG_ANTIALIAS);
			g_surface->SetFontGlyphSet(g_visuals.icons, "Counter-Strike", 20, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);			
			once = true;
		}
		int width, height = 0;
		g_engine->GetScreenSize(width, height);
		if (width != old_width || height != old_height)
		{
			once = false;
			old_width = width;
			old_height = height;
		}
		if (!g_engine->IsInGame() || !csgo_data.local || !g_engine->IsConnected())return;
		g_visuals.on_paint_traverse();
		g_event_handler.on_paint();	
	}	
}
void __fastcall hkLockCursor(void* edx, void* ecx)
{
	static oLockFn orig = hook_manager_.surface_hook->get_original<oLockFn>(67);
	if (cfg_vars.show_menu)//anti crash
	{
		g_surface->UnlockCursor();
		g_input_system->EnableInput(false);
		return;
	}
	g_input_system->EnableInput(true);
	orig(g_surface);
}
int __fastcall hk_do_post_effect(void* _this, int edx, int a1)
{
	static auto oDoPostScreenEffects = hook_manager_.client_mode_hook->get_original<decltype(&hk_do_post_effect)>(44);
	if (!g_engine->IsInGame() || !csgo_data.local)
	{
		return oDoPostScreenEffects(g_client_mode, edx, a1);
	}
	if (cfg_vars.visuals.glow) {
		if (g_engine->IsInGame() && csgo_data.local) {			
			g_visuals.glow(0);
		}
	}
	for (int i = 1; i < 64; i++)
	{
		c_base_player* player = static_cast<c_base_player*>(g_entity_list->GetClientEntity(i));
		if (!player || player->hp() <= 0 || player->dormant() || !player->is_enemy())continue;
		if (cfg_vars.visuals.dlight && cfg_vars.visuals.enabled)g_visuals.do_dlight(player, i);
	}
	
	return oDoPostScreenEffects(g_client_mode, edx, a1);
}
__declspec(naked) void __fastcall hkCreateMove_Proxy(void* _this, int, int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		push ebp
		mov  ebp, esp
		push ebx; not sure if we need this
		push esp
		push dword ptr[active]
		push dword ptr[input_sample_frametime]
		push dword ptr[sequence_number]
		call hkCreateMove
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}
bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx)
{
	// cameraThink = findPattern(L"client", "\x85\xC0\x75\x30\x38\x86");
	static auto dwCAM_Think = utils::get().pattern_scan("client.dll", "85 C0 75 30 38 86");
	if (uintptr_t(_ReturnAddress()) == dwCAM_Think && cfg_vars.misc.thirdperson)
		return true;
	else
		return hook_manager_.sv_cheats_hook->getOriginal<bool, 13>()(pConVar);
}
void __fastcall Hooked_ProcessPacket(NetworkChannel* ecx, void* edx, void* packet, bool header) {
	if (!g_pClientState->m_NetChannel)
		return hook_manager_.net_channel_hook->get_original<fnNetProcessPacket>(39)(ecx, packet, header);

	hook_manager_.net_channel_hook->get_original<fnNetProcessPacket>(39)(ecx, packet, header);
	if (g_engine->IsInGame() && csgo_data.local && csgo_data.local->is_alive())
		ping_manager.flip_state(ecx);//idk


	//for (CEventInfo* it{ g_pClientState->pEve }; it != nullptr; it = it->m_next) {
	//	if (!it->m_class_id)
		//	continue;

		// set all delays to instant.
	//	it->m_fire_delay = 0.f;
	//}

	g_engine->FireEvents();
}
bool __fastcall Hooked_SendNetMsg(NetworkChannel* pNetChan, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice)
{
	static auto o_funk = hook_manager_.net_channel_hook->get_original<fnSetnetMsg>(40);
	if (g_engine->IsInGame() && csgo_data.local)
	{
		if (msg.GetType() == 14) // Return and don't send messsage if its FileCRCCheck
			return false;

		if (msg.GetGroup() == 9) // Fix lag when transmitting voice and fakelagging
			bVoice = true;
	}
	return o_funk(pNetChan, msg, bForceReliable, bVoice);
}
int __fastcall Hooked_SendDatagram(void* netchan, void*, void* datagram)
{
	static auto orig =hook_manager_.net_channel_hook->get_original<send_datagram_fn>(46);

	if (!g_engine->IsInGame())
		return orig(netchan, datagram);

	const auto net_channel = reinterpret_cast<NetworkChannel*> (netchan);
	const auto backup_seqnr = net_channel->in_sequence_nr;

	if (csgo_data.local->is_alive() && cfg_vars.misc.extend_backtrack)
	{
		auto ping_value = 0.2f;
		NetworkChannel* nci = g_engine->GetNetChannelInfo();
		if (nci)
		{
			ping_value -= nci->get_latency(0);//0 = outgoing
		}
		static auto oldseqnum = 0;
		if (oldseqnum != net_channel->in_sequence_nr)
		{
			oldseqnum = net_channel->in_sequence_nr;
			net_channel->in_sequence_nr += 126 - static_cast<uint32_t>(63 * (ping_value));
		}		
	}
	const auto ret = orig(net_channel, datagram);
	net_channel->in_sequence_nr = backup_seqnr;
	return ret;
}
void __fastcall hkDrawModelExecute(void* _this, int edx, IMatRenderContext* ctx, const DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	static auto oDraw = hook_manager_.model_render_hook->get_manual_orig<fnDrawModelExecure, 21>();
	//static fnDrawModelExecure oDraw = hook_manager_.model_render_hook->getOriginal<void, 21>(ctx, state, pInfo, pCustomBoneToWorld);
	if (!csgo_data.local)
	{
		return  oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
	}
	if (g_model_render->IsForcedMaterialOverride() &&
		!strstr(pInfo.pModel->szName, "arms") &&
		!strstr(pInfo.pModel->szName, "weapons/v_")) {
		return oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
	}
	if (csgo_data.in_load_cfg) {
		return oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
	}
	const auto mdl = pInfo.pModel;
	bool is_arm = strstr(mdl->szName, "arms") != nullptr;
	bool is_player = strstr(mdl->szName, "models/player") != nullptr;
	bool is_weapon = strstr(mdl->szName, "models/weapon") != nullptr;
	bool is_sleeve = strstr(mdl->szName, "sleeve") != nullptr;
	bool shadow = strstr(mdl->szName, "models/player/contactshadow") != nullptr;

	static std::vector<const char*>smoke_mats =
	{
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
	};
	for (auto mat_s : smoke_mats)
	{
		IMaterial* mat = g_mat_system->FindMaterial(mat_s, TEXTURE_GROUP_OTHER);
		if (cfg_vars.visuals.smoke_wireframe || cfg_vars.visuals.remove_smoke) {
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
			if (strcmp(mat_s, "particle/vistasmokev1/vistasmokev1_smokegrenade"))mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);//if remove smoke
		}
		else {
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			if (strcmp(mat_s, "particle/vistasmokev1/vistasmokev1_smokegrenade"))mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);//if remove smoke
		}
	}
	if (cfg_vars.misc.fakelags && is_player)
	{
		IClientEntity* entity = (g_entity_list->GetClientEntity(pInfo.entity_index));
		if (entity == csgo_data.local)
		{
			//Vector oldOrigin = pInfo.origin;
			if (g_misc.last_net_matrix)
			{
				//pInfo.origin = g_misc.last_net_origin;
				chams_.overrideMat(chams_.default_mat, false, false);
				oDraw(g_model_render, ctx, state, pInfo, g_misc.last_net_matrix);
				g_model_render->ForcedMaterialOverride(nullptr);
			}
			//pInfo.origin = oldOrigin;
		}
	}

	if (is_player && cfg_vars.visuals.backtrack_chams) {
		Vector oldOrigin = pInfo.origin;
		c_base_player* entity = static_cast<c_base_player*>(g_entity_list->GetClientEntity(pInfo.entity_index));
		if (entity && entity->hp() > 0 && entity != csgo_data.local) {
			if (csgo_data.local && csgo_data.local->hp() > 0)
			{
				auto record = &g_legitbot.records[pInfo.entity_index];
				if (record && record->size() && utils::get().valid_backtrack(record->front().simulation_time)) {
					g_model_render->ForcedMaterialOverride(chams_.metallic_mat);
					oDraw(_this, ctx, state, pInfo, record->back().matrix);
					g_model_render->ForcedMaterialOverride(nullptr);
				}
			}
		}
		pInfo.origin = oldOrigin;
	}
	if (is_sleeve && (cfg_vars.visuals.sleeves_chams || cfg_vars.visuals.no_hands))
	{
		chams_.sleeves_cur_mat = chams_.get_material_by_cfg_material(cfg_vars.visuals.sleeves_chams_type);
		if (cfg_vars.visuals.no_hands)
		{
			chams_.invis_material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			g_model_render->ForcedMaterialOverride(chams_.invis_material);
		}
		else chams_.do_sleeves_chams();
	}
	if (is_arm && !is_sleeve && (cfg_vars.visuals.hands_chams || cfg_vars.visuals.no_hands))
	{
		chams_.hand_cur_mat = chams_.get_material_by_cfg_material(cfg_vars.visuals.hands_chams_type);
		if (cfg_vars.visuals.no_hands)
		{
			chams_.invis_material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			g_model_render->ForcedMaterialOverride(chams_.invis_material, 0, 0);
		}
		else chams_.do_arm_chams(ctx, state, pInfo, pCustomBoneToWorld);

	}
	if (is_weapon && cfg_vars.visuals.weapon_chams && !is_sleeve && !is_player && !is_arm && !csgo_data.local->is_scoped())
	{
		chams_.weapon_cur_mat = chams_.get_material_by_cfg_material(cfg_vars.visuals.weapon_chams_type);
		chams_.do_weapon_chams();
	}
	if (is_player && !shadow && g_engine->IsInGame() && cfg_vars.visuals.chams)
	{
		c_base_player* pl = static_cast<c_base_player*>(g_entity_list->GetClientEntity(pInfo.entity_index));
		c_base_player* local = static_cast<c_base_player*>(g_entity_list->GetClientEntity(g_engine->GetLocalPlayer()));
		if (pl && local && pl->get_team() != local->get_team() && pl->hp() > 0) {
			chams_.player_cur_mat = chams_.get_material_by_cfg_material(cfg_vars.visuals.player_chams_type);
			chams_.player_cur_mat->FindVar("$pearlescent", nullptr, false)->set_value(cfg_vars.visuals.pearlsecent);
			chams_.player_cur_mat->FindVar("$phong", nullptr, false)->set_value(cfg_vars.visuals.phong);
			chams_.player_cur_mat->FindVar("$phongboost", nullptr, false)->set_value(cfg_vars.visuals.phongboost);
			chams_.player_cur_mat->FindVar("$rimlight", nullptr, false)->set_value(cfg_vars.visuals.rimlight);
			chams_.player_cur_mat->FindVar("$rimlightboost", nullptr, false)->set_value(cfg_vars.visuals.rimlight_boost);
			chams_.player_cur_mat->FindVar("$envmaplightscale", nullptr, false)->set_value(cfg_vars.visuals.envmaplightscale);
			chams_.player_cur_mat->FindVar("$rimlightexponent", nullptr, false)->set_value(cfg_vars.visuals.rimlightexponent);
			chams_.player_cur_mat->FindVar("$phongexponent", nullptr, false)->set_value(cfg_vars.visuals.phong_exponent);
			chams_.player_cur_mat->FindVar("$reflectivity", nullptr, false)->set_vector_value(Vector(cfg_vars.visuals.reflectivity.r, cfg_vars.visuals.reflectivity.g, cfg_vars.visuals.reflectivity.b) / 255.f);

			int type = cfg_vars.visuals.player_chams_type;
			if (chams_.player_cur_mat) {
				if (!cfg_vars.visuals.animated_chams) {
					if (type != 5 && type != 3) {
						if (chams_.should_material_envmap_override(type))
						{
							if (type >= 9)
							{
								if (cfg_vars.visuals.chams_glow_contrast)
								{
									float flColor[3] = { cfg_vars.visuals.chams_contrast.r / 255.f, cfg_vars.visuals.chams_contrast.g / 255.f,  cfg_vars.visuals.chams_contrast.b / 255.f };
									g_render_view->SetBlend(cfg_vars.visuals.chams_contrast.a / 255.f);
									g_render_view->SetColorModulation(flColor);
									chams_.overrideMat(chams_.default_mat, true, false, cfg_vars.visuals.chams_contrast / 255.f);
									oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
								}
								chams_.player_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(cfg_vars.visuals.model_color / 125.f);
								g_render_view->SetBlend(cfg_vars.visuals.model_color.a / 255.f);
								chams_.overrideMat(chams_.player_cur_mat, false, false);
							}
							else {
								if (cfg_vars.visuals.ignore_z)
								{
									chams_.player_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(cfg_vars.visuals.chams_invisible / 255.f);
									g_render_view->SetBlend(cfg_vars.visuals.chams_invisible.a / 255.f);
									chams_.overrideMat(chams_.player_cur_mat, true, cfg_vars.visuals.wireframe);
									oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
								}
								chams_.player_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(cfg_vars.visuals.model_color / 125.f);
								g_render_view->SetBlend(cfg_vars.visuals.model_color.a / 255.f);
								chams_.overrideMat(chams_.player_cur_mat, false, false);
							}
						}
						else {
							if (cfg_vars.visuals.ignore_z)
							{
								float flColor[3] = { cfg_vars.visuals.chams_invisible.r / 255.f, cfg_vars.visuals.chams_invisible.g / 255.f,  cfg_vars.visuals.chams_invisible.b / 255.f };
								g_render_view->SetBlend(cfg_vars.visuals.chams_invisible.a / 255.f);
								g_render_view->SetColorModulation(flColor);
								chams_.overrideMat(chams_.player_cur_mat, true, cfg_vars.visuals.wireframe, cfg_vars.visuals.chams_invisible / 255.f);
								oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
							}
							float flColor[3] = { cfg_vars.visuals.model_color.r / 255.f, cfg_vars.visuals.model_color.g / 255.f,  cfg_vars.visuals.model_color.b / 255.f };
							g_render_view->SetBlend(cfg_vars.visuals.model_color.a / 255.f);
							g_render_view->SetColorModulation(flColor);
							chams_.overrideMat(chams_.player_cur_mat, false, false, cfg_vars.visuals.model_color / 255.f);
						}
					}
					else {
						if (type == 5)
						{
							if (cfg_vars.visuals.ignore_z)
							{
								chams_.overrideMat(chams_.player_cur_mat, true, false);
								oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
							}
							chams_.overrideMat(chams_.player_cur_mat, false, false);
						}
						else {
							if (cfg_vars.visuals.ignore_z)
							{
								chams_.overrideMat(chams_.player_cur_mat, true, false, cfg_vars.visuals.chams_invisible / 255.f);
								oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
							}
							float flColor[3] = { cfg_vars.visuals.model_color.r / 15.f, cfg_vars.visuals.model_color.g / 15.f,  cfg_vars.visuals.model_color.b / 15.f };
							g_render_view->SetBlend(cfg_vars.visuals.model_color.a / 255.f);
							g_render_view->SetColorModulation(flColor);
							chams_.overrideMat(chams_.player_cur_mat, false, false, cfg_vars.visuals.model_color / 255.f);
						}
					}
				}
				else {
					if (type != 5 && type != 3) {
						if (chams_.should_material_envmap_override(type))
						{
							if (type == 9)
							{
								if (cfg_vars.visuals.chams_glow_contrast)
								{
									float flColor[3] = { csgo_data.c_rainbow__chams.r / 255.f, csgo_data.c_rainbow__chams.g / 255.f,  csgo_data.c_rainbow__chams.b / 255.f };
									g_render_view->SetBlend(cfg_vars.visuals.chams_invisible.a / 255.f);
									g_render_view->SetColorModulation(flColor);
									chams_.overrideMat(chams_.default_mat, true, false, csgo_data.c_rainbow__chams / 255.f);
									oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
								}
								chams_.player_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(type == 9 ? csgo_data.c_rainbow__chams * 5.5f : csgo_data.c_rainbow__chams);
								g_render_view->SetBlend(cfg_vars.visuals.model_color.a / 255.f);
								chams_.overrideMat(chams_.player_cur_mat, false, false);
							}
							else {
								if (cfg_vars.visuals.ignore_z)
								{
									chams_.player_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(csgo_data.c_rainbow__chams);
									g_render_view->SetBlend(cfg_vars.visuals.chams_invisible.a / 255.f);
									chams_.overrideMat(chams_.player_cur_mat, true, cfg_vars.visuals.wireframe);
									oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
								}
								chams_.player_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(csgo_data.c_rainbow__chams);
								g_render_view->SetBlend(cfg_vars.visuals.model_color.a / 255.f);
								chams_.overrideMat(chams_.player_cur_mat, false, false);
							}
						}
						else {
							if (cfg_vars.visuals.ignore_z)
							{
								float flColor[3] = { csgo_data.c_rainbow__chams.r / 255.f, csgo_data.c_rainbow__chams.g / 255.f,  csgo_data.c_rainbow__chams.b / 255.f };
								g_render_view->SetBlend(csgo_data.c_rainbow__chams.a / 255.f);
								g_render_view->SetColorModulation(flColor);
								chams_.overrideMat(chams_.player_cur_mat, true, cfg_vars.visuals.wireframe, csgo_data.c_rainbow__chams / 255.f);
								oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
							}
							float flColor[3] = { csgo_data.c_rainbow__chams.r / 255.f, csgo_data.c_rainbow__chams.g / 255.f,  csgo_data.c_rainbow__chams.b / 255.f };
							g_render_view->SetBlend(cfg_vars.visuals.model_color.a / 255.f);
							g_render_view->SetColorModulation(flColor);
							chams_.overrideMat(chams_.player_cur_mat, false, false, csgo_data.c_rainbow__chams / 255.f);
						}
					}
					else {
						if (type == 5)
						{
							if (cfg_vars.visuals.ignore_z)
							{
								chams_.overrideMat(chams_.player_cur_mat, true, false);
								oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
							}
							chams_.overrideMat(chams_.player_cur_mat, false, false);
						}
						else {
							if (cfg_vars.visuals.ignore_z)
							{
								chams_.overrideMat(chams_.player_cur_mat, true, false, csgo_data.c_rainbow__chams / 255.f);
								oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
							}
							float flColor[3] = { csgo_data.c_rainbow__chams.r / 15.f, csgo_data.c_rainbow__chams.g / 15.f,  csgo_data.c_rainbow__chams.b / 15.f };
							g_render_view->SetBlend(cfg_vars.visuals.model_color.a / 255.f);
							g_render_view->SetColorModulation(flColor);
							chams_.overrideMat(chams_.player_cur_mat, false, false, csgo_data.c_rainbow__chams / 255.f);
						}
					}
				}
			}
		}
	}
	oDraw(_this, ctx, state, pInfo, pCustomBoneToWorld);
	g_model_render->ForcedMaterialOverride(nullptr);
	static float old[3] = { 1.f,1.f,1.f };
	if (is_player || is_weapon || is_arm || is_sleeve) { g_render_view->SetColorModulation(old); g_render_view->SetBlend(1.f); }
}
void hook_manager::init_all()
{
	panel_hook->init(g_vgui_panel);
	client_mode_hook->init(g_client_mode);
	surface_hook->init(g_surface);
	model_cache_hook->init(g_model_cache);
	//engine_hook->init(g_engine);
	g_event_handler.Init();
	/*MIN HOOK*/
	client_dll_hook->init(g_base_client_dll);
	model_render_hook->init(g_model_render);
	d3d9_hook->init(p_device);
	sv_cheats_hook->init(g_cvar->FindVar("sv_cheats"));
	Sleep(1000);
}
void hook_manager::hook_all()
{
	init_all();
	chams_.init_materials();
	panel_hook->hook(hkPaintTraverse, hk_traverse);
	client_mode_hook->hook(hk_do_post_effect, 44);//-
	surface_hook->hook(hkLockCursor, 67);//-
	/*MIN HOOK*/
	MH_Initialize();
	model_render_hook->hookAt(21, hkDrawModelExecute);
	sv_cheats_hook->hookAt(13, hkSvCheatsGetBool);
	d3d9_hook->hookAt(42, hk_end_scene);//-
	d3d9_hook->hookAt(hk_res, hk_reset);
	client_dll_hook->hookAt(37, hkFrameStageNotify);
	client_dll_hook->hookAt(22, hkCreateMove_Proxy);
	MH_EnableHook(MH_ALL_HOOKS);
	//model_cache_hook->hook(hook_model_find, 10);
}
void hook_manager::unhook_all()
{
	/*IN_GAME*/
	g_surface->LockCursor();
	g_input_system->EnableInput(true);
	/*VMT*/
	panel_hook->unhook();
	client_mode_hook->unhook();
	surface_hook->unhook();
	net_channel_hook->unhook();
	/*MIN HOOK*/
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(g_cvar->FindVar("sv_cheats"));
	MH_RemoveHook(p_device);
	MH_RemoveHook(g_model_render);//model render
	MH_RemoveHook(g_base_client_dll);
	/*IN_GAME*/
	g_menu.shutdown();
	g_event_handler.shutdown();
}
