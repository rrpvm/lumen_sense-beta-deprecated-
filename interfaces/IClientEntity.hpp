#pragma once
#include "IClientUnknown.hpp"
#include "IClientRenderable.hpp"
#include "IClientThinkable.hpp"
#include "IClientNetworkable.hpp"
#include "../hooks/v_funk.h"
#include <cstdint>
#include <cmath>
#include <utils\Vector.h>
enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};
class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	// Delete yourself.
	virtual void			Release(void) = 0;
	// Network origin + angles
	virtual const Vector& GetAbsOrigin(void) const = 0;
	virtual const void* GetAbsAngles(void) const = 0;
	virtual void* GetMouth(void) = 0;
	// Retrieve sound spatialization info for the specified sound on this entity
	// Return false to indicate sound is not audible
	//virtual bool			GetSoundSpatialization(SpatializationInfo_t& info) = 0;
	virtual bool			GetSoundSpatialization(void* info) = 0;
	__forceinline const Vector& get_abs_origin() {
		return v_funk::call_func< const Vector& (__thiscall*)(void*) >(this, 10)(this);
	}

	Vector& get_abs_angles()
	{
		typedef Vector& (__thiscall* OriginalFn)(void*);
		return ((OriginalFn)v_funk::call_func<OriginalFn>(this, 11))(this);
	}
};
class CCSWeaponInfo { //xSeeker
public:
	int8_t pad0[20];
	int32_t iMaxClip1;
	int8_t pad1[12];
	int32_t iMaxReservedAmmo;
	int8_t pad2[96];
	char* szHudName;
	char* szWeaponName;
	int8_t pad3[56];
	int32_t iWeaponType;
	int8_t pad4[4];
	int32_t iWeaponPrice;
	int32_t iKillAward;
	int8_t pad5[20];
	uint8_t bFullAuto;
	int8_t pad6[3];
	int32_t iDamage;
	float_t flArmorRatio;
	int32_t iBullets;
	float_t flPenetration;
	int8_t pad7[8];
	float_t flRange;
	float_t flRangeModifier;
	int8_t pad8[16];
	uint8_t bHasSilencer;
	int8_t pad9[15];
	float_t flSpread;
	float_t flSpreadAlt;
	int8_t pad10[76];
	int32_t iRecoilSeed;
	int8_t pad11[32];
};
#pragma pack(pop)
class IWeaponSystem
{
	virtual void unused0() = 0;
	virtual void unused1() = 0;
public:
	virtual CCSWeaponInfo* GetWpnData(unsigned ItemDefinitionIndex) = 0;
};