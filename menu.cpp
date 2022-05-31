#include "menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "sdk/sdk.hpp"
//#include "helpers/font.h"
#include "backend/cfg_system.hpp"
#include <sdk\csgo.h>
#include <imgui_internal.h>
#define fl_rgb(r,g,b){r *= 255.f; g*=255.f; b *= 255.f;}; 
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (cfg_vars.show_menu && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
	{
		return true;
	}
	return CallWindowProc(g_menu.wndproc_orig, hwnd, msg, wParam, lParam);
}
void menu::init(IDirect3DDevice9* pDevice)
{
	if (!initialized) {
		wndproc_orig = (WNDPROC)SetWindowLongPtr(FindWindowA(NULL, "Counter-Strike: Global Offensive"), GWLP_WNDPROC, (LONG_PTR)WndProc);
		ImGuiContext* ctx = ImGui::CreateContext();
		ImGui::SetCurrentContext(ctx);
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(FindWindowA(0,"Counter-Strike: Global Offensive"));
		ImGui_ImplDX9_Init(pDevice);
	//	menu_title = io.Fonts->AddFontFromMemoryTTF(&poppins, sizeof poppins, 36.f);
		menu_title = io.Fonts->AddFontFromFileTTF("C:\\Users\\Roman\\Downloads\\Permanent_Marker\\PermanentMarker-Regular.ttf", 48);
		menu_main_font = io.Fonts->AddFontFromFileTTF("C:\\Users\\Roman\\Downloads\\Orbitron\\static\\Orbitron-Regular.ttf", 16);
		menu_main_font_small = io.Fonts->AddFontFromFileTTF("C:\\Users\\Roman\\Downloads\\Orbitron\\static\\Orbitron-Regular.ttf", 12);
		weapon_font = io.Fonts->AddFontFromFileTTF("C:\\Users\\Roman\\Downloads\\undefeated.ttf", 24);
		//ImFontConfig cfg;
		//menu_main_font = io.Fonts->AddFontFromMemoryTTF(&raleway, sizeof raleway, 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		//io.Fonts->AddFontDefault();
		//io.Fonts->Build();
		initialized = true;
		
	}
}
void menu::render()
{
	if (GetAsyncKeyState(VK_INSERT) & 1) { cfg_vars.show_menu = !cfg_vars.show_menu; }
	if (cfg_vars.show_menu) {

		ImGui::SetNextWindowSize(ImVec2(770, 700), ImGuiCond_Always);
		ImGuiStyle& style = ImGui::GetStyle();
		{
			style.WindowRounding = 1.0f;
			style.WindowPadding = ImVec2(0.f, 3.f);
			style.DisplayWindowPadding = ImVec2(0.f, 0.f);
			style.DisplaySafeAreaPadding = ImVec2(0.f, 0.f);
			style.TouchExtraPadding = ImVec2(0.f, 0.f);
			style.WindowBorderSize = 1.f;
			style.FramePadding = ImVec2(0.f, 0.f);
			style.ItemSpacing = ImVec2(0.f, 2.f);
			style.FrameRounding = 0.0f;
			style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style.Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
			style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
		}
		style.FrameRounding = 1.0f;
		style.Colors[ImGuiCol_FrameBgActive] = ImColor(33, 33, 33);
		style.Colors[ImGuiCol_FrameBgHovered] = ImColor(33, 33, 33);
		style.Colors[ImGuiCol_CheckMark] = ImColor(191, 44, 67);
		style.Colors[ImGuiCol_SliderGrab] = ImColor(194, 26, 61);
		ImGui::Begin("LumenSense", &cfg_vars.show_menu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::BeginChild("title", ImVec2(770, 40), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
			ImGui::PushFont(menu_title);
			static ImVec2 size = ImGui::CalcTextSize("LUMENSENSE");
			ImGui::SetCursorPos(ImVec2(770 / 2.f - size.x / 2.f, 40.f / 2.f - size.y / 2.f)); ImGui::Text("LUMENSENSE");
			ImGui::PopFont();
			ImGui::EndChild();
			static ImColor active = ImColor(55, 55, 55);
			static ImColor non_active = ImColor(22, 22, 22);
			ImGui::BeginChild("tabs", ImVec2(770, 30), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
			ImGui::PushFont(menu_main_font);
			curr_tab == 0 ? style.Colors[ImGuiCol_Button] = active : style.Colors[ImGuiCol_Button] = non_active;
			if (ImGui::Button("legitbot", ImVec2(770 / 7.f, 30.f)))
			{
				curr_tab = 0;
			}ImGui::SameLine();
			curr_tab == 1 ? style.Colors[ImGuiCol_Button] = active : style.Colors[ImGuiCol_Button] = non_active;
			if (ImGui::Button("players", ImVec2(770 / 7.f, 30.f)))
			{
				curr_tab = 1;
			}ImGui::SameLine();
			curr_tab == 2 ? style.Colors[ImGuiCol_Button] = active : style.Colors[ImGuiCol_Button] = non_active;
			if (ImGui::Button("visuals", ImVec2(770 / 7.f, 30.f)))
			{
				curr_tab = 2;
			}ImGui::SameLine();
			curr_tab == 3 ? style.Colors[ImGuiCol_Button] = active : style.Colors[ImGuiCol_Button] = non_active;
			if (ImGui::Button("skins", ImVec2(770 / 7.f, 30.f)))
			{
				curr_tab = 3;
			}ImGui::SameLine();
			curr_tab == 4 ? style.Colors[ImGuiCol_Button] = active : style.Colors[ImGuiCol_Button] = non_active;
			if (ImGui::Button("medals", ImVec2(770 / 7.f, 30.f)))
			{
				curr_tab = 4;
			}ImGui::SameLine();
			curr_tab == 5 ? style.Colors[ImGuiCol_Button] = active : style.Colors[ImGuiCol_Button] = non_active;
			if (ImGui::Button("misc", ImVec2(770 / 7.f, 30.f)))
			{
				curr_tab = 5;
			}ImGui::SameLine();
			curr_tab == 6 ? style.Colors[ImGuiCol_Button] = active : style.Colors[ImGuiCol_Button] = non_active;
			if (ImGui::Button("config", ImVec2(770 / 7.f, 30.f)))
			{
				curr_tab = 6;
			}
			ImGui::PopFont();
			ImGui::EndChild();
			ImGui::ColorBar("unicorn", ImVec2(770, 5), ImColor(15, 146, 234), ImColor(194, 26, 61));
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			//ImGui::PushFont(tabs_font);
			switch (curr_tab)
			{
			case 0:
				render_legitbot_tab();
				break;
			case 1:
				render_player_tab();
				break;
			case 2:
				render_visual_tab();
				break;

			case 5:
				render_misc_tab();
				break;
			case 6:
				render_cfg_tab();
				break;
			}
			//ImGui::PopFont();
		}
		ImGui::End();
	}

	if (csgo_data.local)
	{
		ImGui::StyleColorsDark();
		ImGuiStyle& s = ImGui::GetStyle();
		s.WindowRounding = 4.f;
		s.ItemSpacing = ImVec2(5, 5);
		s.WindowPadding = ImVec2(0, 5);
		ImGui::PushFont(menu_main_font);
		ImGui::SetNextWindowSize(ImVec2(300.f, 90.f), ImGuiCond_Always);
		ImGui::Begin("keybinds", &cfg_vars.indicators.enabled, ImGuiCond_Always | ImGuiWindowFlags_NoResize);
		{			
			ImGui::SetCursorPosY(2); ImGui::ColorBar("##indicators_unicorn", ImVec2(300.f, 4.f), ImColor(15, 146, 234), ImColor(194, 26, 61));
			if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_shoot)
			{
				ImGui::Text("auto shoot");
			}
			if (GetAsyncKeyState(cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->trigger_key))
			{
				ImGui::Text("trigger");
			}
			ImGui::SetCursorPosX(5);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			s.ItemSpacing = ImVec2(0, 8);
			ImGui::Text("chocked"); ImGui::SameLine();
			ImGui::PushItemWidth(190.f);
			s.FrameRounding = 3.f;
			s.Colors[ImGuiCol_PlotHistogram] = ImColor(0, 133, 233);
			s.Colors[ImGuiCol_FrameBg] = ImColor(.05f, 0.05f, 0.05f);
			ImGui::ProgressBar(float(csgo_data.local->get_chocked_commands())/12.f, ImVec2(-1,0), " ");
			ImGui::PopItemWidth();
		}
		ImGui::End();
		ImGui::PopFont();
	}		
}
void menu::ResetDevice(IDirect3DDevice9* pDevice)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	ImGui_ImplDX9_CreateDeviceObjects();
}
void menu::shutdown()
{
	//menu_title->ClearOutputData();
//	menu_main_font->ClearOutputData();
//	menu_title = nullptr;
//	menu_main_font = nullptr;
	HWND hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
	SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)wndproc_orig);
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
inline void menu::render_legitbot_tab()
{		
	ImGui::BeginChild("legit tab", ImVec2(770, 700), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_ChildBg];
	style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_ChildBg];
	style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_ChildBg];
	ImGui::PushFont(weapon_font);
	static ImColor active = ImColor(255, 255, 255);
	static ImColor non_active = ImColor(100, 100, 100);
	ImGui::BeginChild("##general object", ImVec2(770.f / 6.f, 50.f), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
	{
		csgo_data.active_wpn_type == 0 ? style.Colors[ImGuiCol_Text] = active : style.Colors[ImGuiCol_Text] = non_active;
		ImGui::Button("s", ImVec2(770.f / 6.f, 25.f));
		ImGui::PopFont();
		ImGui::PushFont(menu_main_font);
		static ImVec2 txt_size = ImGui::CalcTextSize("general");
		ImGui::SetCursorPosX(770.f / 6.f / 2.f - txt_size.x/2.f); ImGui::Text("general");
		ImGui::PopFont();
		ImGui::PushFont(weapon_font);
		ImGui::EndChild();
	}
	ImGui::SameLine();
	ImGui::BeginChild("##weapon_select", ImVec2(770.f /6.f * 5.f, 40.f), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
	{

		csgo_data.active_wpn_type == 1 ? style.Colors[ImGuiCol_Text] = active : style.Colors[ImGuiCol_Text] = non_active;
		if (ImGui::Button("A", ImVec2(770.f / 6.f, 30.f)))
		{
		}ImGui::SameLine();
		csgo_data.active_wpn_type == 2 ? style.Colors[ImGuiCol_Text] = active : style.Colors[ImGuiCol_Text] = non_active;
		if (ImGui::Button("K", ImVec2(770.f / 6.f, 30.f)))
		{
		}ImGui::SameLine();
		csgo_data.active_wpn_type == 3 ? style.Colors[ImGuiCol_Text] = active : style.Colors[ImGuiCol_Text] = non_active;
		if (ImGui::Button("S", ImVec2(770.f / 6.f, 30.f)))
		{
		}ImGui::SameLine();
		csgo_data.active_wpn_type == 4 ? style.Colors[ImGuiCol_Text] = active : style.Colors[ImGuiCol_Text] = non_active;
		if (ImGui::Button("d", ImVec2(770.f / 6.f, 30.f)))
		{
		}ImGui::SameLine();
		csgo_data.active_wpn_type == 5 ? style.Colors[ImGuiCol_Text] = active : style.Colors[ImGuiCol_Text] = non_active;
		if (ImGui::Button("Z", ImVec2(770.f / 6.f, 30.f)))
		{
		}ImGui::SameLine();
		non_active = ImColor(125, 125, 125);
		style.Colors[ImGuiCol_Text] = non_active;
		ImGui::EndChild();
	}
	ImGui::PopFont();
	ImGui::BeginChild("##settings_area", ImVec2(770, 700), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::PushFont(menu_main_font);

		//if weapon index > 7 = error window
		style.ChildBorderSize = 0.45f;
		style.Colors[ImGuiCol_Border] = ImColor(30, 30, 30);
		style.ItemSpacing = ImVec2(15, 7);
		style.ItemInnerSpacing = ImVec2(7, 5);
		style.FramePadding = ImVec2(4, 2);
		ImGui::SetCursorPosX(25); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		ImGui::BeginChild("##simple_funcs", ImVec2(770 / 3.f, 360), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::SetCursorPosX(35);			
			ImGui::Text("general");
			ImGui::ColorBar("##simple", ImVec2(770 / 3.f, 3), ImColor(15, 146, 234), ImColor(194, 26, 61));
			ImGui::Checkbox("aimbot", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->enabled); ImGui::SameLine(); key_bind("##aimbot_key", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->aimbot_key, menu_main_font_small);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);
			ImGui::Checkbox("silent", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->silent);
			ImGui::Checkbox("P silent", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->p_silent);
			ImGui::Checkbox("auto shoot", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_shoot); ImGui::SameLine(); key_bind("##auto_shot", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_shot_key, menu_main_font_small);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);
			ImGui::Checkbox("wall check", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->visibility_check);
			ImGui::Checkbox("engine predicition", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->engine_predict);
			ImGui::Checkbox("extrapolation", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->extrapolate);
			ImGui::Checkbox("[RCS]standalone", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->rcs_standalone);
			ImGui::Checkbox("[aim]auto stop", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_stop);
			ImGui::Checkbox("backtrack", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->backtrack);
			ImGui::Checkbox("nearest", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->nearest);
		}
		ImGui::EndChild();
		ImGui::SameLine();
		style.Colors[ImGuiCol_Border] = ImColor(25, 25, 25);
		style.ScrollbarSize = 0.0f;
		ImGui::BeginChild("##sliders_funcs", ImVec2(454,600), true, ImGuiWindowFlags_NoResize  |ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::SetCursorPosX(35);
			ImGui::Text("additional");
			ImGui::ColorBar("##simple", ImVec2(434, 3), ImColor(15, 146, 234), ImColor(194, 26, 61));
			style.ItemInnerSpacing = ImVec2(5, 5);
			style.ItemSpacing = ImVec2(20, 0);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);		
			ImGui::PushItemWidth(350);
			ImGui::SliderInt("fov", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->fov, 1, 540, "%d");
			style.ItemSpacing = ImVec2(20, 30);
			ImGui::SliderInt("P silent fov", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->p_silent_fov, 1, 20, "%d");
			ImGui::SliderFloat("smooth", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->smooth, 1, 50, "%.1f");
			ImGui::SliderFloat("kill delay", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->kill_delay, 1.f, 750.f, "%.1fms");
			ImGui::SliderFloat("switch delay", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->switch_target_delay, 1.f, 1500.f, "%.1fms");
			ImGui::SliderFloat("auto stop power", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->auto_stop_power, 0.0f, 1.f, "%.2f\%\%");
			ImGui::SliderInt("[RCS] pitch scale", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->rcs_x, 0.0f, 100, "%d\%\%");
			ImGui::SliderInt("[RCS] yaw scale", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->rcs_y, 0.0f, 100, "%d\%\%");
			static const char* combo_smooth_type[3] = { "non-control(time)", "control(linear)", "mixed" };
			ImGui::PopItemWidth();
			ImGui::PushItemWidth(260);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
			style.ItemSpacing = ImVec2(10, 10);
			ImGui::Combo("smooth type", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->smooth_type, combo_smooth_type, 3);
			static const char* combo_hit_box[5] = { "head", "neck", "body", "pelvis", "low chest" };
			ImGui::Combo("aim hitbox", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->curr_hitbox, combo_hit_box, 5);
			static const char* combo_target_list[3] = { "by crosshair", "by distance", "by lowest hp" };
			ImGui::Combo("target priority", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->target_method, combo_target_list, 3);
			static const char* fov_type_list[2] = { "static", "distance based" };
			ImGui::Combo("fov type", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->dist_type, fov_type_list, 2);
		}
		ImGui::EndChild();
		ImGui::SetCursorPosY(380);
		ImGui::SetCursorPosX(25);
		ImGui::BeginChild("##trigger_and_etc", ImVec2(770 / 3.f, 350.f), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::SetCursorPosX(35);
			ImGui::Text("Triggerbot");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 12);
			ImGui::ColorBar("##simple1", ImVec2(770 / 3.f, 3), ImColor(15, 146, 234), ImColor(194, 26, 61));			
			ImGui::Checkbox("triggerbot", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->trigger);	
			ImGui::SameLine(); key_bind("##trigger_key", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->trigger_key, menu_main_font_small);
			style.ItemSpacing = ImVec2(10, 13);
			ImGui::PushItemWidth(740 / 3.f - 20);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 12);
			ImGui::SliderInt("trigger chance", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->trigger_chance, 1, 100, "%d\%\%");
			style.ItemSpacing = ImVec2(10, 32);
			ImGui::SliderInt("trigger delay", &cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->trigger_delay, 1, 1000, "%d ms");
			//trigger hitbox combo
			ImGui::PopItemWidth();
		}
		ImGui::EndChild();

		ImGui::PopFont();
		ImGui::EndChild();
	}
	style.WindowPadding = ImVec2(0, 0);
	ImGui::EndChild();
		/*if(ImGui::BeginCombo("nearest hitboxes", preview.c_str()))
		{
			 std::string buffer = "";
			for (int i = 0; i < IM_ARRAYSIZE(combo_hit_box); i++)
			{			
				ImGui::Selectable(combo_hit_box[i], &g_cfg_system.general_aim_settings.hitbox_to_hit_list[i]);
				if (g_cfg_system.general_aim_settings.hitbox_to_hit_list[i])
				{
					if (i + 1 == IM_ARRAYSIZE(combo_hit_box)) {
						buffer += combo_hit_box[i];
					}
					else
					{
						buffer += combo_hit_box[i]; buffer += ",";
					}
				}
			}
			preview = buffer;
			ImGui::EndCombo();
		}/*/	
	//}
	//ImGui::EndChild();	
}
inline void menu::render_visual_tab()
{
	static const char* beam_list[7] = { "purple laser", "glow", "laser","fat","led glow","bubble","physbeam" };
	ImGui::BeginChild("visuals tab", ImVec2(770, 740.f), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_ChildBg];
		static ImColor active = ImColor(55, 55, 55);
		static ImColor non_active = ImColor(22, 22, 22);
		static ImColor non_active_text = ImColor(120, 120, 120);
		style.Colors[ImGuiCol_Text] = non_active_text;
		style.Colors[ImGuiCol_ButtonHovered] = active;
		style.Colors[ImGuiCol_ButtonActive] = active;
		ImGui::PushFont(menu_main_font);
		ImGui::BeginChild("##surface_render_visuals", ImVec2(770, 710.f), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		{
			style.ItemSpacing = ImVec2(10, 10);
			style.PopupBorderSize = 0.2f;
			style.ChildBorderSize = 0.45f;
			style.Colors[ImGuiCol_Border] = ImColor(30, 30, 30);
			style.ItemInnerSpacing = ImVec2(7, 5);
			style.FramePadding = ImVec2(4, 2);
			ImGui::SetCursorPosX(35);
			ImGui::BeginChild("checkboxes", ImVec2(290.f, 490.f), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
			{
				ImGui::SetCursorPosX(35);
				ImGui::Text("checkboxes");
				ImGui::ColorBar("##simple", ImVec2(290.f, 3), ImColor(15, 146, 234), ImColor(194, 26, 61));
				ImGui::Checkbox("aim fov", &cfg_vars.visuals.draw_aim_fov);
				ImGui::Checkbox("show aim target", &cfg_vars.visuals.show_aim_target); ImGui::SameLine(250.f - 39.f - 9.f); color_picker(&cfg_vars.visuals.aim_light_color, "aim target color", "##aim_light_color");
				ImGui::Checkbox("weapon spread", &cfg_vars.visuals.draw_spread_val); ImGui::SameLine(250.f - 39.f - 9.f); color_picker(&cfg_vars.visuals.spread_color, "spread color esp", "##spread_picker_col");
				ImGui::Checkbox("force crosshair", &cfg_vars.visuals.force_crosshair);
				ImGui::Checkbox("remove scope", &cfg_vars.visuals.remove_scope);
				ImGui::Checkbox("thirdperson", &cfg_vars.misc.thirdperson); ImGui::SameLine(); key_bind("##thirdperson_changer", &cfg_vars.misc.thirdperson_key, menu_main_font_small);
				ImGui::Checkbox("kill effect", &cfg_vars.visuals.kill_effect);
				ImGui::Checkbox("wireframe smoke", &cfg_vars.visuals.smoke_wireframe);
				ImGui::Checkbox("remove smoke", &cfg_vars.visuals.remove_smoke);
				ImGui::Checkbox("hit ring", &cfg_vars.visuals.hit_ring);
				ImGui::Checkbox("remove 3d sky", &cfg_vars.visuals.remove_3d_sky);
				ImGui::Checkbox("remove_shadows", &cfg_vars.visuals.remove_shadows);
				ImGui::EndChild();
			}
			ImGui::SameLine();
			ImGui::BeginChild("sliders", ImVec2(400.f, 500.f), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
			{
				ImGui::SetCursorPosX(35);
				ImGui::Text("sliders");
				ImGui::ColorBar("####simple2", ImVec2(400.f, 3), ImColor(15, 146, 234), ImColor(194, 26, 61));
				ImGui::Checkbox("bullet tracers", &cfg_vars.visuals.draw_bullet_tracers);  ImGui::SameLine(400.f - 39.f - 9.f); color_picker(&cfg_vars.visuals.bullet_tracers_col, "bullet color", "##bullet_tracers_color");
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
				ImGui::Text("[color]POST PROCESSING"); ImGui::SameLine(400.f - 39.f - 9.f);
				color_picker(&cfg_vars.visuals.post_effect_col, "post effect", "##post_effect");
				style.ItemSpacing = ImVec2(10, 10);
				ImGui::SliderInt("bright", &cfg_vars.visuals.bright, 0, 2, "%d");
				style.ItemSpacing = ImVec2(10, 30);
				ImGui::SliderFloat("thirdperson distance", &cfg_vars.misc.third_person_distance, 0, 350.f, "%.0f");
				ImGui::SliderFloat("alpha reducing", &cfg_vars.visuals.alpha_reducing, 0.f, 100.f, "%.0f\%\%");
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
				style.ItemSpacing = ImVec2(10, 15);
				ImGui::Combo("tracer type", &cfg_vars.visuals.tracer_type, beam_list, 7);
				style.ItemSpacing = ImVec2(10, 30);
				ImGui::SliderFloat("bullet life", &cfg_vars.visuals.bullet_tracer_life, 0.1f, 10.f, "%.1f");
				ImGui::SliderFloat("bullet width", &cfg_vars.visuals.bullet_tracer_width, 0.1f, 10.f, "%.1f");
				ImGui::SliderFloat("bullet fade", &cfg_vars.visuals.bullet_tracer_fade, 0.1f, 10.f, "%.1f");			
				ImGui::EndChild();
			}			
			ImGui::EndChild();
		}		
		ImGui::PopFont();
		ImGui::EndChild();
	}
}
inline void menu::render_player_tab()
{
	ImGui::BeginChild("player tab", ImVec2(770, 740.f), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_ChildBg];
	static ImColor active = ImColor(55, 55, 55);
	static ImColor non_active = ImColor(22, 22, 22);
	static ImColor non_active_text = ImColor(120, 120, 120);
	style.Colors[ImGuiCol_Text] = non_active_text;
	style.Colors[ImGuiCol_ButtonHovered] = active;
	style.Colors[ImGuiCol_ButtonActive] = active;
	ImGui::PushFont(menu_main_font);
	static int curr_vis_tab = 0;
	ImGui::BeginChild("##player tab selector", ImVec2(770, 30.f), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
	{
		curr_vis_tab == 0 ? style.Colors[ImGuiCol_Button] = active : style.Colors[ImGuiCol_Button] = non_active;
		if (ImGui::Button("surface render", ImVec2(770.f/3.f,30.f)))
		{
			curr_vis_tab = 0;
		}ImGui::SameLine();
		curr_vis_tab == 1 ? style.Colors[ImGuiCol_Button] = active : style.Colors[ImGuiCol_Button] = non_active;
		if (ImGui::Button("effects", ImVec2(770.f / 3.f, 30.f)))
		{
			curr_vis_tab = 1;
		}ImGui::SameLine();
		curr_vis_tab == 2 ? style.Colors[ImGuiCol_Button] = active : style.Colors[ImGuiCol_Button] = non_active;
		if (ImGui::Button("chams", ImVec2(770.f / 3.f, 30.f)))
		{
			curr_vis_tab = 2;
		}		
	}
	ImGui::EndChild();
	if (curr_vis_tab == 0)
	{
		static const char* combo_box_type[5] = { "bounding", "corner", "outlined","deep", "3D" };
		static const char* combo_hp_pos[4] = { "left", "right", "top","bottom" };
		static const char* combo_name_pos[4] = { "top", "bottom", "","" };
		static const char* combo_sound_mat[5] = {"white", "crosshairs","red small", "numbers", "scope_line_blur" };
		ImGui::BeginChild("##surface_render_visuals", ImVec2(770, 710.f), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		style.ItemSpacing = ImVec2(10, 10);
		style.PopupBorderSize = 0.2f;
		style.ChildBorderSize = 0.45f;
		style.Colors[ImGuiCol_Border] = ImColor(30, 30, 30);
		style.ItemInnerSpacing = ImVec2(7, 5);
		style.FramePadding = ImVec2(4, 2);

		ImGui::SetCursorPosX(15);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
		ImGui::BeginChild("##general_players_sb", ImVec2(360.f, 360.f), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::SetCursorPosX(35);
			ImGui::Text("general");
			style.ItemSpacing = ImVec2(10, 5);
			ImGui::ColorBar("##simple3", ImVec2(360.f, 3), ImColor(15, 146, 234), ImColor(194, 26, 61));
			style.ItemSpacing = ImVec2(10, 10);
			ImGui::Checkbox("enabled", &cfg_vars.visuals.enabled);
			ImGui::Checkbox("team esp", &cfg_vars.visuals.team);
			ImGui::Checkbox("box", &cfg_vars.visuals.box);  ImGui::SameLine(360.f - 23.f - 19.f); color_picker(&cfg_vars.visuals.box_color, "box color esp", "##box_picker_col");
			ImGui::Checkbox("sound esp", &cfg_vars.visuals.sound_esp); ImGui::SameLine(360.f - 23.f - 19.f); color_picker(&cfg_vars.visuals.sound_esp_color, "sound color esp", "##sound_esp_color");
			ImGui::Checkbox("name", &cfg_vars.visuals.name); ImGui::SameLine(360.f - 23.f - 19.f); color_picker(&cfg_vars.visuals.name_esp_color, "name color esp", "##nameesp_color");
			ImGui::Checkbox("hp", &cfg_vars.visuals.hp); ImGui::SameLine(360.f - 23.f * 2.f - 19.f * 2.f); color_picker(&cfg_vars.visuals.hp_esp_background_col, "hp_esp_background_col", "##hp_esp_background_col"); ImGui::SameLine(360.f - 23.f  - 19.f ); color_picker(&cfg_vars.visuals.hp_esp_col, "hp_esp_col", "##hp_esp_col");
			ImGui::Checkbox("weapon", &cfg_vars.visuals.weapon_esp);
			ImGui::Checkbox("skeleton", &cfg_vars.visuals.skeleton);ImGui::SameLine(360.f - 23.f - 19.f);color_picker(&cfg_vars.visuals.skeleton_color, "skeleton col", "##skeleton_color");
			ImGui::Checkbox("armor", &cfg_vars.visuals.line);
			//ImGui::Checkbox("box", &cfg_vars.visuals.box); ImGui::SameLine(100); ImGui::PushItemWidth(300); ImGui::Combo("box type", &cfg_vars.visuals.box_type, combo_box_type, 5); ImGui::PopItemWidth(); ImGui::SameLine(770 - 23 - 19); color_picker(&cfg_vars.visuals.box_color, "box color esp", "##box_picker_col");			
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##general_players_sb_2", ImVec2(360.f, 500.f), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::SetCursorPosX(35);
			ImGui::Text("additional");
			style.ItemSpacing = ImVec2(10, 5);
			ImGui::ColorBar("##simple2", ImVec2(360.f, 3), ImColor(15, 146, 234), ImColor(194, 26, 61));
			style.ItemSpacing = ImVec2(10, 10);
			ImGui::Checkbox("[weapon] icon", &cfg_vars.visuals.weapon_esp_icons);
			ImGui::Checkbox("line", &cfg_vars.visuals.line); ImGui::SameLine(360.f - 23.f - 19.f); color_picker(&cfg_vars.visuals.line_esp_color, "line color esp", "##line_esp_color");			
			ImGui::Checkbox("ammo", &cfg_vars.visuals.line);
			ImGui::Checkbox("distance", &cfg_vars.visuals.draw_distance);
			ImGui::Checkbox("speed", &cfg_vars.visuals.line);			
			ImGui::PushItemWidth(230);
			ImGui::Combo("hp position", &cfg_vars.visuals.hp_position, combo_hp_pos, 4);			
			ImGui::Combo("sound esp mat", &cfg_vars.visuals.sound_esp_material, combo_sound_mat, 5);
			style.ItemSpacing = ImVec2(10, 15);
			ImGui::SliderFloat("sound esp life", &cfg_vars.visuals.fl_sound_esp_life, 0.1f, 5.f, "%.1f");
			style.ItemSpacing = ImVec2(10, 35);
			ImGui::SliderFloat("sound esp width", &cfg_vars.visuals.fl_sound_esp_width, 0.1f, 10.f, "%.1f");
			ImGui::SliderFloat("sound esp radius", &cfg_vars.visuals.fl_sound_esp_radius, 10.f, 150.f, "%.1f");
			ImGui::PopItemWidth();
			style.ItemSpacing = ImVec2(10, 10);
		}
		ImGui::EndChild();	
		ImGui::EndChild();
	}
	if (curr_vis_tab == 1)
	{		
		style.FrameBorderSize = 0.0f;
		static const char* beam_list[7] = { "laser", "glow", "third","fourth","5","6","7" };
		style.ItemSpacing = ImVec2(15, 15);
		style.PopupBorderSize = 0.2f;
		style.ChildBorderSize = 0.45f;
		style.Colors[ImGuiCol_Border] = ImColor(30, 30, 30);
		style.ItemInnerSpacing = ImVec2(7, 5);
		style.FramePadding = ImVec2(4, 2);
		ImGui::BeginChild("##effects_players", ImVec2(770, 740.f), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		ImGui::Checkbox("glow", &cfg_vars.visuals.glow);
		ImGui::SameLine(770.f - 35.f - 9.f);
		color_picker(&cfg_vars.visuals.glow_color, "glow color", "##glow color");
		slider_int("glow thikness", &cfg_vars.visuals.glow_thikness, 0, 100, 350);
		style.ItemSpacing = ImVec2(15, 35);
		ImGui::Checkbox("DLight", &cfg_vars.visuals.dlight);ImGui::SameLine(770.f - 35.f - 9.f);
		color_picker(&cfg_vars.visuals.dlight_col, "DLIGHT color", "##dlight_color");
		style.ItemSpacing = ImVec2(15, 15);
		ImGui::Checkbox("DLight rainbow", &cfg_vars.visuals.dlight_rainbow);
		slider_int("DLight radius", &cfg_vars.visuals.dlight_radius, 0, 600, 350);
		style.ItemSpacing = ImVec2(15, 35);
		slider_int("DLight pulse", &cfg_vars.visuals.dlight_type, 0, 5,350);	
		ImGui::Checkbox("trails", &cfg_vars.visuals.trails);
		style.ItemSpacing = ImVec2(15, 15);
		if (cfg_vars.visuals.trails)
		{
			ImGui::Checkbox("trails rainbow", &cfg_vars.visuals.rainbow_trails);
			ImGui::SameLine(770.f - 35.f - 9.f);
			color_picker(&cfg_vars.visuals.trails_col, " trails color", "##rails_col");
			ImGui::SliderFloat("trails lifetime", &cfg_vars.visuals.trails_life, .1f, 5.f, "%.1f");
			style.ItemSpacing = ImVec2(15, 15);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
			ImGui::Combo("trails type", &cfg_vars.visuals.trail_type, beam_list, 7);
		}
		ImGui::Checkbox("local trails", &cfg_vars.visuals.trails_on_rails);
		if (cfg_vars.visuals.trails_on_rails)
		{
			ImGui::Checkbox("local trails rainbow", &cfg_vars.visuals.rainbow_trails_on_rails);
			ImGui::SameLine(770.f - 35.f - 9.f);
			color_picker(&cfg_vars.visuals.local_trails_col, "local trails color", "##rails_on_trails_col");
			ImGui::SliderFloat("local trails lifetime", &cfg_vars.visuals.local_trails_life, .1f, 5.f, "%.1f");
			style.ItemSpacing = ImVec2(15, 35);
			ImGui::Combo(" local trails type", &cfg_vars.visuals.local_trail_type, beam_list, 7);
		}		
		ImGui::EndChild();
	}
	if (curr_vis_tab == 2)
	{
		style.FrameBorderSize = 0.0f;		
		style.ItemSpacing = ImVec2(15, 15);
		style.PopupBorderSize = 0.2f;
		style.ChildBorderSize = 0.45f;
		style.Colors[ImGuiCol_Border] = ImColor(30, 30, 30);
		style.ItemInnerSpacing = ImVec2(7, 5);
		style.FramePadding = ImVec2(4, 2);
		ImGui::BeginChild("##chams_players", ImVec2(770, 740.f), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		ImGui::Checkbox("chams", &cfg_vars.visuals.chams); ImGui::SameLine(770.f - 35.f - 9.f);color_picker(&cfg_vars.visuals.model_color, "chams visible color", "##chams_visible_color");
		ImGui::Checkbox("ignore z buffer", &cfg_vars.visuals.ignore_z); ImGui::SameLine(770.f - 35.f - 9.f);color_picker(&cfg_vars.visuals.chams_invisible, "chams [IN]visible color", "##chams_invisible_color");
		ImGui::Checkbox("arm chams", &cfg_vars.visuals.hands_chams); ImGui::SameLine(770.f - 35.f - 9.f); color_picker(&cfg_vars.visuals.hand_color, "chams hand color", "##chams_hand_color");
		ImGui::Checkbox("weapon chams", &cfg_vars.visuals.weapon_chams); ImGui::SameLine(770.f - 35.f - 9.f); color_picker(&cfg_vars.visuals.weapon_color, "weapon hand color", "##chams_weapon_color");
		ImGui::Checkbox("sleeves chams", &cfg_vars.visuals.sleeves_chams); ImGui::SameLine(770.f - 35.f - 9.f); color_picker(&cfg_vars.visuals.sleeves_c_color, "sleeves hand color", "##chams_sleeves_color");
		ImGui::Checkbox("backtrack chams", &cfg_vars.visuals.backtrack_chams);
		ImGui::EndChild();
	}
	//style.Colors[ImGuiCol_Button] = non_active;
	ImGui::PopFont();
	ImGui::EndChild();	
}
inline void menu::render_misc_tab()
{	
	ImGui::PushFont(menu_main_font);
	ImGui::BeginChild("main functions", ImVec2(770.f, 650.f), false);
	{
		ImGuiStyle& style = ImGui::GetStyle();
		static ImColor non_active_text = ImColor(120, 120, 120);
		style.Colors[ImGuiCol_Border] = ImColor(30, 30, 30);
		style.Colors[ImGuiCol_Text] = non_active_text;
		ImGui::BeginChild("##general misc settings", ImVec2(360.f, 650.f), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::SetCursorPosX(35);
			ImGui::Text("general");
			style.ItemSpacing = ImVec2(10, 5);
			ImGui::ColorBar("##simple4", ImVec2(360.f, 3), ImColor(15, 146, 234), ImColor(194, 26, 61));
			style.ItemSpacing = ImVec2(10, 10);
			ImGui::Checkbox("slow walk", &cfg_vars.misc.slow_walk);
			ImGui::Checkbox("fakelags", &cfg_vars.misc.fakelags);
			ImGui::Checkbox("extended backtrack", &cfg_vars.misc.extend_backtrack);
			ImGui::Checkbox("bunnyhop", &cfg_vars.misc.bunnyhope);
			ImGui::Checkbox("auto strafe", &cfg_vars.misc.auto_strafe);
			ImGui::Checkbox("moon walk", &cfg_vars.misc.moon_walk);
			ImGui::Checkbox("radar hack", &cfg_vars.misc.radar_hack);
			ImGui::Checkbox("hit sound", &cfg_vars.misc.hit_sound);
			ImGui::Checkbox("hit marker", &cfg_vars.misc.hit_marker);
			ImGui::Checkbox("kill_spam", &cfg_vars.misc.kill_spam);
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##additional misc settings", ImVec2(360.f, 650.f), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::SetCursorPosX(35);
			ImGui::Text("additional");
			style.ItemSpacing = ImVec2(10, 5);
			ImGui::ColorBar("##simple5", ImVec2(360.f, 3), ImColor(15, 146, 234), ImColor(194, 26, 61));
			style.ItemSpacing = ImVec2(10, 30);
			ImGui::SliderInt("[fakelags]factor", &cfg_vars.misc.fakelag_factor, 0, 12, "%d");
			ImGui::SliderFloat("slow walk power", &cfg_vars.misc.slow_walk_power, 0.f, 110.f, "%.1f");
			ImGui::SliderFloat("third person distance", &cfg_vars.misc.third_person_distance, 0.f, 300.f, "%.1f");
			ImGui::SliderInt("viewmodel fov", &cfg_vars.misc.viewmodel_fov, 1, 200, "%d");
			ImGui::SliderInt("viewmodel_offset_x", &cfg_vars.misc.viewmodel_offset_x, -15, 15, "%d");
			ImGui::SliderInt("viewmodel_offset_y", &cfg_vars.misc.viewmodel_offset_y, -15, 15, "%d");
			ImGui::SliderInt("viewmodel_offset_z", &cfg_vars.misc.viewmodel_offset_z, -15, 15, "%d");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			style.ItemSpacing = ImVec2(10, 15);
			static const char* hitsound_type[5] = { "WOT", "bring","roblox", "beem","blyat" };
			ImGui::Combo("hitsound type", &cfg_vars.misc.curr_hitsound, hitsound_type, 5);
			static const char* fakelag_type[3] = { "normal", "on peek", "pulsive" };
			ImGui::Combo("fakelag type", &cfg_vars.misc.fakelag_type, fakelag_type, 3);
			ImGui::EndChild();
		}
		ImGui::EndChild();
	}
	ImGui::PopFont();
}
inline void menu::render_cfg_tab()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.ItemSpacing = ImVec2(10, 10);
	style.FramePadding = ImVec2(5, 5);
	style.ItemInnerSpacing = ImVec2(5, 5);
	style.PopupBorderSize = 0.f;
	style.Colors[ImGuiCol_Border] = ImColor(0.1, 0.1f, 0.1f, 1.f);
	static char buffer[128];
	ImGui::PushFont(menu_main_font);
	ImGui::BeginChild("##config_tab", ImVec2(770, 740.f), false);
	ImGui::PushItemWidth(250.f);
	static int curr_item = 0;

	ImGui::SetCursorPosX(30);
	if(ImGui::BeginCombo("cfg list", cfg.current_cfg_name.c_str(), ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightLargest))
	{
		for (std::string config_path : cfg.cfg_list)
		{
			bool is_selected = (cfg.current_cfg_name == config_path); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(config_path.c_str(), is_selected))
				cfg.current_cfg_name = config_path;
				if (is_selected)
					ImGui::SetItemDefaultFocus();   
		}
		ImGui::EndCombo();
	}
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
	ImGui::SetCursorPosX(35);
	if (ImGui::InputText("cfg name", buffer, sizeof(buffer)))
	{
		cfg.buffer_create_cfg = std::string(buffer);
	}
	ImGui::SetCursorPosX(35);
	if (ImGui::Button("save", ImVec2(250.f, 30.f)))
	{
		cfg.Save(cfg.current_cfg_name);
		cfg.cfg_list = cfg.get_cfg_list();		
	}
	ImGui::SetCursorPosX(35);
	if (ImGui::Button("load", ImVec2(250.f, 30.f)))
	{
		cfg.Load(cfg.current_cfg_name);
		cfg.cfg_list = cfg.get_cfg_list();
	}
	ImGui::SetCursorPosX(35);
	if (ImGui::Button("create", ImVec2(255.f, 30.f)))
	{
		cfg.Create(cfg.buffer_create_cfg);
		cfg.cfg_list = cfg.get_cfg_list();		
	}			
	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopFont();
}
void menu::color()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	style.Colors[ImGuiCol_WindowBg] = ImColor(15, 15, 15, 255);
	style.Colors[ImGuiCol_ChildBg] = ImColor(10, 10, 10, 255);
	style.Colors[ImGuiCol_Border] = ImColor(15, 15, 15, 255);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.09f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 0.88f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.15f, 0.60f, 0.78f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.40f, 0.95f, 0.59f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.40f, 0.95f, 0.59f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);	
}
void menu::styled()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 1.f;
	style.FramePadding = ImVec2(4, 0);
	style.WindowPadding = ImVec2(0, 0);
	style.ItemSpacing = ImVec2(0, 0);
	style.ScrollbarSize = 8.f;
	style.ScrollbarRounding = 0.f;
	style.GrabMinSize = 9.f;
}
bool menu::ButtonColor(const char* name , ImVec2 size, Color c, bool lined)
{
	ImGuiStyle& style = ImGui::GetStyle();

	ImColor old_btn = style.Colors[ImGuiCol_Button];
	ImColor old_btn_hov = style.Colors[ImGuiCol_ButtonHovered];
	ImColor old_btn_act = style.Colors[ImGuiCol_ButtonActive];

	style.Colors[ImGuiCol_Button] = ImColor(c.r, c.g, c.b,c.a);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(c.r, c.g, c.b,c.a);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(c.r, c.g, c.b,c.a);
bool response = 	ImGui::Button(name, size);

	style.Colors[ImGuiCol_Button] = old_btn;
	style.Colors[ImGuiCol_ButtonHovered] = old_btn_hov;
	style.Colors[ImGuiCol_ButtonActive] = old_btn_act;
	return response;
}
bool menu::ButtonColor(int r, int g, int b)
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_Button] = ImColor(r, g, b);
    style.Colors[ImGuiCol_ButtonHovered] = ImColor(r, g, b);
    style.Colors[ImGuiCol_ButtonActive] = ImColor(r, g, b);
    return true;
}
void menu::child_color(const char* label, ImVec2 size, bool borders, Color col)
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImColor old_color = style.Colors[ImGuiCol_ChildBg];
	style.Colors[ImGuiCol_ChildBg] = ImColor(col.r, col.g, col.b, col.a);
	ImGui::BeginChild(label, size, borders, ImGuiCond_Always | ImGuiWindowFlags_NoResize);
	style.Colors[ImGuiCol_ChildBg] = old_color;
}
void menu::color_picker_proxy(Color* to_out, const char* name, bool alpha)
{
	float coder_autist[4] = { to_out->r / 255.f, to_out->g / 255.f, to_out->b / 255.f, to_out->a / 255.f };
	//ImGui::ColorPicker(name, &coder_autist, alpha);
	ImGui::ColorEdit4(name, coder_autist, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoLabel);
	to_out->r = coder_autist[0];
	to_out->g = coder_autist[1];
	to_out->b = coder_autist[2];
	to_out->a = coder_autist[3];
	to_out->r *= 255.f;
	to_out->g *= 255.f;
	to_out->b *= 255.f;
	to_out->a *= 255.f;
	if (to_out->a == INFINITE || to_out->a >= 255.f || to_out->a < 0)to_out->a = 255.f;
}
void menu::color_picker(Color* to_out, const char* popup_name, const char* vtn, bool alpha)
{
	/*if (ButtonColor(vtn, ImVec2(19, 8), Color(to_out->r / 255.f,to_out->g / 255.f, to_out->b / 255.f), true))
	{
		ImGui::OpenPopup(popup_name);
	}
	if (ImGui::BeginPopup(popup_name, ImGuiWindowFlags_NoMove))
	{
		ImGui::Text(popup_name);
		color_picker_proxy(to_out, vtn, alpha);		
		ImGui::EndPopup();
	}*/
	color_picker_proxy(to_out, vtn, alpha);
}
void menu::slider_int(const char* label, int* item, int min, int max, float width)
{
	ImGui::PushItemWidth(width);
	ImGui::SliderInt(label, item, min, max, "%d");
	ImGui::PopItemWidth();
}
void menu::key_bind(const char* label,int* k, ImFont* font)
{
	ImGui::PushFont(font);
	ImGui::key_binder(label, k, ImVec2(75, 25));
	ImGui::PopFont();
}

