/*#include "hooks.h"
#include "sdk.h"
#include "../backend/renderer.h"
#include <algorithm>
#include "../backend/config.h"
#include "../backend/global_data.h"
#include "../visuals.h"
#include "../chams.h"
#include "../lumen_legitbot.h"
#include "../sdk/EnginePrediction.h"
#include "../interfaces/ConVar.h"
#include "../SpoofedConVar.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui.h"
//declarations

long __stdcall hkEndScene(IDirect3DDevice9* pDevice);
long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
void __fastcall hkPaintTraverse(void* edx, int ecx, unsigned int panel, bool f, bool d);
void __fastcall hkLockCursor(void* edx, void* ecx);
void __stdcall hkFrameStageNotify(ClientFrameStage_t stage);
void __stdcall hkOverrideView(CViewSetup* p_setup);
void __fastcall hkDrawModelExecute(void* _this, int edx, IMatRenderContext* ctx, const DrawModelState_t& state,  ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBone);
void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket);
void __fastcall hkCreateMove_Proxy(void* _this, int, int sequence_number, float input_sample_frametime, bool active);
void __stdcall hkInMouseActive();
typedef void(__fastcall* fnDrawModelExecure)(void*, int, IMatRenderContext*, const DrawModelState_t&,  ModelRenderInfo_t&, matrix3x4_t*);
typedef long(__stdcall* oEndScene)(IDirect3DDevice9*);
typedef long(__stdcall* oReset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef void(__thiscall* oPaintTraverse)(void* ecx, unsigned int panel, bool f, bool d);
typedef void(__thiscall* oLockFn)(void* ecx);
typedef void(__thiscall* oOverrideFn)(void*ecx, CViewSetup* pSetup);
typedef void(__stdcall* oFrameStageNotify)(ClientFrameStage_t stage);
typedef void(__fastcall* oCreateMove)(void*ecx, int edx, int , float , bool);
typedef void(__thiscall* oInMouseInput)(void* _ecx);

void hooks::attachHooks()
{
	//client_dll_hook.hook(hkInMouseActive, 15);//we get CInput
	gHlClient->callInMouseActive();
	d3d9hook.hook(hkEndScene, EndScene);
	d3d9hook.hook(hkReset, 16);
	panelhook.hook(hkPaintTraverse, paintTraverse);
	surfacehook.hook(hkLockCursor, 67);
	mdlrenderhook.hook(hkDrawModelExecute, DME);
	client_dll_hook.hook(hkFrameStageNotify, 37);
	client_dll_hook.hook(hkCreateMove_Proxy, 22);
	client_mode_hook.hook(hkOverrideView, 18);
}
void hooks::do_unhook()
{
	g_InputSystem->EnableInput(true);
	d3d9hook.unhook();
	panelhook.unhook();
	surfacehook.unhook();
	mdlrenderhook.unhook();
	client_mode_hook.unhook();
	client_dll_hook.unhook();
}
#define anim_fps 120
long __stdcall hkEndScene(IDirect3DDevice9* pDevice)
{
	static oEndScene oFunction = d3d9hook.get_original<oEndScene>(EndScene);
	pDevice->SetVertexShader(nullptr);
	pDevice->SetPixelShader(nullptr);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_FOGENABLE, false);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
	pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);//set to false
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	static ConVar* crosshair = g_Cvar->FindVar("weapon_debug_spread_show");
	static ConVar* mat_ambient_r = g_Cvar->FindVar("mat_ambient_light_r");
	static ConVar* mat_ambient_g = g_Cvar->FindVar("mat_ambient_light_g");
	static ConVar* mat_ambient_b = g_Cvar->FindVar("mat_ambient_light_b");
	static ConVar* viewmodel = g_Cvar->FindVar("viewmodel_fov");
	static ConVar* viewmodel_offset_x = g_Cvar->FindVar("viewmodel_offset_x");
	static ConVar* viewmodel_offset_y = g_Cvar->FindVar("viewmodel_offset_y");
	static ConVar* viewmodel_offset_z = g_Cvar->FindVar("viewmodel_offset_z");
	//static ConVar* net_fakelag = g_Cvar->FindVar("net_fakelag");detect hueta
	static ConVar* nightmode_za_5_kopeek = g_Cvar->FindVar("mat_force_tonemap_scale");
	if (g_cfg_system.show_menu)
	{
		nightmode_za_5_kopeek->SetValue(1.0f);
	} else nightmode_za_5_kopeek->SetValue(0.f);
	viewmodel->m_fnChangeCallbacks.m_Size = 0;	
	viewmodel_offset_x->m_fnChangeCallbacks.m_Size = 0;
	viewmodel_offset_y->m_fnChangeCallbacks.m_Size = 0;
	viewmodel_offset_z->m_fnChangeCallbacks.m_Size = 0;

	static bool once = true;
	if (once)
	{
		g_cfg_system.viewmodel_fov = viewmodel->GetInt();
		g_cfg_system.viewmodel_fovX = viewmodel_offset_x->GetInt();
		g_cfg_system.viewmodel_fovY = viewmodel_offset_y->GetInt();
		g_cfg_system.viewmodel_fovZ = viewmodel_offset_z->GetInt();
		g_cfg_system.post_effect_ambient.r = mat_ambient_r->GetFloat();
		g_cfg_system.post_effect_ambient.g = mat_ambient_g->GetFloat();
		g_cfg_system.post_effect_ambient.b = mat_ambient_b->GetFloat();
		once = false;
	}
	crosshair->SetValue(g_cfg_system.force_crosshair);
	viewmodel->SetValue(g_cfg_system.viewmodel_fov);
	viewmodel_offset_x->SetValue(g_cfg_system.viewmodel_fovX);
	viewmodel_offset_y->SetValue(g_cfg_system.viewmodel_fovY);
	viewmodel_offset_z->SetValue(g_cfg_system.viewmodel_fovZ);
	mat_ambient_r->SetValue(g_cfg_system.post_effect_ambient.r / 255.f);
	mat_ambient_g->SetValue(g_cfg_system.post_effect_ambient.g / 255.f);
	mat_ambient_b->SetValue(g_cfg_system.post_effect_ambient.b / 255.f);

//	net_fakelag->m_fnChangeCallbacks.m_Size = 0;
	//net_fakelag->SetValue(g_cfg_system.net_fake_lag_cvar);

	renderer::get().initialize(pDevice);
	g_menu.init(pDevice);
	g_menu.render();
	
		
	return oFunction(pDevice);
}
long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	static oReset oFunction = d3d9hook.get_original<oReset>(16);

	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto hr = oFunction(device, pPresentationParameters);

	if (hr >= 0)
		ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}
void __fastcall hkPaintTraverse(void* edx, int ecx, unsigned int panel, bool f, bool d)
{
	static oPaintTraverse oTraverse = panelhook.get_original<oPaintTraverse>(paintTraverse);
	if (!strcmp("HudZoom", g_VGuiPanel->GetName(panel)) && g_cfg_system.remove_scope)//remove scope
		return;
	static unsigned int panelId = 0;
	oTraverse(g_VGuiPanel, panel, f, d);


	if (panelId == 0) {
		const auto panelName = g_VGuiPanel->GetName(panel);
		if (!strcmp(panelName, "MatSystemTopPanel")) {
			panelId = panel;
		}
	}
	else if (panelId == panel)
	{
		static bool once = false; if (!once) {
			global_data::get().CourierNew = g_VGuiSurface->CreateFont_();
			g_VGuiSurface->SetFontGlyphSet(global_data::get().CourierNew, "Verdana", 12, 300, 0, 0, FONTFLAG_OUTLINE);
			once = true;
		}													
		if (g_Engine->IsInGame()) {
			//g_DebugOverlay->add_sphere_overlay(Vector(142, -614, -319), 20.0f, 20, 450, 255, 0, 0, 255, 0.025f);
			for (int i = 1; i < 64; ++i)
			{
				C_BasePlayer* player = static_cast<C_BasePlayer*>(EntityList->GetClientEntity(i));
				if (player != nullptr && player != global_data::get().local_player)
				{
					if (player->IsAlive())continue;
					if (player->is_dormant())continue;
					if (!g_cfg_system.team_esp && player->get_team() == global_data::get().local_player->get_team())continue;
					visuals::get().get_esp_player(player, i);
					if (g_cfg_system.box_esp)visuals::get().draw_esp_box();
					if (g_cfg_system.hp_esp)visuals::get().draw_health_esp();
					if (g_cfg_system.name_esp)visuals::get().draw_name_esp();
					if (g_cfg_system.dLight)visuals::get().set_dlight();
					if (g_cfg_system.trails_on_rails)visuals::get().trails();
					if (g_cfg_system.skeleton_esp)visuals::get().bone_esp();
					if (g_cfg_system.draw_aim_fov && g_cfg_system.general_aim_settings.fov_type == 1)
					{
						g_DebugOverlay->add_sphere_overlay(player->get_hitbox_position(0), g_LegitBot.get_fov(player) * 1.3f, 10, 20, 110, 0, 0, 255, .03f);                     
					}
				}			
			}
			if (g_cfg_system.draw_aim_fov && g_cfg_system.general_aim_settings.fov_type == 0) {
				g_VGuiSurface->DrawSetColor(255, 0, 0, 255);
				int x, y;
				g_Engine->GetScreenSize(x, y);
				x = x / 2; y = y / 2;				
				g_VGuiSurface->DrawOutlinedCircle(x, y, g_LegitBot.get_fov(nullptr), 150);
			}
			if (g_cfg_system.backtrack && g_cfg_system.general_aim_settings.aim) {
				g_VGuiSurface->DrawSetColor(255, 0, 255, 255);
				for (int i = 0; i < 64; i++)
				{
					for (int j = 0; j < 12; j++)
					{
						Vector out; ;
						if (utils::get().WorldToScreen(g_LegitBot.hitbox_positions[i][j].hitboxPos, out))
							g_VGuiSurface->DrawFilledRect(out.x, out.y, out.x + 2, out.y + 2);
					}
				}
			}
			if (g_cfg_system.local_trails_on_rails)
			{
				visuals::get().local_trails();
			}
			if (g_cfg_system.remove_scope)
			{
				visuals::get().draw_scope();
			}
			
		}
	}
}
void __fastcall hkLockCursor(void* edx, void* ecx)
{
	static oLockFn orig = surfacehook.get_original<oLockFn>(67);
	if (g_cfg_system.show_menu)//anti crash
	{
		g_VGuiSurface->UnlockCursor();
		g_InputSystem->EnableInput(false);
		return;

	}
	g_InputSystem->EnableInput(true);
	orig(g_VGuiSurface);
}
void __stdcall hkFrameStageNotify(ClientFrameStage_t stage)
{
	
	static oFrameStageNotify ofunc = client_dll_hook.get_original<oFrameStageNotify>(37);

	global_data::get().local_player = static_cast<C_BasePlayer*>(EntityList->GetClientEntity(g_Engine->GetLocalPlayer()));
	
	if (stage == FRAME_RENDER_START) {
		
	}
	ofunc( stage);
}
void __stdcall hkOverrideView(CViewSetup* p_setup)
{
	static oOverrideFn orig = client_mode_hook.get_original<oOverrideFn>(18);
	if (g_Engine->IsInGame()) {
		if (!global_data::get().local_player->is_scoped())
		{
			p_setup->fov = g_cfg_system.fov;
			//	p_setup->viewmodel_fov = g_cfg_system.viewmodel_fov;
		}
		global_data::get().local_fov = p_setup->fov;
	}
	return orig(g_ClientMode,p_setup);
}
 void __fastcall hkDrawModelExecute(void* _this, int edx, IMatRenderContext* ctx, const DrawModelState_t& state,  ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
 {
	 static fnDrawModelExecure oDraw = mdlrenderhook.get_original< fnDrawModelExecure>(DME);
	 if (
		 !g_Engine->IsInGame() &&
		 !strstr(pInfo.pModel->szName, "arms") &&
		 !strstr(pInfo.pModel->szName, "weapons/v_"))
	 {
		 return oDraw(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
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
			IMaterial* mat = g_MatSystem->FindMaterial(mat_s, TEXTURE_GROUP_OTHER);
			if (g_cfg_system.wireframe_smoke) {				
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				if (strcmp(mat_s, "particle/vistasmokev1/vistasmokev1_smokegrenade"))mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);//if remove smoke
			}
			else {
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
				if (strcmp(mat_s, "particle/vistasmokev1/vistasmokev1_smokegrenade"))mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);//if remove smoke
			}
		}
	 if (is_player && g_cfg_system.backtrack_chams) {
		 Vector oldOrigin = pInfo.origin;
		 auto entity = static_cast<C_BasePlayer*>(EntityList->GetClientEntity(pInfo.entity_index));
		 if (entity && entity->hp() > 0 && entity != global_data::get().local_player) {
			 if (global_data::get().local_player && global_data::get().local_player->hp() > 0)
			 {
				 for (int j = 1; j < 11; j++) {					
					 if (g_LegitBot.hitbox_positions[pInfo.entity_index][j].simtime && g_LegitBot.hitbox_positions[pInfo.entity_index][j].simtime + 1 > global_data::get().local_player->get_simulation_time())
					 {
						 pInfo.origin = g_LegitBot.hitbox_positions[pInfo.entity_index][j].hitboxPos;						
						if(g_LegitBot.hitbox_positions[pInfo.entity_index][j].best) chams::get().overrideMat(metallic_mat, false, g_cfg_system.wireframe);
						else chams::get().overrideMat(glow, false, g_cfg_system.wireframe);
						 oDraw(mdlRender, edx, ctx, state, pInfo, g_LegitBot.hitbox_positions[pInfo.entity_index][j].static_matrix);
						 mdlRender->ForcedMaterialOverride(nullptr);
					 }
					 else
					 {
						// memset(&g_LegitBot.hitbox_positions[pInfo.entity_index][j].hitboxPos, 0, sizeof(g_LegitBot.hitbox_positions[pInfo.entity_index][j].hitboxPos));
						 memset(&g_LegitBot.hitbox_positions[pInfo.entity_index][j].static_matrix, 0, sizeof(g_LegitBot.hitbox_positions[pInfo.entity_index][j].static_matrix));
					 }
				 }
			 }
		 }
		 pInfo.origin = oldOrigin;
	 }	
	 if (is_player && !shadow && g_cfg_system.chams)
	 {		
		 switch (g_cfg_system.chams_type)
		 {
		 case 0:
			 if (g_cfg_system.chams_animate)
			 {
				 chams::get().overrideMat(default_mat_pulse, g_cfg_system.ignore_z, g_cfg_system.wireframe);
			 }
			 else chams::get().overrideMat(default_mat, g_cfg_system.ignore_z, g_cfg_system.wireframe, g_cfg_system.chams_color / 255.f );
			 break;

		 case 1:
			 if (g_cfg_system.chams_animate)
			 {
				 chams::get().overrideMat(metallic_mat_pulse, g_cfg_system.ignore_z, g_cfg_system.wireframe);
			 }
			 else chams::get().overrideMat(metallic_mat, g_cfg_system.ignore_z, g_cfg_system.wireframe);
			 break;

		 case 2:
			 if (g_cfg_system.chams_animate)
			 {
				 chams::get().overrideMat(pearlescent_mat_pulse, g_cfg_system.ignore_z, g_cfg_system.wireframe, g_cfg_system.chams_color /255.f);
			 }
			 else chams::get().overrideMat(pearlescent_mat, g_cfg_system.ignore_z, g_cfg_system.wireframe, g_cfg_system.chams_color / 255.f);
			 break;
		 case 3: {
			 float flColor[3] = { g_cfg_system.chams_color.r / 25.f, g_cfg_system.chams_color.g / 25.f,  g_cfg_system.chams_color.b / 25.f };
			 if (g_cfg_system.chams_animate)
			 {
				 //g_RenderView->SetBlend(1.f);
				 //g_RenderView->SetColorModulation(flColor);
				 mdlRender->ForcedMaterialOverride(glow_armrace);
			 }
			 else {
				 g_RenderView->SetBlend(1.f);
				 g_RenderView->SetColorModulation(flColor);
			 }
			 break;
		 }
		 case 4:
			 chams::get().overrideMat(glass, g_cfg_system.ignore_z, g_cfg_system.wireframe);
			 break;
		 case 5:
			 chams::get().overrideMat(crystal, g_cfg_system.ignore_z, g_cfg_system.wireframe);
			 break;
		 case 6:
		 {
			 g_RenderView->SetBlend(.7f);
			 chams::get().overrideMat(default_mat, false, g_cfg_system.wireframe, g_cfg_system.chams_color );
			 oDraw(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);			
			 chams::get().overrideMat(glow, true, g_cfg_system.wireframe, g_cfg_system.chams_color / 255.f);
			 break;
		 }			 
		 }
	 }	
	 if (is_arm  && g_cfg_system.hand_chams)
	 {
		 switch (g_cfg_system.hand_chams_type)
		 {
		 case 0:
			 if (g_cfg_system.chams_animate)
			 {
				 chams::get().overrideMat(default_mat_pulse, false, g_cfg_system.wireframe);
			 }
			 else chams::get().overrideMat(default_mat, false, g_cfg_system.wireframe, g_cfg_system.hand_chams_color / 255.f);
			 break;

		 case 1:
			 if (g_cfg_system.chams_animate)
			 {
				 chams::get().overrideMat(metallic_mat_pulse, false, g_cfg_system.wireframe);
			 }
			 else chams::get().overrideMat(metallic_mat, false, g_cfg_system.wireframe);
			 break;

		 case 2:
			 if (g_cfg_system.chams_animate)
			 {
				 chams::get().overrideMat(pearlescent_mat_pulse, false, g_cfg_system.wireframe, g_cfg_system.hand_chams_color / 255.f);
			 }
			 else chams::get().overrideMat(pearlescent_mat, false, g_cfg_system.wireframe, g_cfg_system.hand_chams_color / 255.f);
			 break;
		 case 3: {
			 float flColor[3] = { g_cfg_system.chams_color.r / 25.f, g_cfg_system.chams_color.g / 25.f,  g_cfg_system.hand_chams_color.b / 25.f };
			 if (g_cfg_system.chams_animate)
			 {
				 g_RenderView->SetBlend(1.f);
				 g_RenderView->SetColorModulation(flColor);
			 }
			 else {
				 g_RenderView->SetBlend(1.f);
				 g_RenderView->SetColorModulation(flColor);
			 }
			 break;
		 }
		 case 4:
			 chams::get().overrideMat(weapon_blink, false, g_cfg_system.wireframe);
			 break;
		 case 5:
			 chams::get().overrideMat(weapon_gold, false, g_cfg_system.wireframe);
			 break;
		 case 6:
		 {		 
			 chams::get().overrideMat(crystal, false, g_cfg_system.wireframe, g_cfg_system.hand_chams_color / 255.f);
			 oDraw(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);			
		 }
		 }
	 }
	 if (is_weapon && g_cfg_system.weapon_chams && !is_sleeve)
	 {
		 switch (g_cfg_system.wpn_chams_type)
		 {
		 case 0:
			 if (g_cfg_system.chams_animate)
			 {
				 chams::get().overrideMat(default_mat_pulse, false, g_cfg_system.wireframe);
			 }
			 else chams::get().overrideMat(default_mat, false, g_cfg_system.wireframe, g_cfg_system.wpn_chams_color / 255.f);
			 break;

		 case 1:
			 if (g_cfg_system.chams_animate)
			 {
				 chams::get().overrideMat(metallic_mat_pulse, false, g_cfg_system.wireframe);
			 }
			 else chams::get().overrideMat(metallic_mat, false, g_cfg_system.wireframe );
			 break;

		 case 2:
			 if (g_cfg_system.chams_animate)
			 {
				 chams::get().overrideMat(pearlescent_mat_pulse, false, g_cfg_system.wireframe, g_cfg_system.wpn_chams_color  / 255.f);
			 }
			 else chams::get().overrideMat(pearlescent_mat, false, g_cfg_system.wireframe, g_cfg_system.wpn_chams_color / 255.f);
			 break;
		 case 3: {
			 float flColor[3] = { g_cfg_system.chams_color.r / 25.f, g_cfg_system.chams_color.g / 25.f,  g_cfg_system.hand_chams_color.b / 25.f };
			 if (g_cfg_system.chams_animate)
			 {
				 g_RenderView->SetBlend(1.f);
				 g_RenderView->SetColorModulation(flColor);
			 }
			 else {
				 g_RenderView->SetBlend(1.f);
				 g_RenderView->SetColorModulation(flColor);
			 }
			 break;
		 }
		 case 4:
			 chams::get().overrideMat(weapon_blink, false, g_cfg_system.wireframe);
			 break;
		 case 5:
			 chams::get().overrideMat(weapon_gold, false, g_cfg_system.wireframe);
			 break;
		 case 6:
		 {
			 chams::get().overrideMat(glass, false, g_cfg_system.wireframe, g_cfg_system.wpn_chams_color / 255.f);
			 oDraw(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
		 }
		 }
	 }
	 oDraw(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
	 mdlRender->ForcedMaterialOverride(nullptr);
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
void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
	static oCreateMove oMoves = client_dll_hook.get_original<oCreateMove>(22);
	oMoves(gHlClient, 0, sequence_number, input_sample_frametime, active);
	if (!g_Input)return;
	auto cmd = g_Input->GetUserCmd(sequence_number);
	auto verified = g_Input->GetVerifiedCmd(sequence_number);
	if (!cmd || !cmd->command_number)
		return;
	if (!global_data::get().local_player)return;

	

	if(g_cfg_system.moon_walk)g_Misc.gravity_walk(cmd);
	if(g_cfg_system.bunny_hop)g_Misc.bunny_hop(cmd);
	if(g_cfg_system.radar_hack)g_Misc.radar_hack();
	if(g_cfg_system.fakelags && !g_Engine->IsVoiceRecording())g_Misc.fake_lags(bSendPacket);
	if(g_cfg_system.chat_spam)g_Misc.chat_spam();
	if (g_cfg_system.backtrack)g_LegitBot.back_track(cmd);
	if(g_cfg_system.general_aim_settings.engine_prediction)engine_prediction::RunEnginePred(cmd);
	if (g_cfg_system.general_aim_settings.rcs_standalone)g_LegitBot.rcs_standalone(cmd);
	if(g_cfg_system.general_aim_settings.aim)g_LegitBot.aimbot(cmd, bSendPacket);	
	if (g_cfg_system.general_aim_settings.engine_prediction)engine_prediction::EndEnginePred(cmd);
	//if (g_cfg_system.backtrack)g_LegitBot.back_track(cmd);
	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();
}
void __stdcall hkInMouseActive()
{
	if (!g_Input) {
		__asm {
			mov g_Input, ecx
		}
	static oInMouseInput original = client_dll_hook.get_original<oInMouseInput>(15);
	//CInput* buffer;
	//g_Input = buffer;//g_Input now == nullptr
	original(gHlClient);
	/*if (!g_Input) {
		__asm {
			mov g_Input, ecx
	}
	}
	 
}
*/