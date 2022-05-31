#pragma once
#include "../imgui/imgui.h"
#include <d3d9.h>
#include "utils/Color.h"
class menu 
{
public:
	void init(IDirect3DDevice9* pDevice);
	void render();
	void ResetDevice(IDirect3DDevice9* pDevice);
	void shutdown();
	inline void render_legitbot_tab();
	inline void render_visual_tab();
	inline void render_player_tab();
	inline void render_misc_tab();
	inline void render_cfg_tab();
	WNDPROC wndproc_orig;
	ImFont* menu_title = nullptr;
	ImFont* menu_main_font = nullptr;
	ImFont* menu_main_font_small = nullptr;
	ImFont* weapon_font = nullptr;
	void color();
	void styled();
	bool ButtonColor(const char* label, ImVec2 size, Color col, bool lined = false);
	bool ButtonColor(int r,int g,int b);
	void child_color(const char* label, ImVec2 size, bool borders,Color col);
	void color_picker_proxy(Color* to_out, const char* name, bool alpha = false);
	void color_picker(Color* to_out, const char* popup_name,const char* btn, bool alpha = false);
	void slider_int(const char* label, int* item, int min, int max, float width);	
	void key_bind(const char* label,int* k,  ImFont* font);
private:
	float fl = 0.0f;//indicate and get alpha of menu
	int curr_tab = 0;
	bool initialized = false;
	ImColor btn_color;
};

inline menu g_menu;