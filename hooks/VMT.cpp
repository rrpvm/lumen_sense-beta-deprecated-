#include "VMT.h"

void VMT::init(void* base_class)
{
	hooked_table = (uintptr_t**)(base_class);
	ovftable = *hooked_table;
	methods = 0;
	while (reinterpret_cast<uintptr_t**>(*hooked_table)[methods])
	{
		++methods;
	}
	new_table = new uintptr_t[sizeof(uintptr_t) * methods];
	memcpy(new_table, *hooked_table, sizeof(uintptr_t) * methods);//or ovftable
}

void VMT::hook(void* function, int virtual_index)
{
	if (hooked_table != nullptr)
	{
		DWORD protect;
		VirtualProtect(hooked_table, sizeof(uintptr_t) * methods, PAGE_EXECUTE_READWRITE, &protect);
		new_table[virtual_index] = reinterpret_cast<uintptr_t>(function);
		*hooked_table = new_table;
		VirtualProtect(hooked_table, sizeof(uintptr_t) * methods, protect, &protect);
	}
}

void VMT::unhook()
{
	if (ovftable)
	{
		DWORD protect;
		VirtualProtect(hooked_table, sizeof(uintptr_t) * methods, PAGE_EXECUTE_READWRITE, &protect);
		*hooked_table = ovftable;
		VirtualProtect(hooked_table, sizeof(uintptr_t) * methods, protect, &protect);
		ovftable = nullptr;//20/08/2020 updated
		new_table = nullptr;
		methods = 0;		
	}
}
