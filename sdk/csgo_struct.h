#pragma once
#include "../interfaces/ClientClass.hpp"
#include "../includes.hpp"
#include "sdk.hpp"
#include "../utils/utils.h"
#include <sdk\EHandle.h>
#define BONE_CALCULATE_MASK            0x1F
#define BONE_PHYSICALLY_SIMULATED    0x01    // bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL        0x02    // procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL        0x04    // bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE    0x08    // bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER    0x10    // bone aligns to the screen, constrained by it's own axis.
#define BONE_USED_BY_ANYTHING        0x0007FF00
#define BONE_USED_BY_HITBOX            0x00000100    // bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT        0x00000200    // bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK    0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0    0x00000400    // bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1    0x00000800    
#define BONE_USED_BY_VERTEX_LOD2    0x00001000  
#define BONE_USED_BY_VERTEX_LOD3    0x00002000
#define BONE_USED_BY_VERTEX_LOD4    0x00004000
#define BONE_USED_BY_VERTEX_LOD5    0x00008000
#define BONE_USED_BY_VERTEX_LOD6    0x00010000
#define BONE_USED_BY_VERTEX_LOD7    0x00020000
#define BONE_USED_BY_BONE_MERGE        0x00040000    // bone is available for bone merge to occur against it
#define HITBOX_HEAD   0
#define HITBOX_NECK   1
#define HITBOX_PELVIS 2
#define HITBOX_BELLY  3
#define HITBOX_THORAX 4
#define HITBOX_LOWER_CHEST 5
#define HITBOX_UPPER_CHEST 6
#define HITBOX_RIGHT_THIGH 7
#define HITBOX_LEFT_THIGH  8
#define HITBOX_RIGHT_CALF  9
#define HITBOX_LEFT_CALF  10
#define HITBOX_RIGHT_FOOT 11
#define HITBOX_LEFT_FOOT  12
#define HITBOX_RIGHT_HAND 13
#define HITBOX_LEFT_HAND  14
#define HITBOX_RIGHT_UPPER_ARM 15
#define HITBOX_RIGHT_FOREARM   16
#define HITBOX_LEFT_UPPER_ARM  17
#define HITBOX_LEFT_FOREARM    18
#define HITBOX_MAX 19
struct netvar_table {
	std::string               name;
	RecvProp* prop;
	uint32_t                  offset;
	std::vector<RecvProp*>    child_props;
	std::vector<netvar_table> child_tables;
};

class netvar_manager : public singleton <netvar_manager> {
public:
public:
	void init();
	void dump();
	uint32_t get_offset(const std::string& tableName, const std::string& propName);
private:	
	uint32_t get_offset(netvar_table& table, const std::string& propName);
	netvar_table load_table(RecvTable* table);
	std::vector<netvar_table>data;
	ClientClass* client_class_all;
};
inline netvar_manager g_netvar;
struct datamap_t;
struct typedescription_t
{
	int32_t fieldType; //0x0000
	char* fieldName; //0x0004
	int fieldOffset[2]; //0x0008
	int16_t fieldSize_UNKNWN; //0x0010
	int16_t flags_UNKWN; //0x0012
	char pad_0014[12]; //0x0014
	datamap_t* td; //0x0020
	char pad_0024[24]; //0x0024
}; //Size: 0x003C
struct datamap_t
{
	typedescription_t* dataDesc;
	int                    dataNumFields;
	char const* dataClassName;
	datamap_t* baseMap;

