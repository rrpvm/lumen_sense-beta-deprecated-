#pragma once
#include "../sdk/IHandleEntity.hpp"
#include "../utils/Vector.h"
class IClientNetworkable;
class IClientEntity;
class IClientRenderable;
class IClientThinkable;
class ICollideable
{
public:
	virtual void pad0();
	virtual const Vector& OBBMins() const;
	virtual const Vector& OBBMaxs() const;
};
class IClientUnknown : public IHandleEntity
{
public:
	//virtual ICollideable*		GetCollideable() = 0;
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	//virtual IClientRenderable*	GetClientRenderable() = 0;
	virtual void* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual void* GetBaseEntity() = 0;
	//virtual IClientThinkable*	GetClientThinkable() = 0;
	virtual void* GetClientThinkable() = 0;
};