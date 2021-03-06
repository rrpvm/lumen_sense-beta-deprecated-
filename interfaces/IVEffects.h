#pragma once
#include "../utils/Vector.h"
#include "../hooks/v_funk.h"
#include <Windows.h>
typedef unsigned char byte;
struct ColorRGBExp32
{
	byte r, g, b;
	signed char exponent;
};
struct dlight_t
{
	int		flags;
	Vector	origin;
	float	radius;
	ColorRGBExp32	color;
	float	die;
	float	decay;
	float	minlight;
	int		key;
	int		style;
	Vector	m_Direction;
	float	m_InnerAngle;
	float	m_OuterAngle;
	float GetRadius() const
	{
		return radius;
	}
	float GetRadiusSquared() const
	{
		return radius * radius;
	}
	float IsRadiusGreaterThanZero() const
	{
		return radius > 0.0f;
	}
};

class IVEffects
{
public:
	dlight_t* CL_AllocDlight(int key)
	{
		typedef dlight_t* (__thiscall* OriginalFn)(void* ecx, int);
		return v_funk::call_func<OriginalFn>(this, 4)(this, key);
	}
	dlight_t* CL_AllocElight(int key)
	{
		typedef dlight_t* (__thiscall* OriginalFn)(void* ecx, int);
		return v_funk::call_func<OriginalFn>(this, 5)(this, key);
	}
	dlight_t* GetElightByKey(int key)
	{
		typedef dlight_t* (__thiscall* OriginalFn)(void* ecx, int);
		return v_funk::call_func<OriginalFn>(this, 8)(this, key);
	}
};