	bool                chains_validated;
	// Have the "packed" offsets been computed
	bool                packed_offsets_computed;
	int                    packed_size;
};
enum class ItemDefinitionIndex : short
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525
};
enum ClassId {
	ClassId_CAI_BaseNPC = 0,
	ClassId_CAK47,
	ClassId_CBaseAnimating,
	ClassId_CBaseAnimatingOverlay,
	ClassId_CBaseAttributableItem,
	ClassId_CBaseButton,
	ClassId_CBaseCombatCharacter,
	ClassId_CBaseCombatWeapon,
	ClassId_CBaseCSGrenade,
	ClassId_CBaseCSGrenadeProjectile,
	ClassId_CBaseDoor,
	ClassId_CBaseEntity,
	ClassId_CBaseFlex,
	ClassId_CBaseGrenade,
	ClassId_CBaseParticleEntity,
	ClassId_CBasePlayer,
	ClassId_CBasePropDoor,
	ClassId_CBaseTeamObjectiveResource,
	ClassId_CBaseTempEntity,
	ClassId_CBaseToggle,
	ClassId_CBaseTrigger,
	ClassId_CBaseViewModel,
	ClassId_CBaseVPhysicsTrigger,
	ClassId_CBaseWeaponWorldModel,
	ClassId_CBeam,
	ClassId_CBeamSpotlight,
	ClassId_CBoneFollower,
	ClassId_CBRC4Target,
	ClassId_CBreachCharge,
	ClassId_CBreachChargeProjectile,
	ClassId_CBreakableProp,
	ClassId_CBreakableSurface,
	ClassId_CBumpMine,
	ClassId_CBumpMineProjectile,
	ClassId_CC4,
	ClassId_CCascadeLight,
	ClassId_CChicken,
	ClassId_CColorCorrection,
	ClassId_CColorCorrectionVolume,
	ClassId_CCSGameRulesProxy,
	ClassId_CCSPlayer,
	ClassId_CCSPlayerResource,
	ClassId_CCSRagdoll,
	ClassId_CCSTeam,
	ClassId_CDangerZone,
	ClassId_CDangerZoneController,
	ClassId_CDEagle,
	ClassId_CDecoyGrenade,
	ClassId_CDecoyProjectile,
	ClassId_CDrone,
	ClassId_CDronegun,
	ClassId_CDynamicLight,
	ClassId_CDynamicProp,
	ClassId_CEconEntity,
	ClassId_CEconWearable,
	ClassId_CEmbers,
	ClassId_CEntityDissolve,
	ClassId_CEntityFlame,
	ClassId_CEntityFreezing,
	ClassId_CEntityParticleTrail,
	ClassId_CEnvAmbientLight,
	ClassId_CEnvDetailController,
	ClassId_CEnvDOFController,
	ClassId_CEnvGasCanister,
	ClassId_CEnvParticleScript,
	ClassId_CEnvProjectedTexture,
	ClassId_CEnvQuadraticBeam,
	ClassId_CEnvScreenEffect,
	ClassId_CEnvScreenOverlay,
	ClassId_CEnvTonemapController,
	ClassId_CEnvWind,
	ClassId_CFEPlayerDecal,
	ClassId_CFireCrackerBlast,
	ClassId_CFireSmoke,
	ClassId_CFireTrail,
	ClassId_CFish,
	ClassId_CFists,
	ClassId_CFlashbang,
	ClassId_CFogController,
	ClassId_CFootstepControl,
	ClassId_CFunc_Dust,
	ClassId_CFunc_LOD,
	ClassId_CFuncAreaPortalWindow,
	ClassId_CFuncBrush,
	ClassId_CFuncConveyor,
	ClassId_CFuncLadder,
	ClassId_CFuncMonitor,
	ClassId_CFuncMoveLinear,
	ClassId_CFuncOccluder,
	ClassId_CFuncReflectiveGlass,
	ClassId_CFuncRotating,
	ClassId_CFuncSmokeVolume,
	ClassId_CFuncTrackTrain,
	ClassId_CGameRulesProxy,
	ClassId_CGrassBurn,
	ClassId_CHandleTest,
	ClassId_CHEGrenade,
	ClassId_CHostage,
	ClassId_CHostageCarriableProp,
	ClassId_CIncendiaryGrenade,
	ClassId_CInferno,
	ClassId_CInfoLadderDismount,
	ClassId_CInfoMapRegion,
	ClassId_CInfoOverlayAccessor,
	ClassId_CItem_Healthshot,
	ClassId_CItemCash,
	ClassId_CItemDogtags,
	ClassId_CKnife,
	ClassId_CKnifeGG,
	ClassId_CLightGlow,
	ClassId_CMaterialModifyControl,
	ClassId_CMelee,
	ClassId_CMolotovGrenade,
	ClassId_CMolotovProjectile,
	ClassId_CMovieDisplay,
	ClassId_CParadropChopper,
	ClassId_CParticleFire,
	ClassId_CParticlePerformanceMonitor,
	ClassId_CParticleSystem,
	ClassId_CPhysBox,
	ClassId_CPhysBoxMultiplayer,
	ClassId_CPhysicsProp,
	ClassId_CPhysicsPropMultiplayer,
	ClassId_CPhysMagnet,
	ClassId_CPhysPropAmmoBox,
	ClassId_CPhysPropLootCrate,
	ClassId_CPhysPropRadarJammer,
	ClassId_CPhysPropWeaponUpgrade,
	ClassId_CPlantedC4,
	ClassId_CPlasma,
	ClassId_CPlayerPing,
	ClassId_CPlayerResource,
	ClassId_CPointCamera,
	ClassId_CPointCommentaryNode,
	ClassId_CPointWorldText,
	ClassId_CPoseController,
	ClassId_CPostProcessController,
	ClassId_CPrecipitation,
	ClassId_CPrecipitationBlocker,
	ClassId_CPredictedViewModel,
	ClassId_CProp_Hallucination,
	ClassId_CPropCounter,
	ClassId_CPropDoorRotating,
	ClassId_CPropJeep,
	ClassId_CPropVehicleDriveable,
	ClassId_CRagdollManager,
	ClassId_CRagdollProp,
	ClassId_CRagdollPropAttached,
	ClassId_CRopeKeyframe,
	ClassId_CSCAR17,
	ClassId_CSceneEntity,
	ClassId_CSensorGrenade,
	ClassId_CSensorGrenadeProjectile,
	ClassId_CShadowControl,
	ClassId_CSlideshowDisplay,
	ClassId_CSmokeGrenade,
	ClassId_CSmokeGrenadeProjectile,
	ClassId_CSmokeStack,
	ClassId_CSnowball,
	ClassId_CSnowballPile,
	ClassId_CSnowballProjectile,
	ClassId_CSpatialEntity,
	ClassId_CSpotlightEnd,
	ClassId_CSprite,
	ClassId_CSpriteOriented,
	ClassId_CSpriteTrail,
	ClassId_CStatueProp,
	ClassId_CSteamJet,
	ClassId_CSun,
	ClassId_CSunlightShadowControl,
	ClassId_CSurvivalSpawnChopper,
	ClassId_CTablet,
	ClassId_CTeam,
	ClassId_CTeamplayRoundBasedRulesProxy,
	ClassId_CTEArmorRicochet,
	ClassId_CTEBaseBeam,
	ClassId_CTEBeamEntPoint,
	ClassId_CTEBeamEnts,
	ClassId_CTEBeamFollow,
	ClassId_CTEBeamLaser,
	ClassId_CTEBeamPoints,
	ClassId_CTEBeamRing,
	ClassId_CTEBeamRingPoint,
	ClassId_CTEBeamSpline,
	ClassId_CTEBloodSprite,
	ClassId_CTEBloodStream,
	ClassId_CTEBreakModel,
	ClassId_CTEBSPDecal,
	ClassId_CTEBubbles,
	ClassId_CTEBubbleTrail,
	ClassId_CTEClientProjectile,
	ClassId_CTEDecal,
	ClassId_CTEDust,
	ClassId_CTEDynamicLight,
	ClassId_CTEEffectDispatch,
	ClassId_CTEEnergySplash,
	ClassId_CTEExplosion,
	ClassId_CTEFireBullets,
	ClassId_CTEFizz,
	ClassId_CTEFootprintDecal,
	ClassId_CTEFoundryHelpers,
	ClassId_CTEGaussExplosion,
	ClassId_CTEGlowSprite,
	ClassId_CTEImpact,
	ClassId_CTEKillPlayerAttachments,
	ClassId_CTELargeFunnel,
	ClassId_CTEMetalSparks,
	ClassId_CTEMuzzleFlash,
	ClassId_CTEParticleSystem,
	ClassId_CTEPhysicsProp,
	ClassId_CTEPlantBomb,
	ClassId_CTEPlayerAnimEvent,
	ClassId_CTEPlayerDecal,
	ClassId_CTEProjectedDecal,
	ClassId_CTERadioIcon,
	ClassId_CTEShatterSurface,
	ClassId_CTEShowLine,
	ClassId_CTesla,
	ClassId_CTESmoke,
	ClassId_CTESparks,
	ClassId_CTESprite,
	ClassId_CTESpriteSpray,
	ClassId_CTest_ProxyToggle_Networkable,
	ClassId_CTestTraceline,
	ClassId_CTEWorldDecal,
	ClassId_CTriggerPlayerMovement,
	ClassId_CTriggerSoundOperator,
	ClassId_CVGuiScreen,
	ClassId_CVoteController,
	ClassId_CWaterBullet,
	ClassId_CWaterLODControl,
	ClassId_CWeaponAug,
	ClassId_CWeaponAWP,
	ClassId_CWeaponBaseItem,
	ClassId_CWeaponBizon,
	ClassId_CWeaponCSBase,
	ClassId_CWeaponCSBaseGun,
	ClassId_CWeaponCycler,
	ClassId_CWeaponElite,
	ClassId_CWeaponFamas,
	ClassId_CWeaponFiveSeven,
	ClassId_CWeaponG3SG1,
	ClassId_CWeaponGalil,
	ClassId_CWeaponGalilAR,
	ClassId_CWeaponGlock,
	ClassId_CWeaponHKP2000,
	ClassId_CWeaponM249,
	ClassId_CWeaponM3,
	ClassId_CWeaponM4A1,
	ClassId_CWeaponMAC10,
	ClassId_CWeaponMag7,
	ClassId_CWeaponMP5Navy,
	ClassId_CWeaponMP7,
	ClassId_CWeaponMP9,
	ClassId_CWeaponNegev,
	ClassId_CWeaponNOVA,
	ClassId_CWeaponP228,
	ClassId_CWeaponP250,
	ClassId_CWeaponP90,
	ClassId_CWeaponSawedoff,
	ClassId_CWeaponSCAR20,
	ClassId_CWeaponScout,
	ClassId_CWeaponSG550,
	ClassId_CWeaponSG552,
	ClassId_CWeaponSG556,
	ClassId_CWeaponShield,
	ClassId_CWeaponSSG08,
	ClassId_CWeaponTaser,
	ClassId_CWeaponTec9,
	ClassId_CWeaponTMP,
	ClassId_CWeaponUMP45,
	ClassId_CWeaponUSP,
	ClassId_CWeaponXM1014,
	ClassId_CWorld,
	ClassId_CWorldVguiText,
	ClassId_DustTrail,
	ClassId_MovieExplosion,
	ClassId_ParticleSmokeGrenade,
	ClassId_RocketTrail,
	ClassId_SmokeTrail,
	ClassId_SporeExplosion,
	ClassId_SporeTrail,
};
enum move_type : int {
	MOVETYPE_NONE,//freeze
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,//npc move
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,

};
enum EntityFlags : int
{
	FL_ONGROUND = (1 << 0),
	FL_DUCKING = (1 << 1),
	FL_WATERJUMP = (1 << 2),
	FL_ONTRAIN = (1 << 3),
	FL_INRAIN = (1 << 4),
	FL_FROZEN = (1 << 5),
	FL_ATCONTROLS = (1 << 6),
	FL_CLIENT = (1 << 7),
	FL_FAKECLIENT = (1 << 8)
};
#define g_val(type, offset) return *(type*)(uintptr_t(this) + offset );
class c_base_entity : public IClientEntity{
public:
	datamap_t* get_data_desk_map() {
		typedef datamap_t* (__thiscall* o_GetPredDescMap)(void*);
		return v_funk::call_func<o_GetPredDescMap>(this, 15)(this);
	}
	datamap_t* GetPredDescMap() {
		typedef datamap_t* (__thiscall* o_GetPredDescMap)(void*);
		return v_funk::call_func<o_GetPredDescMap>(this, 17)(this);
	}
	static __forceinline c_base_entity* GetEntityByIndex(int index) {
		return static_cast<c_base_entity*>(g_entity_list->GetClientEntity(index));
	}
	static __forceinline c_base_entity* get_entity_from_handle(CBaseHandle h) {
		return static_cast<c_base_entity*>(g_entity_list->GetClientEntityFromHandle(h));
	}
	Vector& get_abs_origin() {
		typedef Vector& (__thiscall* o_get_origin)(void*);
		return v_funk::call_func<o_get_origin>(this, 10)(this);
	}
	Vector& get_abs_angles() {
		typedef Vector& (__thiscall* o_get_origin)(void*);
		return v_funk::call_func<o_get_origin>(this, 11)(this);
	}
	
