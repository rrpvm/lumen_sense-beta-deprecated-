#pragma once
#include "../utils/singleton.h"
#include "../utils/Color.h"
class hooks :
    public singleton <hooks>
{
public:
    hooks() {};
    ~hooks() {};
    void attachHooks();
    void do_unhook();
   
    bool chams = true;
    bool chams2 = true;
    bool box_esp = true;
    bool draw_patricles = false;
    bool draw_patricles_passive = false;
    DWORD key = 0x0;
    int slider_var = 20;
    Color c = Color(255,255,0,0);
    int slider_var2 = 20;
    int curr = 0;
};

