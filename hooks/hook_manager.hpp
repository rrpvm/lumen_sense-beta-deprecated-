#pragma once
#include "VMT.h"
#include <memory>
#include "../includes.hpp"
#include "MinHook.h"
#include <hooks\MinHook.h>
class hook_manager {
public:
	enum index {
		hk_end_sc = 42,
		hk_traverse = 41,
		hk_create_m = 22,
		hk_draw_model_ex = 21,
		hk_res = 16,
	};
	hook_manager() {		
		d3d9_hook = std::make_unique<MinHook>();
		model_render_hook = std::make_unique<MinHook>();
		sv_cheats_hook = std::make_unique<MinHook>();
		client_dll_hook = std::make_unique<MinHook>();
		net_channel_hook = std::make_unique<VMT>();		
		panel_hook = std::make_unique<VMT>();
		client_mode_hook = std::make_unique<VMT>();
		surface_hook = std::make_unique<VMT>();
		model_cache_hook = std::make_unique<VMT>();		
		engine_hook = std::make_unique<VMT>();
	};
	~hook_manager() {

	};
	void init_all();
	void hook_all();
	void  unhook_all();
	std::unique_ptr<MinHook>d3d9_hook;
	std::unique_ptr<MinHook>model_render_hook;
	std::unique_ptr<MinHook>sv_cheats_hook;
	std::unique_ptr<MinHook>client_dll_hook;
	std::unique_ptr<VMT>net_channel_hook;
	std::unique_ptr<VMT>panel_hook;
	std::unique_ptr<VMT>surface_hook;
	std::unique_ptr<VMT>client_mode_hook;	
	std::unique_ptr<VMT>model_cache_hook;	
	std::unique_ptr<VMT>engine_hook;
private:
};
inline hook_manager hook_manager_;