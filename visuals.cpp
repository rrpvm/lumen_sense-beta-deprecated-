#include "visuals.hpp"
#include "../LumenSense/utils/utils.h"
#include "../LumenSense/sdk/sdk.hpp"
#include "../LumenSense/sdk/csgo.h"
#include <backend\cfg_system.hpp>
#include <g_event.h>


static auto get_char_by_name = [](std::string name) -> const char*
{
	//в пизду индексы
	if (name == "glock")return "C";
	else if (name == "p250")return "A";
	else if (name == "ak47")return "B";
	else if (name == "MP9")return "D";
	else if (name == "aug")return "E";
	else if (name == "deagle")return "F";
	else if (name == "flashbang")return "G";
	else if (name == "hegrenade")return "H";
	else if (name == "g3sg1")return "I";
	else if (name == "knife")return "J";
	else if (name == "nova")return "K";
	else if (name == "mac10")return "L";
	else if (name == "p90")return "M";
	else if (name == "ssg08")return "N";
	else if (name == "scar20")return "O";
	else if (name == "smokegrenade")return "P";
	else if (name == "ump45")return "Q";
	else if (name == "awp")return "R";
	else if (name == "beretts")return "S";
	else if (name == "famas")return "T";
	else if (name == "galil-ar")return "V";
	else if (name == "m4a1")return "W";
	return "";
};
void visuals::on_paint_traverse()
{
	for (int i = 1; i < 64; ++i)
	{
		c_base_player* player = static_cast<c_base_player*>(g_entity_list->GetClientEntity(i));
		if (!player || player->hp() <= 0 || player == csgo_data.local || csgo_data.alpha_data[i] == 0.0f)continue;
		if (!cfg_vars.visuals.team && !player->is_enemy())continue;
		if (cfg_vars.visuals.enabled)g_visuals.get_perfomance(player, i);//if cfg_vars.visuals.enabled
		if (cfg_vars.visuals.box && cfg_vars.visuals.enabled)draw_box(player, i);
		if (cfg_vars.visuals.hp && cfg_vars.visuals.enabled)draw_hp(player, i);
		if (cfg_vars.visuals.line && cfg_vars.visuals.enabled)draw_line(player, i);
		if (cfg_vars.visuals.name && cfg_vars.visuals.enabled)draw_name(player, i);
		if (cfg_vars.visuals.trails && cfg_vars.visuals.enabled)trails(player, i);
		if (cfg_vars.visuals.skeleton && cfg_vars.visuals.enabled)bone_esp(player, i);
		if (cfg_vars.visuals.enabled && cfg_vars.visuals.weapon_esp)draw_weapons(player, i);
		if (cfg_vars.visuals.enabled)g_visuals.draw_flags(player, i);
		if (cfg_vars.visuals.draw_distance)g_visuals.draw_distance(player, i);
	}
	if (cfg_vars.visuals.remove_scope)draw_scope_lines();
	if (cfg_vars.visuals.trails_on_rails && cfg_vars.visuals.enabled)local_trails();
	if (cfg_vars.visuals.draw_aim_fov)draw_aim_fov(0);
	if (cfg_vars.visuals.draw_spread_val)draw_spread();
	reduce_flash();
}
void visuals::get_perfomance(c_base_player* pl, int i)
{

	RECT bbox = pl->get_bbox();
	visuals_data[i].pos2D = Vector(bbox.left, bbox.top);
	visuals_data[i].head2D = Vector(bbox.right, bbox.bottom);	
	if (bbox.right == 0 || bbox.bottom == 0)
	{
		visuals_data[i].can_see_through_view_matrix = false;
		return;
	}
	visuals_data[i].can_see_through_view_matrix = true;
	visuals_data[i].height = (visuals_data[i].pos2D.y - visuals_data[i].head2D.y);
	//visuals_data[i].width = (visuals_data[i].head2D.x - visuals_data[i].pos2D.x);
	visuals_data[i].width = (visuals_data[i].head2D.x - visuals_data[i].pos2D.x);
}
void visuals::draw_box(c_base_player* pl,int i)
{	
	if (!visuals_data[i].can_see_through_view_matrix)return;
	int y = visuals_data[i].head2D.y;
	float width = visuals_data[i].width;
	int x2 = visuals_data[i].pos2D.x;	
	if (cfg_vars.visuals.box_background && cfg_vars.visuals.box_type != 4)
	{
		g_surface->draw_filled_rect(x2, y, visuals_data[i].width, visuals_data[i].height, Color(cfg_vars.visuals.box_background_color.a, cfg_vars.visuals.box_background_color.r, cfg_vars.visuals.box_background_color.g, cfg_vars.visuals.box_background_color.b));
	}
	g_surface->DrawSetColor(cfg_vars.visuals.box_color.r, cfg_vars.visuals.box_color.g, cfg_vars.visuals.box_color.b, csgo_data.box_color_alpha[i]);
	switch (cfg_vars.visuals.box_type)
	{
	case 0:
	{		
		g_surface->draw_outlined_rect(x2, y,  visuals_data[i].width, visuals_data[i].height, 2, Color(csgo_data.box_color_alpha[i], cfg_vars.visuals.box_color.r, cfg_vars.visuals.box_color.g, cfg_vars.visuals.box_color.b));
		break;
	}
	case 1: {
		g_surface->DrawOutlinedRect(x2, y, x2 + visuals_data[i].width / 3.f, y + 1);//top left
		g_surface->DrawOutlinedRect(x2 + visuals_data[i].width * 2.f / 3.f, y, x2 + visuals_data[i].width, y + 1);//top right
		g_surface->DrawOutlinedRect(x2, y + visuals_data[i].height, x2 + visuals_data[i].width / 3.f, y + visuals_data[i].height + 1);//bottom left
		g_surface->DrawOutlinedRect(x2 + visuals_data[i].width * 2.f / 3.f, y + visuals_data[i].height, x2 + visuals_data[i].width, y + visuals_data[i].height + 1);// bottom right
		g_surface->DrawOutlinedRect(x2, y, x2 + 1, y + visuals_data[i].height / 3.f);//y axis left
		g_surface->DrawOutlinedRect(x2, y + visuals_data[i].height * 2.f / 3.f, x2 + 1, y + visuals_data[i].height);//y axis left
		g_surface->DrawOutlinedRect(x2 + visuals_data[i].width, y, x2 + 1 + visuals_data[i].width, y + visuals_data[i].height / 3.f);//y axis right
		g_surface->DrawOutlinedRect(x2 + visuals_data[i].width, y + visuals_data[i].height * 2.f / 3.f, x2 + 1 + visuals_data[i].width, y + visuals_data[i].height);//y axis right
		break;
	}
	case 2:
	{
		g_surface->DrawOutlinedRect(x2, y, x2 + visuals_data[i].width, y + visuals_data[i].height);
		g_surface->DrawOutlinedRect(x2 - 2, y - 2, x2 + visuals_data[i].width + 2, y + visuals_data[i].height + 2);//+2/-2					
		break;
	}
	case 3: {
		g_surface->DrawOutlinedRect(x2, y, x2 + visuals_data[i].width / 3.f, y + 1);//top left	
		g_surface->DrawOutlinedRect(x2 + visuals_data[i].width * 2.f / 3.f, y, x2 + visuals_data[i].width, y + 1);//top right
		g_surface->DrawOutlinedRect(x2, y + visuals_data[i].height, x2 + visuals_data[i].width / 3.f, y + visuals_data[i].height + 1);//bottom left
		g_surface->DrawOutlinedRect(x2 + visuals_data[i].width * 2.f / 3.f, y + visuals_data[i].height, x2 + visuals_data[i].width, y + visuals_data[i].height + 1);// bottom right
		g_surface->DrawOutlinedRect(x2, y, x2 + 1, y + visuals_data[i].height);//y axis left
		g_surface->DrawOutlinedRect(x2 + visuals_data[i].width, y, x2 + 1 + visuals_data[i].width, y + visuals_data[i].height);//y axis left
		break;
	}
	case 4:
	{
		if (!pl)return;
		ICollideable* collide = pl->GetCollideable();
		Vector min = collide->OBBMins();
		Vector max = collide->OBBMaxs();
		Vector corners[8] =
		{
			Vector(min.x,min.y,min.z),
			Vector(min.x,max.y,min.z),
			Vector(max.x,max.y,min.z),
			Vector(max.x,min.y,min.z),
			Vector(min.x,min.y,max.z),
			Vector(min.x,max.y,max.z),
			Vector(max.x,max.y,max.z),
			Vector(max.x,min.y,max.z)
		};
		float ang = pl->GetEyeAngles().y;
		for (int z = 0; z < 8; z++)
			corners[z].Rotate2D(ang);
		Vector _corners[8];

		for (int j = 0; j <= 7; j++)
			if (!utils::get().WorldToScreen(pl->get_origin() + corners[j], _corners[j]))return;
		for (int j = 1; j <= 4; j++)
		{
			g_surface->DrawLine((int)(_corners[j - 1].x), (int)(_corners[j - 1].y), (int)(_corners[j % 4].x), (int)(_corners[j % 4].y));
			g_surface->DrawLine((int)(_corners[j - 1].x), (int)(_corners[j - 1].y), (int)(_corners[j + 3].x), (int)(_corners[j + 3].y));
			g_surface->DrawLine((int)(_corners[j + 3].x), (int)(_corners[j + 3].y), (int)(_corners[j % 4 + 4].x), (int)(_corners[j % 4 + 4].y));
		}
		break;
	}
	default:
		break;
	}
}
void visuals::draw_line(c_base_player* pl,int i)
{
	if (!visuals_data[i].can_see_through_view_matrix)return;
	g_surface->DrawSetColor(cfg_vars.visuals.line_esp_color.r, cfg_vars.visuals.line_esp_color.g, cfg_vars.visuals.line_esp_color.b, csgo_data.alpha_data[i]);
	g_surface->DrawLine(csgo_data.screen_center.x , csgo_data.screen_center.y * 2.f -15, visuals_data[i].head2D.x - visuals_data[i].width/2.f, visuals_data[i].head2D.y);
}
#define min_dst 200
void visuals::draw_hp(c_base_player* pl,int i)
{
	if (!visuals_data[i].can_see_through_view_matrix)return;
	float scale = pl->hp() / 100.f;
	if (scale > 1.f)scale = 1.0f;
	else if (scale < 0.f)scale = 0.0f;
	if (!g_event_handler.hit_data[i].empty()&&  g_event_handler.hit_data[i].back().hit_time + 0.4f > g_global_vars->curtime)
	{		
		float percent = (g_global_vars->curtime - g_event_handler.hit_data[i].back().hit_time) / .4f;
		if (percent > 1.0f)percent = 1.f;
		if (percent < .0f)percent = 0.f;
		float _n_damage = (g_event_handler.hit_data[i].back().dmg / 100.f * (percent * 100.f));
		float rest = g_event_handler.hit_data[i].back().dmg - _n_damage;
		scale = (pl->hp() + rest) / 100.f;
		if (scale > 1.f)scale = 1.f;
	}
	float distance = csgo_data.local->get_origin().DistTo(pl->get_origin());
	float margin =  4 - ( distance / min_dst / 4.f);
	margin *= 1.1f;
	if (margin < 0)margin = fabs(margin);
	if (margin < 1.5f)margin = 1.5f;
	if (margin > 4.5f)margin = 4.5f;
	int wide; int tall;
	g_surface->get_text_size(verdana, std::to_string(pl->hp()), wide, tall);
	Color hp_background = Color(csgo_data.alpha_data[i], cfg_vars.visuals.hp_esp_background_col.r, cfg_vars.visuals.hp_esp_background_col.g, cfg_vars.visuals.hp_esp_background_col.b);
	Color hp_col = Color(csgo_data.alpha_data[i], cfg_vars.visuals.hp_esp_col.r, cfg_vars.visuals.hp_esp_col.g, cfg_vars.visuals.hp_esp_col.b);
	switch (cfg_vars.visuals.hp_position)
	{	
	case 0:				
		g_surface->draw_filled_rect(visuals_data[i].pos2D.x - margin, visuals_data[i].head2D.y, margin, visuals_data[i].height, hp_background);
		g_surface->draw_filled_rect(visuals_data[i].pos2D.x - margin, visuals_data[i].head2D.y + (visuals_data[i].height * (1-scale)), margin, visuals_data[i].height * scale, hp_col);
		g_surface->draw_rect(visuals_data[i].pos2D.x - margin - 1.f, visuals_data[i].head2D.y, margin + 2.f, visuals_data[i].height, Color(csgo_data.alpha_data[i], 10, 10, 10));
		g_surface->DrawT(visuals_data[i].pos2D.x - margin - wide/2.f, visuals_data[i].head2D.y, Color(csgo_data.alpha_data[i], 255.f, 255.f, 255.f), verdana, true, std::to_string(pl->hp()).c_str());
		break;	
	case 1:
		g_surface->draw_filled_rect(visuals_data[i].head2D.x + 1.f, visuals_data[i].head2D.y, margin, visuals_data[i].height, hp_background);
		g_surface->draw_filled_rect(visuals_data[i].head2D.x + 1.f, visuals_data[i].head2D.y + (visuals_data[i].height * (1 - scale)), margin, visuals_data[i].height * scale, hp_col);
		g_surface->draw_rect(visuals_data[i].head2D.x + 0.5f, visuals_data[i].head2D.y, margin + 2.5f, visuals_data[i].height, Color(csgo_data.alpha_data[i], 10, 10, 10));
		g_surface->DrawT(visuals_data[i].head2D.x +margin+  wide / 2.f, visuals_data[i].head2D.y, Color(csgo_data.alpha_data[i], 255.f, 255.f, 255.f), verdana, true, std::to_string(pl->hp()).c_str());
		break;
	case 2:
		g_surface->draw_filled_rect(visuals_data[i].pos2D.x, visuals_data[i].head2D.y, visuals_data[i].width, margin, hp_background);
		g_surface->draw_filled_rect(visuals_data[i].pos2D.x, visuals_data[i].head2D.y, visuals_data[i].width * scale, margin, hp_col);
		g_surface->draw_rect(visuals_data[i].pos2D.x, visuals_data[i].head2D.y, visuals_data[i].width, margin, Color(csgo_data.alpha_data[i], 10, 10, 10));
		g_surface->DrawT(visuals_data[i].pos2D.x, visuals_data[i].head2D.y - margin - tall/2.f, Color(csgo_data.alpha_data[i], 255.f, 255.f, 255.f), verdana, true, std::to_string(pl->hp()).c_str());
		//g_surface->draw_rect(visuals_data[i].pos2D.x - visuals_data[i].width / 2.f, visuals_data[i].head2D.y - margin * 2.f, visuals_data[i].width, margin, Color(csgo_data.alpha_data[i], 0, 0, 0));//1.5, 1.5 + 0.5f = 0.2f
		//g_surface->draw_filled_rect(visuals_data[i].pos2D.x - visuals_data[i].width / 2.f, visuals_data[i].head2D.y - margin * 1.5f, visuals_data[i].width, margin, Color(csgo_data.alpha_data[i], 255, 0, 0));//1.5, 1.5 + 0.5f = 0.2f
		//g_surface->draw_filled_rect(visuals_data[i].pos2D.x - visuals_data[i].width / 2.f, visuals_data[i].head2D.y - margin * 1.5f, visuals_data[i].width * scale, margin, Color(csgo_data.alpha_data[i], 0, 255, 0));//1.5, 1.5 + 0.5f = 0.2f
		break;
	case 3:
		g_surface->draw_filled_rect(visuals_data[i].pos2D.x, visuals_data[i].pos2D.y, visuals_data[i].width, margin, hp_background);
		g_surface->draw_filled_rect(visuals_data[i].pos2D.x, visuals_data[i].pos2D.y, visuals_data[i].width * scale, margin, hp_col);
		g_surface->draw_rect(visuals_data[i].pos2D.x, visuals_data[i].pos2D.y, visuals_data[i].width, margin, Color(csgo_data.alpha_data[i], 10, 10, 10));
		g_surface->DrawT(visuals_data[i].pos2D.x, visuals_data[i].pos2D.y + margin + tall / 2.f, Color(csgo_data.alpha_data[i], 255.f, 255.f, 255.f), verdana, true, std::to_string(pl->hp()).c_str());
		break;
	}
}
void visuals::draw_name(c_base_player* pl,int i)
{
	if (!visuals_data[i].can_see_through_view_matrix)return;
	player_info_t info;
	char player_name[128];
	g_engine->GetPlayerInfo(i, &info);
	memcpy_s(player_name, 128, info.szName, 128);
	const size_t name_size = strlen(info.szName);
	if (name_size > 13)
	{
		player_name[14] = '.';
		player_name[15] = '.';
		player_name[16] = '.';
		player_name[17] = '\0';
	}
	int wide,  tall;
	g_surface->get_text_size(verdana, player_name, wide, tall);
	switch (cfg_vars.visuals.name_position)
	{
	case 0:
	{
		g_surface->DrawT(visuals_data[i].head2D.x - visuals_data[i].width/2.f, visuals_data[i].head2D.y - tall, Color(csgo_data.name_color_alpha[i], cfg_vars.visuals.name_esp_color.r, cfg_vars.visuals.name_esp_color.g, cfg_vars.visuals.name_esp_color.b), verdana, true, player_name);
		break;
	}
	case 1:
	{
		g_surface->DrawT(visuals_data[i].head2D.x - visuals_data[i].width / 2.f, visuals_data[i].pos2D.y + tall/4.f, Color(csgo_data.name_color_alpha[i], cfg_vars.visuals.name_esp_color.r, cfg_vars.visuals.name_esp_color.g, cfg_vars.visuals.name_esp_color.b), verdana, true, player_name);
		break;
	}
	default:
		break;
	}
}
void visuals::draw_flags(c_base_player* pl, int i)
{
	/*if (!visuals_data[i].can_see_through_view_matrix)return;
	bool left_side = true;//where will draw text
	bool right_side = false;
	if (pl->is_scoped())
	{
		g_surface->DrawT(visuals_data[i].head2D.x + visuals_data[i].width/2.f, visuals_data[i].head2D.y, Color(csgo_data.alpha_data[i], 255.f,255.f,255.f), verdana, true, "scoped"); //or draw icon
		left_side = false;
		right_side = true;
	}*/


}
void visuals::draw_weapons(c_base_player* pl, int i)
{
	if (!visuals_data[i].can_see_through_view_matrix)return;
	c_base_combat_weapon* weapon = pl->get_active_weapon();
	if (!weapon)return;
	CCSWeaponInfo* data = weapon->GetCSWeaponData();
	if (!data)return;

	int wide, tall;
	std::string buffer = data->szWeaponName;
	std::string reciever = "";
	for (int i = 0; i < buffer.size(); i++)//weapon_
	{
		if (i >= 7)
		{
			reciever += buffer[i];
		}
	}
	if (!cfg_vars.visuals.weapon_esp_icons)
	{
		g_surface->get_text_size(verdana, reciever.c_str(), wide, tall);
		g_surface->DrawT(visuals_data[i].pos2D.x + visuals_data[i].width / 2.f, visuals_data[i].pos2D.y + tall / 2.f, Color(255, 255, 255, 255), verdana, true, reciever.c_str());
	}
	else {
		const char* f = get_char_by_name(reciever);
		g_surface->get_text_size(verdana, f, wide, tall);
		g_surface->DrawT(visuals_data[i].pos2D.x + visuals_data[i].width / 2.f, visuals_data[i].pos2D.y + tall / 2.f, Color(255, 255, 255, 255), icons, true, f);
	}
}
void visuals::draw_scope_lines()
{
	if (cfg_vars.visuals.remove_scope)
	{
		if (csgo_data.local)
		{
			if (csgo_data.local->is_scoped()) {					
				g_surface->Line(csgo_data.screen_center.x, 0, csgo_data.screen_center.x, csgo_data.screen_center.y * 2.f, Color(255, 0, 0, 0));//horizontal
				g_surface->Line(0,csgo_data.screen_center.y, csgo_data.screen_center.x * 2.f, csgo_data.screen_center.y, Color(255, 0, 0, 0));
			}
		}
	}
}
void visuals::do_dlight(c_base_player* pl,int i)
{
	static float flRainbow = 0.0f;
	flRainbow += g_global_vars->frametime / 40.f;
	if (flRainbow >= 1.0f)flRainbow = 0.0f;
	if (flRainbow < 0.f)flRainbow = 0.0f;

	dlight_t* pDlight = g_effects->CL_AllocDlight(i);
	pDlight->origin = pl->get_origin() + Vector(0,0,5);
	pDlight->radius = cfg_vars.visuals.dlight_radius;

	if (cfg_vars.visuals.dlight_rainbow)
	{
		Color hs;
		hs.ColorConvertHSVtoRGB(flRainbow, 1.f, 1.f, hs.r, hs.g, hs.b);
		hs.r = hs.r * 255.0f;
		hs.g = hs.g * 255.0f;
		hs.b = hs.b * 255.0f;
		pDlight->color.r = hs.r;
		pDlight->color.g = hs.g;
		pDlight->color.b = hs.b;
	}
	else {
		pDlight->color.r = cfg_vars.visuals.dlight_col.r;
		pDlight->color.g = cfg_vars.visuals.dlight_col.g;
		pDlight->color.b = cfg_vars.visuals.dlight_col.b;
	}
	pDlight->die = g_global_vars->curtime + 0.1f;
	pDlight->color.exponent = 5;
	pDlight->style = cfg_vars.visuals.dlight_type;
	pDlight->decay = 45.f;
	pDlight->m_InnerAngle = 180 - (rand() % 360);
	pDlight->m_OuterAngle = 0;
	pDlight->key = i;
}
void visuals::trails(c_base_player* pl,int i)
{
	if (!visuals_data[i].can_see_through_view_matrix)return;
	Vector origin = pl->get_origin();
	static float fl_rainbow = 0.0f;
	fl_rainbow += g_global_vars->absoluteframetime / (float)g_engine->GetMaxClients();
	if (fl_rainbow > 1.0f)fl_rainbow = 0.0f;
	if (pl->get_velocity().Length2D() > 0)
	{
		Color c;
		c.ColorConvertHSVtoRGB(fl_rainbow, 1.f, 1.f, c.r, c.g, c.b);
		g_render_beam->draw_beam_custom(origin, Vector(origin.x, origin.y + 8.f, origin.z + 1), 2, (cfg_vars.visuals.trails_life + 0.1f), 8.f, 8.f, .2f, cfg_vars.visuals.trail_type, cfg_vars.visuals.rainbow_trails ? c * 255.f : cfg_vars.visuals.trails_col);
	}

}
void visuals::local_trails()
{	
	static Vector old = csgo_data.local->get_origin();	
	Vector origin = csgo_data.local->get_origin();
	static float fl_rainbow = 0.0f;
	fl_rainbow += g_global_vars->absoluteframetime;
	if (fl_rainbow > 1.0f)fl_rainbow = 0.0f;
	if (csgo_data.local->get_velocity().Length2D() > 0)
	{
		Color c;
		c.ColorConvertHSVtoRGB(fl_rainbow, 1.f, 1.f, c.r, c.g, c.b);		
		g_render_beam->draw_beam_custom(origin, Vector(origin.x + (origin.x - old.x), origin.y + 8.f, origin.z + 1), 4, (cfg_vars.visuals.local_trails_life + 0.1f),8.f, 8.f, .2f,cfg_vars.visuals.local_trail_type, cfg_vars.visuals.rainbow_trails_on_rails ? c * 255.f : cfg_vars.visuals.local_trails_col);
		if (old != origin)
		{
			old = origin;
		}
	}
}
void visuals::glow(int _i)
{
	for (auto i = 0; i < glow_manager->m_GlowObjectDefinitions.Count(); i++) {
		GlowObjectDefinition_t& glowObject = glow_manager->m_GlowObjectDefinitions[i];
		c_base_player* entity = reinterpret_cast<c_base_player*>(glowObject.m_pEntity);

		if (glowObject.IsUnused())
			continue;

		if (!entity || entity->dormant())
			continue;

		auto class_id = entity->GetClientClass()->m_ClassID;
		Color buffer = Color(csgo_data.alpha_data[_i]/255.f,255,255,255);
		cfg_vars.visuals.glow_color.a = csgo_data.alpha_data[_i] / 255.f;
		switch (class_id) {
		case 40://player
		{			
			bool is_enemy = entity->is_enemy();
			buffer = is_enemy ? cfg_vars.visuals.glow_color : Color(csgo_data.alpha_data[_i]/255.f,0, 255, 0);
			if (!cfg_vars.visuals.team && !is_enemy)continue;
			break;
		}
		default:
			continue;
			break;
		}
		/*case ClassId_CChicken:
			if (!g_Options.glow_chickens)
				continue;
			entity->m_bShouldGlow() = true;
			color = g_Options.color_glow_chickens;
			break;
		case ClassId_CBaseAnimating:
			if (!g_Options.glow_defuse_kits)
				continue;
			color = g_Options.color_glow_defuse;
			break;
		case ClassId_CPlantedC4:
			if (!g_Options.glow_planted_c4)
				continue;
			color = g_Options.color_glow_planted_c4;
			break;
		default:
		{
			if (entity->IsWeapon()) {
				if (!g_Options.glow_weapons)
					continue;
				color = g_Options.color_glow_weapons;
			}
		}*/
		//}
		if (entity->is_enemy()) {
			glowObject.m_flRed = buffer.r / 255.0f;
			glowObject.m_flGreen = buffer.g / 255.0f;
			glowObject.m_flBlue = buffer.b / 255.0f;
			glowObject.m_flAlpha = cfg_vars.visuals.glow_thikness / 100.f;
			glowObject.m_bFullBloomRender = false;
			glowObject.m_bRenderWhenOccluded = true;
			glowObject.m_bRenderWhenUnoccluded = false;
		}
	}
}
void visuals::draw_aim_fov(int i)
{
	g_surface->DrawSetColor(200, 255, 255, 255);
	float ratio = csgo_data.screen_center.x / csgo_data.screen_center.y;
	float screenFov = atanf((ratio) * (0.75f) * tan((69.f * 0.5f) / 180.f * 3.14f));//60 = fov of player(viewmodel_fov)
	float radiusFOV;
	if (cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->p_silent && csgo_data.local->get_fired_shots() < 1)//psilent
	{
		radiusFOV = tanf((cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->p_silent_fov / 3.f) / 180.f * 3.14f) / tanf(screenFov) * csgo_data.screen_center.x;
	}
	else {
		radiusFOV = tanf((cfg_vars.legitbot.wpn_set_table[csgo_data.active_wpn_type]->fov / 3.f) / 180.f * 3.14f) / tanf(screenFov) * csgo_data.screen_center.x;
	}	
	if (radiusFOV >= 1)
	{
		g_surface->DrawOutlinedCircle(csgo_data.screen_center.x, csgo_data.screen_center.y, radiusFOV * 0.65f, 150);
	}
}
void visuals::bone_esp(c_base_player* pl,int i)
{
	if (!visuals_data[i].can_see_through_view_matrix || !pl)return;
	
	Vector Hitbox[19];
	Vector Hitboxw2s[19];
	for (int hitbox = 0; hitbox < 19; hitbox++)
	{
		Hitbox[hitbox] = pl->get_hitbox_position(hitbox);
		if(!utils::get().WorldToScreen(Hitbox[hitbox], Hitboxw2s[hitbox]))return;
	}
	/*or debug overlay ->d3dline without w2s*/
	//spine
	g_surface->Line(Hitboxw2s[HITBOX_HEAD].x, Hitboxw2s[HITBOX_HEAD].y, Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, cfg_vars.visuals.skeleton_color );
	g_surface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, cfg_vars.visuals.skeleton_color );
	g_surface->Line(Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, cfg_vars.visuals.skeleton_color);
	g_surface->Line(Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, cfg_vars.visuals.skeleton_color);
	g_surface->Line(Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, cfg_vars.visuals.skeleton_color);
	g_surface->Line(Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, cfg_vars.visuals.skeleton_color);

	//right leg
	g_surface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, cfg_vars.visuals.skeleton_color);
	g_surface->Line(Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, cfg_vars.visuals.skeleton_color );
	g_surface->Line(Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, Hitboxw2s[HITBOX_RIGHT_FOOT].x, Hitboxw2s[HITBOX_RIGHT_FOOT].y, cfg_vars.visuals.skeleton_color );

	//right arm
	g_surface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, cfg_vars.visuals.skeleton_color );
	g_surface->Line(Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, cfg_vars.visuals.skeleton_color );
	g_surface->Line(Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, Hitboxw2s[HITBOX_RIGHT_HAND].x, Hitboxw2s[HITBOX_RIGHT_HAND].y, cfg_vars.visuals.skeleton_color );

	//left leg
	g_surface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, cfg_vars.visuals.skeleton_color );
	g_surface->Line(Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, cfg_vars.visuals.skeleton_color );
	g_surface->Line(Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, Hitboxw2s[HITBOX_LEFT_FOOT].x, Hitboxw2s[HITBOX_LEFT_FOOT].y, cfg_vars.visuals.skeleton_color );

	//left arm
	g_surface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, cfg_vars.visuals.skeleton_color );
	g_surface->Line(Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, cfg_vars.visuals.skeleton_color);
	g_surface->Line(Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, Hitboxw2s[HITBOX_LEFT_HAND].x, Hitboxw2s[HITBOX_LEFT_HAND].y, cfg_vars.visuals.skeleton_color );
}
void visuals::model_changer(ClientFrameStage_t stage)
{
	if (stage != FRAME_RENDER_START && stage != FRAME_RENDER_END)
	{
		return;
	}
	static int originalIdx = 0;

	if (!csgo_data.local) {
		originalIdx = 0;
		return;
	}
	const char* model = "models/player/custom_player/scaffold_t.mdl";
	if (stage == FRAME_RENDER_START) {
		originalIdx = csgo_data.local->get_model_index();
		if (const auto modelprecache = client_string_table_container->FindTable("modelprecache")) {
			modelprecache->AddString(false, model);
			//const auto viewmodelArmConfig = memory->getPlayerViewmodelArmConfigForPlayerModel(model);
		//	modelprecache->AddString(false, viewmodelArmConfig[2]);
			//modelprecache->AddString(false, viewmodelArmConfig[3]);
		}
	}
	const auto idx = stage == FRAME_RENDER_END && originalIdx ? originalIdx : g_model_info->get_model_index(model);

	csgo_data.local->set_model_index(idx);

}
void visuals::draw_spread()
{
	if (!g_engine->IsInGame() ||!csgo_data.local|| csgo_data.local->hp() <= 0 || !csgo_data.local->get_active_weapon())return;
	float accuracy = csgo_data.local->get_active_weapon()->get_inaccuracy() * 550.f; //3000
	g_surface->DrawFilledCircle(csgo_data.screen_center,cfg_vars.visuals.spread_color, accuracy, 120);
}
void visuals::third_person()
{
	static bool isInThirdperson{ true };
	static float lastTime{ 0.0f };

	/*if (GetAsyncKeyState(config->visuals.thirdpersonKey) && memory->globalVars->realtime - lastTime > 0.5f) {
		isInThirdperson = !isInThirdperson;
		lastTime = memory->globalVars->realtime;
	}*/

	if (cfg_vars.misc.thirdperson && csgo_data.local)
	{
		g_input->m_fCameraInThirdPerson = true;
		g_input->m_vecCameraOffset.z = static_cast<float>(cfg_vars.misc.third_person_distance);		
	}
	else if(!cfg_vars.misc.thirdperson && csgo_data.local) {
		g_input->m_fCameraInThirdPerson = false;
	}
		/*if (g_input->m_fCameraInThirdPerson = (!config->visuals.thirdpersonKey || isInThirdperson)
			&& localPlayer && localPlayer->isAlive())
			memory->input->cameraOffset.z = static_cast<float>(config->visuals.thirdpersonDistance);*/
}
void visuals::remove_smoke()
{	
	static std::vector<std::string> smoke_materials =
	{
		("particle/beam_smoke_01"),
		("particle/particle_smokegrenade"),
		("particle/particle_smokegrenade1"),
		("particle/particle_smokegrenade2"),
		("particle/particle_smokegrenade3"),
		("particle/particle_smokegrenade_sc"),
		("particle/smoke1/smoke1"),
		("particle/smoke1/smoke1_ash"),
		("particle/smoke1/smoke1_nearcull"),
		("particle/smoke1/smoke1_nearcull2"),
		("particle/smoke1/smoke1_snow"),
		("particle/smokesprites_0001"),
		("particle/smokestack"),
		("particle/vistasmokev1/vistasmokev1"),
		("particle/vistasmokev1/vistasmokev1_emods"),
		("particle/vistasmokev1/vistasmokev1_emods_impactdust"),
		("particle/vistasmokev1/vistasmokev1_fire"),
		("particle/vistasmokev1/vistasmokev1_nearcull"),
		("particle/vistasmokev1/vistasmokev1_nearcull_fog"),
		("particle/vistasmokev1/vistasmokev1_nearcull_nodepth"),
		("particle/vistasmokev1/vistasmokev1_smokegrenade"),
		("particle/vistasmokev1/vistasmokev4_emods_nocull"),
		("particle/vistasmokev1/vistasmokev4_nearcull"),
		("particle/vistasmokev1/vistasmokev4_nocull")
	};
	static std::vector<const char*>smoke_mats =
	{
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
	};
	static bool o_remove = cfg_vars.visuals.remove_smoke;
	if (o_remove != cfg_vars.visuals.remove_smoke)
	{
		for (std::string mat_n : smoke_materials)
		{
			IMaterial* mat = g_mat_system->FindMaterial(mat_n.c_str(), "Other textures");
			if(mat)
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, cfg_vars.visuals.remove_smoke);
		}	
		o_remove = cfg_vars.visuals.remove_smoke;
	}	
	if (cfg_vars.visuals.remove_smoke)
	{
		DWORD smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
		int* p_smoke = (int*)(smokecout);
		*p_smoke = 0;
	}
}
void visuals::reduce_flash()
{
	if (csgo_data.local)
	{
		csgo_data.local->get_max_flash_alpha() = 255.f - (2.55f *cfg_vars.visuals.alpha_reducing);
	}
}

void visuals::draw_distance(c_base_player* pl, int i)
{
	if (!csgo_data.local || !pl || !visuals_data[i].can_see_through_view_matrix)return;
	float distance = pl->get_origin().DistTo(csgo_data.local->get_origin());
	distance /= 100.f;
	g_surface->DrawT(visuals_data[i].head2D.x, visuals_data[i].head2D.y, Color(255.f, 255.f, 255.f, 255.f), verdana, true, std::to_string((int)distance).c_str());
}

