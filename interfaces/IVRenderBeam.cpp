#include "IVRenderBeam.h"
#include "../utils/utils.h"
#define	TE_BEAMPOINTS		0		// beam effect between two points
#define TE_SPRITE			1	// additive sprite, plays 1 cycle
#define TE_BEAMDISK			2	// disk that expands to max radius over lifetime
#define TE_BEAMCYLINDER		3		// cylinder that expands to max radius over lifetime
#define TE_BEAMFOLLOW		4		// create a line of decaying beam segments until entity stops moving
#define TE_BEAMRING			5		// connect a beam ring to two entities
#define TE_BEAMSPLINE		6		
#define TE_BEAMRINGPOINT	7
#define	TE_BEAMLASER		8		// Fades according to viewpoint
#define TE_BEAMTESLA		9
void IViewRenderBeams::draw_beam_custom(Vector start, Vector end, int segments, float time,float width,float end_width, float fade,int type, Color color)
{
	static bool once = true;
	if (once)
	{
		utils::get().precache_model("sprites/purplelaser1.vmt");
		utils::get().precache_model("sprites/glow01.vmt");
		utils::get().precache_model("sprites/laserbeam.vmt");
		utils::get().precache_model("sprites/white.vmt");
		utils::get().precache_model("sprites/ledglow.vmt");
		utils::get().precache_model("sprites/blueglow01.vmt");
		utils::get().precache_model("sprites/physbeam.vmt");
		once = false;
	}
	BeamInfo_t beamInfo;
	beamInfo.m_nType = 0;
	const char* mat_name = "";
	switch (type)
	{
	case 0:
		mat_name = "sprites/purplelaser1.vmt";
		break;
	case 1:
		mat_name = "sprites/glow01.vmt";
		break;
	case 2:
		mat_name = "sprites/laserbeam.vmt";
		break;
	case 3:
		mat_name = "sprites/white.vmt";
		break;
	case 4:
		mat_name = "sprites/ledglow.vmt";
		break;
	case 5:
		//mat_name = "sprites/bubble.vmt";
		mat_name = "sprites/blueglow01.vmt";
		break;
	case 6:
		mat_name = "sprites/bubble.vmt";
		break;
	case 7:
		mat_name = "sprites/physbeam.vmt";
		break;
	default:
		mat_name = "sprites/physbeam.vmt";
		break;
	}
	beamInfo.m_pszModelName = mat_name;
	beamInfo.m_nModelIndex = -1; // will be set by CreateBeamPoints if its -1
	beamInfo.m_flHaloScale = 3.0f;
	beamInfo.m_flLife = time;
	beamInfo.m_flWidth = width;
	beamInfo.m_flEndWidth = end_width;
	beamInfo.m_flFadeLength = fade;
	beamInfo.m_flAmplitude = 0.5f;
	beamInfo.m_flSpeed = 0.0f;
	beamInfo.m_nStartFrame = 0;
	beamInfo.m_flFrameRate = 0.f;
	beamInfo.m_flRed = color.r;
	beamInfo.m_flGreen = color.g;
	beamInfo.m_flBlue = color.b;
	beamInfo.m_nSegments = segments;
	beamInfo.m_bRenderable = true;
	beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM; //FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM
	beamInfo.m_flBrightness = 255.f;
	beamInfo.m_vecStart = start;
	beamInfo.m_vecEnd = end;
	if (this) {
		Beam_t* myBeam = this->CreateBeamPoints(beamInfo);
		if (myBeam)
			this->DrawBeam(myBeam); //renderBeam func
	}
}
#include "../sdk/sdk.hpp"
void IViewRenderBeams::draw_beam_ring(int type,Vector origin, float radius, float life, float width, float speed, Color col)
{
	static bool once = true;
	if (once)
	{
		utils::get().precache_model("sprites/crosshairs.vmt");
		utils::get().precache_model("sprites/white.vmt");
		utils::get().precache_model("sprites/player_red_small.vmt");
		utils::get().precache_model("sprites/numbers.vmt");
		utils::get().precache_model("sprites/scope_line_blur.vmt");
		once = false;
	}
	std::string buffer_name = "";
	switch (type)
	{
	case 0:
		buffer_name = "sprites/white.vmt";
		break;
	case 1:
		buffer_name = "sprites/crosshairs.vmt";
		break;
	case 2:
		buffer_name = "sprites/player_red_small.vmt";
		break;
	case 3:
		buffer_name = "sprites/numbers.vmt";
		break;
	case 4:
		buffer_name = "sprites/scope_line_blur.vmt";
		break;
	}
	BeamInfo_t beam_info;
	beam_info.m_nType = TE_BEAMRING;
	beam_info.m_pszModelName = buffer_name.c_str();
	beam_info.m_nModelIndex = g_model_info->get_model_index(buffer_name.c_str());
	beam_info.m_nHaloIndex = g_model_info->get_model_index(buffer_name.c_str());
	beam_info.m_pszHaloName = buffer_name.c_str();
	beam_info.m_flHaloScale = 6.f;
	beam_info.m_flLife = life;
	beam_info.m_flWidth = width;
	beam_info.m_flEndWidth = width;
	beam_info.m_flFadeLength = 1.5f;
	beam_info.m_flAmplitude = .1f;
	beam_info.m_flBrightness = col.a * 1.5f;
	beam_info.m_flSpeed = 0.1f;
	beam_info.m_nStartFrame = 0;
	beam_info.m_flFrameRate = 1.5f;
	beam_info.m_flRed = col.r;
	beam_info.m_flGreen = col.g;
	beam_info.m_flBlue = col.b;
	beam_info.m_nSegments = 2;
	beam_info.m_bRenderable = true;
	beam_info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;
	beam_info.m_vecCenter = origin + Vector(0, 0, 5);
	beam_info.m_flStartRadius = 1;
	beam_info.m_flEndRadius = radius;
	if (this) {
		Beam_t* myBeam = this->CreateBeamRingPoint(beam_info);
		if (myBeam)
			this->DrawBeam(myBeam); //renderBeam func
	}
}
void IViewRenderBeams::draw_beam_ring( float life,float static_radius, Color col,Vector origin, float width)
{
	static bool once = true;
	if (once)
	{
		utils::get().precache_model("sprites/purplelaser1.vmt");		
		once = false;
	}
	std::string buffer_name = "sprites/purplelaser1.vmt";
	BeamInfo_t beam_info;
	beam_info.m_nType = TE_BEAMRING;
	beam_info.m_pszModelName = buffer_name.c_str();
	beam_info.m_nModelIndex = g_model_info->get_model_index(buffer_name.c_str());
	beam_info.m_nHaloIndex = g_model_info->get_model_index(buffer_name.c_str());
	beam_info.m_pszHaloName = buffer_name.c_str();
	beam_info.m_flHaloScale = 3.f;
	beam_info.m_flLife = life;
	beam_info.m_flWidth = width;
	beam_info.m_flEndWidth = width;
	beam_info.m_flFadeLength = 0.f;
	beam_info.m_flAmplitude = .1f;
	beam_info.m_flBrightness = col.a * 2.f;
	beam_info.m_flSpeed = 0.05f;
	beam_info.m_nStartFrame = 0;
	beam_info.m_flFrameRate = 1.f;
	beam_info.m_flRed = col.r;
	beam_info.m_flGreen = col.g;
	beam_info.m_flBlue = col.b;
	beam_info.m_nSegments = 2;
	beam_info.m_bRenderable = true;
	beam_info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;
	beam_info.m_vecCenter = origin + Vector(0, 0, 2);
	beam_info.m_flStartRadius = 2;
	beam_info.m_flEndRadius = static_radius;
	if (this) {
		Beam_t* myBeam = this->CreateBeamRingPoint(beam_info);
		if (myBeam)
			this->DrawBeam(myBeam); //renderBeam func
	}
}
