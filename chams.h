#pragma once
#include "interfaces/IMaterialSystem.hpp"
#include "interfaces/IVModelRender.hpp"
#include "utils/Color.h"
#include <string>
#include "utils/utils.h"
class chams
{
public:
    void init_materials();
    void overrideMat(IMaterial* mat, bool ignoreZ, bool wireFrame, Color color) noexcept;
    void overrideMat(IMaterial* mat, bool ignoreZ, bool wireFrame) noexcept;
    IMaterial* create_material(std::string name, bool wireframe, bool ignorez, std::string data, KeyValues* val = nullptr);
    void do_players_chams( void*ecx,int edx,void* ctx, const DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
    void do_weapon_chams();
    void do_arm_chams( void* ctx, const DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
    void do_sleeves_chams();
    IMaterial* get_material_by_cfg_material( int var);
    bool should_material_envmap_override(int type);
    IMaterial* player_cur_mat = nullptr;
    IMaterial* hand_cur_mat = nullptr;
    IMaterial* sleeves_cur_mat = nullptr;
    IMaterial* weapon_cur_mat = nullptr;
public:
     IMaterial* default_mat = nullptr;
     IMaterial* default_mat_pulse = nullptr;
     IMaterial* metallic_mat = nullptr;
     IMaterial* pearlescent_mat = nullptr;
     IMaterial* glass = nullptr;
     IMaterial* crystal = nullptr;
     IMaterial* velvet = nullptr;
     IMaterial* glow = nullptr;
     IMaterial* weapon_plastik = nullptr;
     IMaterial* weapon_blink = nullptr;
     IMaterial* weapon_gold = nullptr;
     IMaterial* glow_armrace = nullptr;
     IMaterial* invis_material = nullptr;
     IMaterial* reflectivity_mat = nullptr;
     IMaterial* glow_second = nullptr; 
     IMaterial* sound_esp_mat = nullptr; 


     KeyValues* glow_key = new KeyValues();
     float old_blend[3];

     IMaterialVar* materials_setting[4][10];
};
inline chams chams_;