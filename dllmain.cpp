#include "includes.hpp"
#include "sdk/sdk.hpp"
#include "hooks/hook_manager.hpp"
#include "backend/cfg_system.hpp"
DWORD  WINAPI start_main_routine(PVOID hModule)
{
    //cfg.Setup();
    sdk::get().create_interfaces();
    hook_manager_.hook_all();
    cfg.cfg_list = cfg.get_cfg_list();
    cfg.Setup();
    Beep(100, 100);
    while (!GetAsyncKeyState(VK_DELETE))
    {
        Sleep(700);
    }
       hook_manager_.unhook_all();
    FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 1);

}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    DisableThreadLibraryCalls(hModule);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, start_main_routine, hModule, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}




