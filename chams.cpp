#include "chams.h"
#include "sdk/sdk.hpp"
#include "sdk/csgo_struct.h"
#include "hooks/hooks.h"
#include "sdk/sdk.hpp"
#include "hooks/hooks.h"
#include "hooks/hook_manager.hpp"
#include <backend\cfg_system.hpp>
#include <sdk\csgo.h>
//#include <fstream>

void chams::init_materials()
{
	std::string lumen_metall = R"#("VertexLitGeneric" {
	  $envmap "/models/effects/crystal_cube_vertigo_hdr"
	$envmaptint "[.5 .9 .6]"
	$envmaplightscale 0.01
	$envmapfresnelminmaxexp "[0 1 2]"
	$alpha 0.9
	$envmapcontrast 1
	$phong 1
	$phongamount 256
	$phongexponent 125
	$phongboost 2.5
	$rimlight               1
	$rimlightexponent       2
	$rimlightboost          .2	
	$model 1
	$nocull 0
	$selfillum 1 
	$halfambert 1
	$znearer 0
	$nofog 1
	$color "[0.7 1 0.7]"
	$ignorez 0
}
)#";
	std::string default_pulse = R"#("VertexLitGeneric" {
		"$basetexture" "vgui/white"
	"$envmap" "env_cubemap"
	"$envmaptint"	 "[1 1 1]"
	$envmapcontrast 1
	$color "[1 0 0]"

	"$nofog" "1"
            "$lightwarptexture"             "metalic"
		proxies	{
		sine{
			sineperiod 2
			sinemin 0.4
			sinemax 1
			resultvar "$color[0]"
		}
		sine{
			sineperiod 3.5
			sinemin 0.3
			sinemax 1
			resultvar "$color[1]"
		}
		sine{
			sineperiod 6.5
			sinemin 0.2
			sinemax 1
			resultvar "$color[2]"
		}
		sine{
			sineperiod 1.4
			sinemin 0.3
			sinemax 1
			resultvar "$alpha"
		}
	}
}
)#";
	std::string _glow = R"#("VertexLitGeneric" { 
    "$additive" "1"
	"$flat" "0"
	 $model 1
	 $nocull 0
	 $selfillum 1 
	 $halfambert 1
	 $znearer 0
	 $nofog 1
    "$envmap" "env_cubemap"
    "$envmaptint" "[1.5 .7 .7]"
    "$envmapfresnel" "1"
    "$envmapfresnelminmaxexp" "[0 1 2]"
    "$alpha" "1.0"
})#";
	std::string _glow_arm_race = R"#("VertexLitGeneric" { 
   "$additive" "1"
	"$envmap" "models/effects/cube_white"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "0.8"
})#";
	std::string pearlescent = R"#("VertexLitGeneric" {
		"$basetexture"                    "vgui/white_additive"
		"$envmap"                         "env_cubemap" 
		"$envmaptint"					  "[1 4.7 0.6]"
		"$envmapcontrast"						"1"
		"$envmaplightscale"						"1"
		"$phong"								"1"
		"$phongboost"							"1"
		"$phongexponent"						"280"
		"$rimlight"								"1"
		"$rimlightexponent"						"1"
		"$rimlightboost"						"100"
		"$halflambert"							"1"
		"$nocull"								"1"
		"$lightwarptexture"						"2000"		
		"$lightmapwaterfog"						"0"
		"$pearlescent"							"12"
}
		)#";
	std::string defaulT = R"#("VertexLitGeneric" {	
