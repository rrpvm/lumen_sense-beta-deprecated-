#pragma once
#include "../../d3d9/Include/d3d9.h" //for idiots which cant download directx sdk. xd lol, its my private cheat. why i speak myself? is it a shiza?
#pragma comment(lib,"d3d9.lib")
#include "../interfaces/IVEngine.hpp"
#include "../interfaces/IClientEntityList.hpp"
#include <interfaces\IBaseClientDll.hpp>
#include "../interfaces/IClientEntity.hpp"
#include <interfaces\CPanel.hpp>
#include <interfaces\ISurface.hpp>
#include <interfaces\IVModelInfo.h>
#include <interfaces\IVRenderView.h>
#include <interfaces\IInputSystem.h>
#include <interfaces\IVEffects.h>
#include <interfaces\IVRenderBeam.h>
#include <interfaces\game_event_manager.h>
#include <interfaces\glow.h>
#include <interfaces\IClientMode.h>
#include <interfaces\ICvar.h>
#include <interfaces\CInput.h>
#include <interfaces\ModelCache.h>
#include <interfaces/Prediction.h>
#include "../utils/utils.h"
class sdk : public singleton <sdk> {
public:
	void create_interfaces();
	template <typename T>
	static constexpr auto relativeToAbsolute(uintptr_t address) noexcept
	{
		return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
	}
};

inline IVEngineClient* g_engine = nullptr;
inline IClientEntityList* g_entity_list = nullptr;
inline IBaseClientDLL* g_base_client_dll = nullptr;
inline IWeaponSystem* g_weapon_system = nullptr;
inline CPanel* g_vgui_panel = nullptr;//CPanel is IPanel
inline ISurface* g_surface = nullptr;
inline IVModelInfo* g_model_info = nullptr;
inline IMaterialSystem* g_mat_system = nullptr;
inline IVModelRender* g_model_render = nullptr;
inline IVRenderView* g_render_view = nullptr;
inline c_globalvar_base* g_global_vars = nullptr;
inline IInputSystem* g_input_system = nullptr;
inline IVEffects* g_effects = nullptr;
inline IViewRenderBeams* g_render_beam = nullptr;
inline IGameEventManager* g_event_manager = nullptr;
inline IEngineTrace* g_trace_gay = nullptr;//gay or ray ?
inline CGlowObjectManager* glow_manager = nullptr;
inline IDirect3DDevice9* p_device = nullptr;
inline IClientMode* g_client_mode = nullptr;
inline ICvar* g_cvar = nullptr;
inline CInput* g_input = nullptr;
inline CClientState* g_pClientState = nullptr;
inline IMDLCache* g_model_cache = nullptr;
inline CNetworkStringTableContainer* client_string_table_container = nullptr;
inline IGameMovement* g_pMovement = nullptr;
inline IMoveHelper* g_pMoveHelper = nullptr;
inline CPrediction* g_pPrediction = nullptr;

inline uintptr_t linegoesthrusmoke = utils::get().pattern_mask_scan("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");


#define time_to_ticks(dt)  static_cast<int>(0.5f + dt / g_global_vars->interval_per_tick)
#define ticks_to_time(t) (g_global_vars->interval_per_tick * (t))
