#pragma once
#include "../includes.hpp"
#include "VMatrix.h"
#include "singleton.h"
#include <filesystem>
#include <fstream>
#include <string>
class Address {
protected:

	uintptr_t m_ptr;

public:

	// default c/dtor
	__forceinline Address() : m_ptr{} { };
	__forceinline ~Address() { };

	__forceinline Address(uintptr_t a) :
		m_ptr{ a } { }

	__forceinline Address(const void* a) :
		m_ptr{ (uintptr_t)a } { }

	__forceinline operator uintptr_t() {
		return m_ptr;
	}

	__forceinline operator void* () {
		return (void*)m_ptr;
	}

	__forceinline operator const void* () {
		return (const void*)m_ptr;
	}

	// to is like as but dereferences.
	template< typename t = Address >
	__forceinline t to() const {
		return *(t*)m_ptr;
	}

	template< typename t = Address >
	__forceinline t as() const {
		return (t)m_ptr;
	}

	template< typename t = Address >
	__forceinline t at(ptrdiff_t offset) const {
		return *(t*)(m_ptr + offset);
	}

	template< typename t = Address >
	__forceinline t add(ptrdiff_t offset) const {
		return (t)(m_ptr + offset);
	}

	template< typename t = Address >
	__forceinline t sub(ptrdiff_t offset) const {
		return (t)(m_ptr - offset);
	}

	template< typename t = Address >
	__forceinline t get(size_t dereferences = 1) {
		return (t)get_(dereferences);
	}

	template< typename t = Address >
	__forceinline void set(t val) {
		*(t*)m_ptr = val;
	}

	template< typename t = Address >
	__forceinline t rel(size_t offset = 0) {
		uintptr_t out;
		uint32_t rel;

		out = m_ptr + offset;

		rel = *(uint32_t*)out;
		if (!rel)
			return t{};

		out = (out + 0x4) + rel;

		return (t)out;
	}

	__forceinline static bool safe(Address to_check) {
		static MEMORY_BASIC_INFORMATION32 mbi{};

		if (!to_check
			|| to_check < 0x10000
			|| to_check > 0xFFE00000
			|| !VirtualQuery(to_check, (PMEMORY_BASIC_INFORMATION)&mbi, sizeof(mbi)))
			return false;

		if (!mbi.AllocationBase
			|| mbi.State != MEM_COMMIT
			|| mbi.Protect == PAGE_NOACCESS
			|| mbi.Protect & PAGE_GUARD)
			return false;

		return true;
	}

private:
	__forceinline uintptr_t get_(size_t dereferences) {
		uintptr_t temp = m_ptr;

		while (dereferences-- && safe(temp))
			temp = *(uintptr_t*)temp;

		return temp;
	}
};
#ifndef CONCAT_IMPL
#define CONCAT_IMPL(x, y) x##y
#ifndef MACRO_CONCAT
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#ifndef PAD
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE];
#endif
#endif
#endif
class KeyValues {
public:
	PAD(32)
};
class utils : public singleton <utils>
{
public:
	template <typename t = Address>
	__forceinline static t get_method(Address this_ptr, size_t index) {
		return (t)this_ptr.to< t* >()[index];
	}
	std::uint8_t* PatternScan(void* module, const char* signature);
	unsigned long pattern_scan(const char* module_name, const char* signature);
	static DWORD pattern_mask_scan(const char* module_n, const BYTE* mask, const char* mask_string);
	static void* getInterface(const char* dllname, const char* interfacename);
	static void* getInterfaceStatic(const char* dllname, const char* interfacename);
	static void init_key_values(KeyValues* keyvalues, const char* key_name);
	static void load_from_buffer(KeyValues* key_values, const char* resource_name, const char* buf, void* file_sys = nullptr, const char* path_id = nullptr, void* eval_sym_proc = nullptr, void* unk = nullptr);
	//vector3
	Vector calculate_aim_angles(Vector to_hit, Vector my_angles);
    static bool screen_transform(const Vector& in, Vector& out);
	static float    random_between_two_float(float min, float max);
    //--------------------------------------------------------------------------------
	 bool WorldToScreen(const Vector& in, Vector& out);
	static void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
	{
		out[0] = in1.Dot(in2[0]) + in2[0][3];
		out[1] = in1.Dot(in2[1]) + in2[1][3];
		out[2] = in1.Dot(in2[2]) + in2[2][3];
	}
	bool IntersectionBoundingBox(const Vector& src, const Vector& dir, const Vector& min, const Vector& max, Vector* hit_point)
	{
		/*
		Fast Ray-Box Intersection
		by Andrew Woo
		from "Graphics Gems", Academic Press, 1990
		*/

		constexpr int NUMDIM = 3;
		constexpr int RIGHT = 0;
		constexpr int LEFT = 1;
		constexpr int MIDDLE = 2;

		bool inside = true;
		char quadrant[NUMDIM];
		int i;

		// Rind candidate planes; this loop can be avoided if
		// rays cast all from the eye(assume perpsective view)
		Vector candidatePlane;
		for (i = 0; i < NUMDIM; i++)
		{
			if (src[i] < min[i])
			{
				quadrant[i] = LEFT;
				candidatePlane[i] = min[i];
				inside = false;
			}
			else if (src[i] > max[i])
			{
				quadrant[i] = RIGHT;
				candidatePlane[i] = max[i];
				inside = false;
			}
			else
			{
				quadrant[i] = MIDDLE;
			}
		}

		// Ray origin inside bounding box
		if (inside)
		{
			if (hit_point)
				*hit_point = src;
			return true;
		}

		// Calculate T distances to candidate planes
		Vector maxT;
		for (i = 0; i < NUMDIM; i++)
		{
			if (quadrant[i] != MIDDLE && dir[i] != 0.f)
				maxT[i] = (candidatePlane[i] - src[i]) / dir[i];
			else
				maxT[i] = -1.f;
		}

		// Get largest of the maxT's for final choice of intersection
		int whichPlane = 0;
		for (i = 1; i < NUMDIM; i++)
		{
			if (maxT[whichPlane] < maxT[i])
				whichPlane = i;
		}

		// Check final candidate actually inside box
		if (maxT[whichPlane] < 0.f)
			return false;

		for (i = 0; i < NUMDIM; i++)
		{
			if (whichPlane != i)
			{
				float temp = src[i] + maxT[whichPlane] * dir[i];
				if (temp < min[i] || temp > max[i])
				{
					return false;
				}
				else if (hit_point)
				{
					(*hit_point)[i] = temp;
				}
			}
			else if (hit_point)
			{
				(*hit_point)[i] = candidatePlane[i];
			}
		}

		// ray hits box
		return true;
	}
	bool precache_model(const char* szModelName);
	static void __CreateFont(HMODULE module, LPCSTR resource, std::string name);
	float lerp();
	bool valid_backtrack(float simtime);
private:
};