"$basetexture"	"vgui/white"
	"$model"		"1"
	"$flat"			"0"
	"$nocull"		"1"
	"$halflambert"	"1"
	"$nofog"		"1"
	"$ignorez"		"0"
	"$znearer"		"0"
	"$wireframe"	"0" 
	})#";
	std::string invis = R"#("VertexLitGeneric" {	
	})#";
	std::string reflectivity = R"#("VertexLitGeneric" {	
	 "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       "env_cubemap"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast"             "1"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
	})#";
	//default_mat_pulse = create_material("standart_textured_chams_pulse.vmt", false, false, default_pulse);
	//glow_second = g_mat_system->FindMaterial("regular_glow");	
	default_mat = create_material("default_material.vmt", false, false, defaulT);	
	metallic_mat = create_material("lumen_metallic.vmt", false, false, lumen_metall);
	pearlescent_mat = create_material("pearlescent_lumen.vmt", false, false, pearlescent);
	weapon_blink = g_mat_system->FindMaterial("models/inventory_items/dogtags/dogtags_outline");
	weapon_plastik = g_mat_system->FindMaterial("models/inventory_items/trophy_majors/gloss");
	glass = g_mat_system->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass");
	crystal = g_mat_system->FindMaterial("models/inventory_items/trophy_majors/crystal_blue");
	velvet = g_mat_system->FindMaterial("models/inventory_items/trophy_majors/velvet");		
	glow = create_material("glow_lumen_c.vmt", false, false, _glow, chams_.glow_key);
	glow_armrace = create_material("glow_lumen_c2.vmt", false, false, _glow_arm_race);
	weapon_gold = g_mat_system->FindMaterial("models/inventory_items/trophy_majors/gold");	
	invis_material = create_material("invis.vmt", false, false, invis);
	reflectivity_mat = create_material("reflect2ivity_by_lumen.vmt", false, false, reflectivity);
}
void chams::overrideMat(IMaterial* mat, bool ignoreZ, bool wireFrame, Color color) noexcept
{
	if (mat && g_mat_system && !mat->IsErrorMaterial())  {
		try {	
			
			//g_model_render->ForcedMaterialOverride(nullptr);
			mat->IncrementReferenceCount();			
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireFrame);
			mat->ColorModulate(color.r, color.g, color.b);
			g_model_render->ForcedMaterialOverride(mat);
			mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignoreZ);
			mat->IncrementReferenceCount();
		}
		catch (...)
		{

		}
	}
}
void chams::overrideMat(IMaterial* mat, bool ignoreZ, bool wireFrame) noexcept
{
	if (mat && !mat->IsErrorMaterial()) {
		g_model_render->ForcedMaterialOverride(nullptr);
		mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignoreZ);
		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireFrame);
		g_model_render->ForcedMaterialOverride(mat);
		mat->IncrementReferenceCount();
	}
}
IMaterial* chams::create_material(std::string name, bool wireframe, bool ignorez, std::string data, KeyValues* val)
{
	KeyValues* key_values = nullptr;;
	if (val == nullptr) key_values = new KeyValues();
	else key_values = val;
	utils::init_key_values(key_values, "VertexLitGeneric");
	utils::load_from_buffer(key_values, name.c_str(), data.c_str());
	return g_mat_system->CreateMaterial(name.c_str(), key_values);
}
typedef void(__fastcall* fnDrawModelExecure)(void*, int, IMatRenderContext*, const DrawModelState_t&, ModelRenderInfo_t&, matrix3x4_t*);
void chams::do_players_chams(void*ecx, int edx,void* ctx, const DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	
}
void chams::do_weapon_chams()
{
	int type = cfg_vars.visuals.weapon_chams_type;
	if (!weapon_cur_mat)return;
	if (!cfg_vars.visuals.animated_chams) {
		if (type != 5 && type != 3) {
			if (should_material_envmap_override(type))
			{
				weapon_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(type >=  9 ? cfg_vars.visuals.weapon_color / 55.f :  cfg_vars.visuals.weapon_color / 255.f);
				g_render_view->SetBlend(cfg_vars.visuals.weapon_color.a / 255.f);
			}
			else {
				float flColor[3] = { cfg_vars.visuals.weapon_color.r / 255.f, cfg_vars.visuals.weapon_color.g / 255.f,  cfg_vars.visuals.weapon_color.b / 255.f };
				g_render_view->SetBlend(cfg_vars.visuals.weapon_color.a / 255.f);
				g_render_view->SetColorModulation(flColor);
			}
		}
		else {
			if (type == 5)
			{
				overrideMat(weapon_cur_mat, false, false);
				return;
			}
			else {
				float flColor[3] = { cfg_vars.visuals.weapon_color.r / 15.f, cfg_vars.visuals.weapon_color.g / 15.f,  cfg_vars.visuals.weapon_color.b / 15.f };
				g_render_view->SetBlend(cfg_vars.visuals.weapon_color.a / 255.f);
				g_render_view->SetColorModulation(flColor);
				return;
			}
		}
		overrideMat(weapon_cur_mat, false, false);
	}
	else {
		if (type != 5 && type != 3) {
			if (should_material_envmap_override(type))
			{
				weapon_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(type >= 9 ? csgo_data.c_rainbow__chams  * 5.5f : csgo_data.c_rainbow__chams );
				g_render_view->SetBlend(1.f);
			}
			else {
				float flColor[3] = { csgo_data.c_rainbow__chams.r, csgo_data.c_rainbow__chams.g,  csgo_data.c_rainbow__chams.b };
				g_render_view->SetBlend(1.f);
				g_render_view->SetColorModulation(flColor);
			}
		}
		else {
			if (type == 5)
			{
				overrideMat(weapon_cur_mat, false, false);
				return;
			}
			else {
				float flColor[3] = { csgo_data.c_rainbow__chams.r, csgo_data.c_rainbow__chams.g ,  csgo_data.c_rainbow__chams.b };
				g_render_view->SetBlend(1.f);
				g_render_view->SetColorModulation(flColor);
				return;
			}
		}
		overrideMat(weapon_cur_mat, false, false);
	}
}
void chams::do_arm_chams(void* ctx, const DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	int type = cfg_vars.visuals.hands_chams_type;
	if (!hand_cur_mat)return;
	if (!cfg_vars.visuals.animated_chams) {
		if (type != 5 && type != 3) {
			if (should_material_envmap_override(type))
			{
				hand_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(type >= 9 ? cfg_vars.visuals.hand_color / 55.f :  cfg_vars.visuals.hand_color / 255.f);
				g_render_view->SetBlend(cfg_vars.visuals.hand_color.a / 255.f);
			}
			else {
				float flColor[3] = { cfg_vars.visuals.hand_color.r / 255.f, cfg_vars.visuals.hand_color.g / 255.f,  cfg_vars.visuals.hand_color.b / 255.f };
				g_render_view->SetBlend(cfg_vars.visuals.hand_color.a / 255.f);
				g_render_view->SetColorModulation(flColor);
			}
		}
		else {
			if (type == 5)
			{
				overrideMat(hand_cur_mat, false, false);
				return;
			}
			else {
				float flColor[3] = { cfg_vars.visuals.hand_color.r / 15.f, cfg_vars.visuals.hand_color.g / 15.f,  cfg_vars.visuals.hand_color.b / 15.f };
				g_render_view->SetBlend(cfg_vars.visuals.hand_color.a / 255.f);
				g_render_view->SetColorModulation(flColor);
				return;
			}
		}
		overrideMat(hand_cur_mat, false, false);
	}
	else {
		if (type != 5 && type != 3) {
			if (should_material_envmap_override(type))
			{
				hand_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(type >= 9 ? csgo_data.c_rainbow__chams * 5.5f : csgo_data.c_rainbow__chams);
				g_render_view->SetBlend(1.f);
			}
			else {
				float flColor[3] = { csgo_data.c_rainbow__chams.r , csgo_data.c_rainbow__chams.g ,  csgo_data.c_rainbow__chams.b };
				g_render_view->SetBlend(1.f);
				g_render_view->SetColorModulation(flColor);
			}
		}
		else {
			if (type == 5)
			{
				overrideMat(hand_cur_mat, false, false);
				return;
			}
			else {
				float flColor[3] = { csgo_data.c_rainbow__chams.r, csgo_data.c_rainbow__chams.g ,  csgo_data.c_rainbow__chams.b  };
				g_render_view->SetBlend(1.f);
				g_render_view->SetColorModulation(flColor);
				return;
			}
		}
		overrideMat(hand_cur_mat, false, false);
	}
}
void chams::do_sleeves_chams() {

	int type = cfg_vars.visuals.sleeves_chams_type;
	if (!sleeves_cur_mat)return;
	if (!cfg_vars.visuals.animated_chams) {
		if (type != 5 && type != 3) {
			if (should_material_envmap_override(type))
			{
				sleeves_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(type >=  9 ? cfg_vars.visuals.sleeves_c_color / 55.f : cfg_vars.visuals.sleeves_c_color / 255.f);
				g_render_view->SetBlend(cfg_vars.visuals.sleeves_c_color.a / 255.f);
			}
			else {
				float flColor[3] = { cfg_vars.visuals.sleeves_c_color.r / 255.f, cfg_vars.visuals.sleeves_c_color.g / 255.f,  cfg_vars.visuals.sleeves_c_color.b / 255.f };
				g_render_view->SetBlend(cfg_vars.visuals.sleeves_c_color.a / 255.f);
				g_render_view->SetColorModulation(flColor);
			}
		}
		else
		{
			if (type == 5)
			{
				overrideMat(sleeves_cur_mat, false, false);
				return;
			}
			else {
				float flColor[3] = { cfg_vars.visuals.sleeves_c_color.r / 15.f, cfg_vars.visuals.sleeves_c_color.g / 15.f,  cfg_vars.visuals.sleeves_c_color.b / 15.f };
				g_render_view->SetBlend(cfg_vars.visuals.sleeves_c_color.a / 255.f);
				g_render_view->SetColorModulation(flColor);
				return;
			}
		}
		overrideMat(sleeves_cur_mat, false, false);
	}
	else {
		if (type != 5 && type != 3) {
			if (should_material_envmap_override(type))
			{
				hand_cur_mat->FindVar("$envmaptint", nullptr, false)->set_vector_value(type >= 9? csgo_data.c_rainbow__chams * 5.5f : csgo_data.c_rainbow__chams );
				g_render_view->SetBlend(1.f);
			}
			else {
				float flColor[3] = { csgo_data.c_rainbow__chams.r , csgo_data.c_rainbow__chams.g ,  csgo_data.c_rainbow__chams.b  };
				g_render_view->SetBlend(1.f);
				g_render_view->SetColorModulation(flColor);
			}
		}
		else {
			if (type == 5)
			{
				overrideMat(sleeves_cur_mat, false, false);
				return;
			}
			else {
				float flColor[3] = { csgo_data.c_rainbow__chams.r, csgo_data.c_rainbow__chams.g,  csgo_data.c_rainbow__chams.b  };
				g_render_view->SetBlend(1.f);
				g_render_view->SetColorModulation(flColor);
				return;
			}
		}
		overrideMat(sleeves_cur_mat, false, false);
	}
}
IMaterial* chams::get_material_by_cfg_material( int var)
{
	switch (var)
	{
	case 0:
		return default_mat;
		break;
	case 1:
		return metallic_mat;//,etallioc mat
		break;
	case 2:
		return pearlescent_mat;
		break;
	case 3:
		return default_mat;
		break;
	case 4:
		return glass;
		break;
	case 5:
		return crystal;
		break;
	case 6:
		return velvet;
		break;
	case 7:
		return weapon_gold;
		break;
	case 8:
		return weapon_blink;
		break;
	case 9:
		return glow;
		break;
	case 10:
		return glow_armrace;
		break;
	}
}
bool chams::should_material_envmap_override(int type)
{
	return type == 1 || type == 4 || type == 5 || type == 6 || type == 7 || type == 9 || type == 10;
//	return type != 2 && type != 3 && type == 8;
}

