#pragma once
class v_funk
{
public:
	template <typename T>
	static T call_func(void* _class,int index) {
		int* vTable = *(int**)(_class);
		int dwAdr = vTable[index];
		return (T)dwAdr;
	}
private:
};
#define virtual_f(type, pointer, index, args)  v_funk::call_func<type(__thiscall*)(args)>(pointer, index);