	bool is_player() {
		return v_funk::call_func<bool(__thiscall*)(c_base_entity*)>(this, 157)(this);
	}
	bool is_loot() {
		int m_ClassID = this->GetClientClass()->m_ClassID;
		return m_ClassID == ClassId_CPhysPropAmmoBox ||
			m_ClassID == ClassId_CPhysPropLootCrate ||
			m_ClassID == ClassId_CPhysPropRadarJammer ||
			m_ClassID == ClassId_CPhysPropWeaponUpgrade ||
			m_ClassID == ClassId_CDrone ||
			m_ClassID == ClassId_CDronegun ||
			m_ClassID == ClassId_CItem_Healthshot ||
			m_ClassID == ClassId_CItemCash ||
			m_ClassID == ClassId_CBumpMine;
	}
	
	bool is_weapon() {
		return  v_funk::call_func<bool(__thiscall*)(c_base_entity*)>(this, 165)(this);
	}
	void set_model_index(int index) {
		typedef void(__thiscall* oSetModelIndex)(PVOID, int);
		return v_funk::call_func< oSetModelIndex >(this, 75)(this, index);
	}
	int get_model_index() {
		static int offset = g_netvar.get_offset("CBaseEntity", "m_nModelIndex");
		g_val(int, offset);
	}
};

