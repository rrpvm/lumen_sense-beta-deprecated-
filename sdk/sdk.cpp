#include "sdk.hpp"
#include <utils\utils.h>
#include "csgo_struct.h"
#include <interfaces\IClientMode.h>
void sdk::create_interfaces()
{
	HMODULE shaderapidx9 = GetModuleHandleW(L"shaderapidx9.dll");
	HMODULE client = GetModuleHandleW(L"client.dll");

	p_device = **(IDirect3DDevice9***)(utils::get().PatternScan(shaderapidx9, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	g_weapon_system = *(IWeaponSystem**)(utils::get().PatternScan(client, "8B 35 ? ? ? ? FF 10 0F B7 C0") + 2);
	g_global_vars = **(c_globalvar_base***)(utils::get().PatternScan(client, "A1 ? ? ? ? 5E 8B 40 10") + 1);
	g_render_beam = *reinterpret_cast<IViewRenderBeams**>(utils::get().PatternScan(client, "A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") + 1);
	glow_manager = *(CGlowObjectManager**)(utils::get().PatternScan(client, "0F 11 05 ? ? ? ? 83 C8 01") + 3);
	g_input = *(CInput**)(utils::get().PatternScan(client, "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
	g_entity_list = (IClientEntityList*)utils::getInterface("client.dll", "VClientEntityList0");
	g_base_client_dll = (IBaseClientDLL*)utils::getInterface("client.dll", "VClient0");
	g_engine = (IVEngineClient*)utils::getInterface("engine.dll", "VEngineClient0");
	g_vgui_panel = (CPanel*)utils::getInterface("vgui2.dll", "VGUI_Panel0");
	g_mat_system = (IMaterialSystem*)utils::getInterface("materialsystem.dll", "VMaterialSystem0");
	g_surface = (ISurface*)utils::getInterface("vguimatsurface.dll", "VGUI_Surface0");
	g_model_info = (IVModelInfo*)utils::getInterface("engine.dll", "VModelInfoClient0");
	g_model_render = (IVModelRender*)utils::getInterface("engine.dll", "VEngineModel0");
	g_render_view = (IVRenderView*)utils::getInterface("engine.dll", "VEngineRenderView0");
	g_input_system = (IInputSystem*)utils::getInterface("inputsystem.dll", "InputSystemVersion0");
	g_effects = (IVEffects*)utils::getInterface("engine.dll", "VEngineEffects0");
	g_trace_gay = (IEngineTrace*)utils::getInterface("engine.dll", "EngineTraceClient0");
	g_event_manager = (IGameEventManager*)utils::getInterfaceStatic("engine.dll", "GAMEEVENTSMANAGER002");
	g_model_cache = (IMDLCache*)utils::getInterfaceStatic("datacache.dll", "MDLCache004");
	client_string_table_container = (CNetworkStringTableContainer*)utils::getInterface("engine.dll","VEngineClientStringTable0");


	g_pMovement = (IGameMovement*)utils::getInterface("client.dll", "GameMovement0");
	g_pMoveHelper = **(IMoveHelper***)(utils::get().PatternScan(client, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
	g_pPrediction = (CPrediction*)utils::getInterface("client.dll", "VClientPrediction0");
	g_pClientState = utils::get_method(g_engine, 12).add(16).get(2).as< CClientState* >();//pasted
	do {
		g_client_mode = **(IClientMode***)((*(uintptr_t**)g_base_client_dll)[10] + 0x5);//we find it from handle 	

	} while (!g_client_mode);
	g_cvar = (ICvar*)utils::getInterface("vstdlib.dll", "VEngineCvar0");
	g_netvar.init();

}
