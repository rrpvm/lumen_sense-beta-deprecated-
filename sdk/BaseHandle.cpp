#include "CBaseHandle.hpp"
#include "sdk.hpp"
IHandleEntity* CBaseHandle::Get() const
{
    return g_entity_list->GetClientEntityFromHandle(*this);
}