class c_base_combat_weapon : public c_base_entity {
public:
	CCSWeaponInfo* GetCSWeaponData()
	{
		return g_weapon_system->GetWpnData((unsigned int)this->get_item_definition_index());
	}
	bool HasBullets();
	bool CanFire();
	bool IsGrenade();
	bool IsKnife();
	bool IsReloading();
	bool IsRifle();
	bool IsPistol();
	bool IsSniper();
	bool IsGun();	
	void UpdateAccuracyPenalty()
	{
		typedef void(__thiscall* o_inaccuracy)(LPVOID);
		return v_funk::call_func< o_inaccuracy >(this, 483)(this); //483	
	}
	float get_inaccuracy()
	{
		typedef float(__thiscall* o_inaccuracy)(LPVOID);
		return v_funk::call_func< o_inaccuracy >(this, 482)(this); //481		
	}
	float get_spread()
	{
		typedef float(__thiscall* o_weapon_spread)(PVOID);
		return v_funk::call_func< o_weapon_spread >(this, 452)(this); //451	
	}
	float& get_accuracy_penalty()
	{
		static int m_offset_accuracy_penalty = g_netvar.get_offset("DT_WeaponCSBase", "m_fAccuracyPenalty");
		g_val(float, m_offset_accuracy_penalty);
	}
	int get_zoom_lvl()
	{
		static int m_fLastShotTime = g_netvar.get_offset("DT_WeaponCSBase", "m_zoomLevel");
		g_val(int, m_fLastShotTime);
	}
	float get_next_primary_attack()
	{
		static int m_flNextPrimaryAttack = g_netvar.get_offset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
		g_val(float, m_flNextPrimaryAttack);
	}
	int get_ammo()
	{
		static int m_iClip1 = g_netvar.get_offset("DT_BaseCombatWeapon", "m_iClip1");
		g_val(int, m_iClip1);
	}
	ItemDefinitionIndex get_item_definition_index()
	{
		static int m_iItemDefinitionIndex = g_netvar.get_offset("DT_BaseAttributableItem", "m_iItemDefinitionIndex");
		g_val(ItemDefinitionIndex, m_iItemDefinitionIndex);
	}
	int GetActiveWeaponIndex()
	{
		static int m_hActiveWeapon = g_netvar.get_offset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		return *reinterpret_cast<int*>(uintptr_t(this) + m_hActiveWeapon) & 0xFFF;
	}
	
};
class c_base_player : public  c_base_entity{
public:
	c_base_combat_weapon* get_active_weapon()
	{
		static int m_hActiveWeapon = g_netvar.get_offset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		const auto weaponData = *(CHandle<c_base_combat_weapon*>*)((uintptr_t)this +m_hActiveWeapon);
		return reinterpret_cast<c_base_combat_weapon*>(g_entity_list->GetClientEntityFromHandle(weaponData));
	}
	
