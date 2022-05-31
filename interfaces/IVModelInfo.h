#pragma once
#include "IVModelRender.hpp"
#include "IMaterialSystem.hpp"
class IVModelInfo
{
public:
	virtual	~IVModelInfo() = default;
	virtual const model_t* get_model(int modelindex) const = 0;
	virtual int	get_model_index(const char* name) const = 0;
	virtual const char* get_model_name(const model_t * model) const = 0;


	void get_model_materials(const model_t* mod, const int count, IMaterial** out)
	{
		typedef void(__thiscall* o_model)(void*ecx,const model_t* mod, const int count, IMaterial** out);
		return v_funk::call_func<o_model>(this, 19)(this,mod, count, out);
	}
	studiohdr_t* get_studio_model(const model_t* mod)
	{
		typedef studiohdr_t*(__thiscall* o_model)(void*ecx, const model_t* mod);
		return v_funk::call_func<o_model>(this, 32)(this,mod);
	}
	const model_t* FindOrLoadModel(const char* name) {
		typedef model_t* (__thiscall* o_FindOrLoadModel)(void* ecx, const char* name);
		return v_funk::call_func<o_FindOrLoadModel>(this, 43)(this, name);
	}
};
