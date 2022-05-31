#pragma once
#include "../hooks/v_funk.h"
class CPanel {
public:
    const char* GetName(unsigned int vguiPanel)
    {
        typedef const char* (__thiscall* our_call)(void*, unsigned int);
        return v_funk::call_func<our_call>(this, 36)(this, vguiPanel);
    }
};