	/*__forceinline vec3_t& m_vecAbsVelocity() {
		return get< vec3_t >(g_entoffsets.m_vecAbsVelocity); 0x94
	}*/
	__forceinline bool& m_lifeState() {
		static int offset = g_netvar.get_offset("DT_BasePlayer", "m_lifeState");
		g_val(bool, offset);
	}
	bool dormant(){
		g_val(bool,0xED);
	}
	Vector& get_origin() {
		static int offset = g_netvar.get_offset("DT_BaseEntity", "m_vecOrigin");
		g_val(Vector, offset);
	}
	Vector get_eye_position_f() {
		static int offset = g_netvar.get_offset("DT_BasePlayer", "m_vecViewOffset[0]");
		Vector eye = *(Vector*)(uintptr_t(this) + offset);
		Vector buffer = eye + get_origin();
		return buffer;
	}
	Vector& get_eye_position() {
		static int offset = g_netvar.get_offset("DT_BasePlayer", "m_vecViewOffset[0]");
		g_val(Vector, offset);
	}
	Vector& get_velocity() {
		static int offset = 0x114;
		g_val(Vector, offset);
	}
	Vector& get_aim_punch_angle() {
		static int offset = g_netvar.get_offset("DT_BasePlayer", "m_aimPunchAngle");
		g_val(Vector, offset);
	}

