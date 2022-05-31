#pragma once
class C_BaseEntity;
#include "../utils/Vector.h"
#include <Windows.h>
#include "IVEngineTrace.h"
#include "../utils/Color.h"
class Beam_t;
class C_Beam;
struct BeamInfo_t
{
	int			m_nType;

	// Entities
	IClientEntity* m_pStartEnt;
	int			m_nStartAttachment;
	IClientEntity* m_pEndEnt;
	int			m_nEndAttachment;

	// Points
	Vector		m_vecStart;
	Vector		m_vecEnd;

	int			m_nModelIndex;
	const char* m_pszModelName;

	int			m_nHaloIndex;
	const char* m_pszHaloName;
	float		m_flHaloScale;

	float		m_flLife;
	float		m_flWidth;
	float		m_flEndWidth;
	float		m_flFadeLength;
	float		m_flAmplitude;

	float		m_flBrightness;
	float		m_flSpeed;

	int			m_nStartFrame;
	float		m_flFrameRate;

	float		m_flRed;
	float		m_flGreen;
	float		m_flBlue;

	bool		m_bRenderable;

	int			m_nSegments;

	int			m_nFlags;

	// Rings
	Vector		m_vecCenter;
	float		m_flStartRadius;
	float		m_flEndRadius;

	BeamInfo_t()
	{
		m_nType = 0;
		m_nSegments = -1;
		m_pszModelName = NULL;
		m_pszHaloName = NULL;
		m_nModelIndex = -1;
		m_nHaloIndex = -1;
		m_bRenderable = true;
		m_nFlags = 0;
	}
};
class IViewRenderBeams
{
public:
	// Construction
public:
	IViewRenderBeams(void);
	virtual				~IViewRenderBeams(void);

	// Implement IViewRenderBeams
public:
	virtual	void		InitBeams(void);
	virtual	void		ShutdownBeams(void);
	virtual	void		ClearBeams(void);

	// Updates the state of the temp ent beams
	virtual void		UpdateTempEntBeams();

	virtual void		DrawBeam(Beam_t* pbeam);
	virtual void		DrawBeam(C_Beam* pbeam, ITraceFilter* pEntityBeamTraceFilter = NULL);

	virtual	void		KillDeadBeams(IClientEntity* pDeadEntity);

	virtual	void		CreateBeamEnts(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int type = -1);
	virtual Beam_t* CreateBeamEnts(BeamInfo_t& beamInfo);

	virtual	void		CreateBeamEntPoint(int	nStartEntity, const Vector* pStart, int nEndEntity, const Vector* pEnd,
		int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b);
	virtual Beam_t* CreateBeamEntPoint(BeamInfo_t& beamInfo);

	virtual	void		CreateBeamPoints(Vector& start, Vector& end, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b);
	virtual	Beam_t* CreateBeamPoints(BeamInfo_t& beamInfo);

	virtual	void		CreateBeamRing(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags);
	virtual Beam_t* CreateBeamRing(BeamInfo_t& beamInfo);

	virtual void		CreateBeamRingPoint(const Vector& center, float start_radius, float end_radius, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags);
	virtual Beam_t* CreateBeamRingPoint(BeamInfo_t& beamInfo);

	virtual	void		CreateBeamCirclePoints(int type, Vector& start, Vector& end,
		int modelIndex, int haloIndex, float haloScale, float life, float width,
		float endWidth, float fadeLength, float amplitude, float brightness, float speed,
		int startFrame, float framerate, float r, float g, float b);
	virtual Beam_t* CreateBeamCirclePoints(BeamInfo_t& beamInfo);

	virtual	void		CreateBeamFollow(int startEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float r, float g, float b,
		float brightness);
	virtual Beam_t* CreateBeamFollow(BeamInfo_t& beamInfo);

	void draw_beam_custom(Vector start, Vector end, int segments, float time,float width,float end_width, float fade,int type, Color col);
	void draw_beam_ring(int type,Vector origin, float radius, float life, float ampl, float speed, Color col);
	void draw_beam_ring(float life, float static_radius, Color col,Vector origin, float width);

};
struct bullet
{
	bullet(int userID, Vector FirePos, Vector i, float itime)
	{
		this->userid = userid;
		this->firepos = FirePos;
		this->impactpos = i;
		this->impacttime = itime;
	}
	Vector impactpos;
	float impacttime;
	Vector firepos;
	int userid;



};
enum
{
	FBEAM_STARTENTITY = 0x00000001,
	FBEAM_ENDENTITY = 0x00000002,
	FBEAM_FADEIN = 0x00000004,
	FBEAM_FADEOUT = 0x00000008,
	FBEAM_SINENOISE = 0x00000010,
	FBEAM_SOLID = 0x00000020,
	FBEAM_SHADEIN = 0x00000040,
	FBEAM_SHADEOUT = 0x00000080,
	FBEAM_ONLYNOISEONCE = 0x00000100,		// Only calculate our noise once
	FBEAM_NOTILE = 0x00000200,
	FBEAM_USE_HITBOXES = 0x00000400,		// Attachment indices represent hitbox indices instead when this is set.
	FBEAM_STARTVISIBLE = 0x00000800,		// Has this client actually seen this beam's start entity yet?
	FBEAM_ENDVISIBLE = 0x00001000,		// Has this client actually seen this beam's end entity yet?
	FBEAM_ISACTIVE = 0x00002000,
	FBEAM_FOREVER = 0x00004000,
	FBEAM_HALOBEAM = 0x00008000,		// When drawing a beam with a halo, don't ignore the segments and endwidth
	FBEAM_REVERSED = 0x00010000,
	NUM_BEAM_FLAGS = 17	// KEEP THIS UPDATED!
};