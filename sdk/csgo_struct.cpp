#include "csgo_struct.h"
#include "csgo.h"
struct netvar_table;
void netvar_manager::init()
{
	data.clear();
	client_class_all = g_base_client_dll->GetAllClasses();
	for (ClientClass* cl = client_class_all; cl != nullptr; cl = cl->m_pNext)
	{
		if (cl->m_pRecvTable)
		{			
			data.emplace_back(load_table(cl->m_pRecvTable));
		}
	}
}

uint32_t netvar_manager::get_offset(const std::string& tableName, const std::string& propName)
{
    auto result = 0u;
    for ( netvar_table& table : data) {
        if (table.name == tableName) {
            result = get_offset(table, propName);
            if (result != 0)
                return result;
        }
    }
    return 0;
}

uint32_t netvar_manager::get_offset(netvar_table& table, const std::string& propName)
{
    for (const auto& prop : table.child_props) {
        if (strncmp(prop->m_pVarName, propName.data(), propName.size()) == 0) {
            return table.offset + prop->m_Offset;
        }
    }
    for (netvar_table& child : table.child_tables) {
        auto prop_offset = get_offset(child, propName);
        if (prop_offset != 0)
            return table.offset + prop_offset;
    }
    for (const auto& child : table.child_tables) {
        if (strncmp(child.prop->m_pVarName, propName.data(), propName.size()) == 0) {
            return table.offset + child.offset;
        }
    }
    return 0;
}

netvar_table netvar_manager::load_table(RecvTable* recvTable)
{
    netvar_table table = netvar_table{}; 
    table.offset = 0;
    table.name = recvTable->m_pNetTableName;

    for (auto i = 0; i < recvTable->m_nProps; ++i) {
        auto prop = &recvTable->m_pProps[i];

        if (!prop || isdigit(prop->m_pVarName[0]))
            continue;
        if (strcmp("baseclass", prop->m_pVarName) == 0)
            continue;

        if (prop->m_RecvType == 6 && prop->m_pDataTable) {
            table.child_tables.emplace_back(load_table(prop->m_pDataTable));
            table.child_tables.back().offset = prop->m_Offset;
            table.child_tables.back().prop = prop;
        }
        else {
            if(prop)table.child_props.emplace_back(prop);
        }
    }
    return table;
}

bool c_base_player::is_enemy()
{   
        return csgo_data.local->get_team() != this->get_team();  
}

bool c_base_player::can_see(c_base_player* player, Vector hitbox)
{
    if (!csgo_data.local || !player)return false;
    CGameTrace tr;
    Ray_t ray;
    CTraceFilter filter;
    filter.pSkip = this;
    Vector endpos = hitbox;
    Vector origin = this->get_eye_position_f();
    ray.Init(origin, endpos);
    g_trace_gay->TraceRay(ray, MASK_PLAYERSOLID, &filter, &tr);
    return tr.hit_entity == player && tr.fraction > 0.96f;
}

int& c_base_player::get_think_tick()
{
    return *(int*)((uintptr_t)(this) + 0x40);
}

void c_base_player::think()
{
    typedef void(__thiscall* oThink)(void*);
    v_funk::call_func<oThink>(this, 138)(this);
}

void c_base_player::pre_think()
{
    typedef void(__thiscall* pre_think)(void*);
    v_funk::call_func<pre_think>(this, 315)(this);
}

void c_base_player::run_think()
{
    static auto fn = reinterpret_cast<void(__thiscall*)(int)>(utils::get().PatternScan(GetModuleHandle("client.dll"), "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B C6"));

    if (get_think_tick() != -1 && get_think_tick() > 0
        && get_think_tick() < time_to_ticks(g_global_vars->curtime))
    {
        get_think_tick() = -1;
        fn(0);
        think();
    }
}

void c_base_player::run_pre_think()
{
    static auto fn = reinterpret_cast<bool(__thiscall*)(void*, int)>(utils::get().PatternScan(GetModuleHandle("client.dll"), "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87"));

    if (fn(this, 0))
        pre_think();
}