	Vector get_hitbox_position(int id)
	{
		matrix3x4_t boneMatrix[128];

		if (SetupBones(boneMatrix, 128, BONE_USED_BY_HITBOX, 0.0f)) {
			auto studio_model = g_model_info->get_studio_model(GetModel());
			if (studio_model) {
				auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(id);
				if (hitbox) {
					auto
						min = Vector{},
						max = Vector{};
					utils::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
					utils::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);
					return (min + max) / 2.0f;
				}
			}
		}
		return Vector{};
	}
	Vector GetEyeAngles()
	{
		static int offset = g_netvar.get_offset("DT_CSPlayer", "m_angEyeAngles");
		g_val(Vector, offset);
	}
	bool is_alive() {
		if (this)
		{
			static int offset = g_netvar.get_offset("DT_BasePlayer", "m_lifeState");
			return !(*(bool*)(uintptr_t(this) + offset)) && this->hp() > 0;
		}
		return false;
	}
	int get_team() {
		static int offset = g_netvar.get_offset("DT_BaseEntity", "m_iTeamNum");
		g_val(int, offset);
	}
	int hp() {
		static int offset = 0x100;
		g_val(int, offset);
	}
	int& get_flags() {
		static int offset = g_netvar.get_offset("DT_BasePlayer", "m_fFlags");
		g_val(int, offset);
	}
	CBaseHandle& h_ground_entity() {
		static int offset = g_netvar.get_offset("DT_BasePlayer", "m_hGroundEntity");
		g_val(CBaseHandle, offset);
	}
	bool is_enemy(); 
	int get_index() {
		for (int i = 1; i < 64; i++)
		{
			IClientEntity* pl = g_entity_list->GetClientEntity(i);
			if (pl == this)return i;
		}
		return 0;
	}
	bool is_scoped() {
		static int offset = g_netvar.get_offset("DT_CSPlayer", "m_bIsScoped");
		g_val(bool, offset);
	}
	const matrix3x4_t& m_rgflCoordinateFrame()
	{
		static auto _m_rgflCoordinateFrame = g_netvar.get_offset("DT_BaseEntity", "m_CollisionGroup") - 0x30;
		return *(matrix3x4_t*)((uintptr_t)this + _m_rgflCoordinateFrame);
	}
	RECT get_bbox() {
		RECT rect{};
		ICollideable* collideable = this->GetCollideable();

		if (!collideable)
			return rect;

		Vector min = collideable->OBBMins();
		Vector max = collideable->OBBMaxs();

		const matrix3x4_t& trans = this->m_rgflCoordinateFrame();

		Vector points[] = {
			Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(min.x, min.y, max.z),
			Vector(max.x, min.y, max.z)
		};

		Vector pointsTransformed[8];
		for (int i = 0; i < 8; i++) {
			utils::VectorTransform(points[i], trans, pointsTransformed[i]);
			if (i >= 4)
			{
				pointsTransformed[i].z += 2.f;
			}
		}			
		Vector screen_points[8] = {};
		for (int i = 0; i < 8; i++) {
			if (!utils::get().WorldToScreen(pointsTransformed[i], screen_points[i]))
				return rect;
		}
		float left = screen_points[0].x;
		float top = screen_points[0].y;
		float right = screen_points[0].x;
		float bottom = screen_points[0].y;

		for (int i = 1; i < 8; i++) {
			if (left > screen_points[i].x)
				left = screen_points[i].x;
			if (top < screen_points[i].y)
				top = screen_points[i].y;
			if (right < screen_points[i].x)
				right = screen_points[i].x;
			if (bottom > screen_points[i].y)
				bottom = screen_points[i].y;
		}
		return RECT{ (long)left, (long)top, (long)right, (long)bottom };
	}
	int& get_tick_base() {
		static int offset = g_netvar.get_offset("DT_BasePlayer", "m_nTickBase");
		g_val(int, offset);
	}
	int get_fired_shots() {
		static int offset = g_netvar.get_offset("DT_CSPlayer", "m_iShotsFired");
		g_val(int, offset);
	}
	bool* get_p_bSpotted() {
		static int offset = g_netvar.get_offset("DT_BaseEntity", "m_bSpotted");
		return (bool*)((uintptr_t)(this) + offset);
	}
	bool can_see(c_base_player* player, Vector hitbox);
	inline Vector ExtrapolateTick(Vector p0, Vector v0)
	{
		if(g_global_vars)
		return p0 + (v0 * g_global_vars->interval_per_tick);
		return Vector(0, 0, 0);
	}
	Vector GetPredicted(Vector p0)
	{	
		return ExtrapolateTick(p0, this->get_velocity());		
	}
	int get_movement_type()
	{
		return *(int*)((uintptr_t)(this) + 0x25C);//movement type offset
	}
	int get_chocked_commands()
	{
		return *(int*)(uintptr_t(g_pClientState) + 0x4D28);
	}
	float& p_health_shot_expirationTime()
	{
		static int offset = g_netvar.get_offset("DT_CSPlayer", "m_flHealthShotBoostExpirationTime");
		return *(float*)(uintptr_t(this) + offset);
	}
	int& get_think_tick();	
	void think();
	void pre_think();
	void run_think();
	void run_pre_think();
	c_user_cmd* get_current_command() {
		static int offset = g_netvar.get_offset("DT_BasePlayer", "m_hConstraintEntity");
		return *reinterpret_cast<c_user_cmd**>(uintptr_t(this) + offset - 0xC);
	}
	float& get_max_flash_alpha() {
		static int m_flFlashMaxAlpha = g_netvar.get_offset("DT_CSPlayer", "m_flFlashMaxAlpha");
		g_val(float, m_flFlashMaxAlpha);
	}
	float get_sim_time() {
		static int offset = g_netvar.get_offset("DT_BaseEntity", "m_flSimulationTime");
		g_val(float, offset);
	}
	bool is_alive_funk() {
		typedef bool(__thiscall* o_alive)(void* ecx);
		return v_funk::call_func<o_alive>(this, 155)(this);
	}
	
	void set_abs_origin(const Vector& origin)
	{
		using Fn = void(__thiscall*)(void*, const Vector&);
		static auto fn = reinterpret_cast<Fn>(utils::get().pattern_scan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1"));
		return fn(this, origin);
	}
};