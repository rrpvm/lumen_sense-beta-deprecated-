#pragma once
#include <Windows.h>

class VMT
{
public:
	VMT() {};
	~VMT() {};
	void init(void* base_class);
	void hook(void* function, int virtual_index);
	void unhook();
	template <typename T>
	T get_original(int index);
	uintptr_t* ovftable = nullptr;
	uintptr_t* new_table = nullptr;
private:
	int methods = 0;
	uintptr_t** hooked_table = nullptr;
	
};

template<typename T>
inline T VMT::get_original(int index)
{
	if (ovftable && ovftable[index])
	{
		return (T)ovftable[index];
	}
	return 0;
}
