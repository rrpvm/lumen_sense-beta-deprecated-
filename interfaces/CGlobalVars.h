#pragma once
#define INTERFACEVERSION_PLAYERINFOMANAGER			"PlayerInfoManager002"
class c_globalvar_base
{
public:
    float     realtime;                     // 0x0000
    int       framecount;                   // 0x0004
    float     absoluteframetime;            // 0x0008
    float     absoluteframestarttimestddev; // 0x000C
    float     curtime;                      // 0x0010
    float     frametime;                    // 0x0014
    int       max_clients;                   // 0x0018
    int       tickcount;                    // 0x001C
    float     interval_per_tick;            // 0x0020
    float     interpolation_amount;         // 0x0024
    int       sim_ticks_this_frame;            // 0x0028
    int       network_protocol;             // 0x002C
    void* p_save_data;                    // 0x0030
    bool      m_b_client;                    // 0x0031
    bool      m_b_remoteclient;              // 0x0032

private: 
    int       nTimestampNetworkingBase;
    int       nTimestampRandomizeWindow;
};
/*class IPlayerInfo
{
public:
	// returns the players name (UTF-8 encoded)
	virtual const char* GetName() = 0;
	// returns the userid (slot number)
	virtual int		GetUserID() = 0;
	// returns the string of their network (i.e Steam) ID
	virtual const char* GetNetworkIDString() = 0;
	// returns the team the player is on
	virtual int GetTeamIndex() = 0;
	// changes the player to a new team (if the game dll logic allows it)
	virtual void ChangeTeam(int iTeamNum) = 0;
	// returns the number of kills this player has (exact meaning is mod dependent)
	virtual int	GetFragCount() = 0;
	// returns the number of deaths this player has (exact meaning is mod dependent)
	virtual int	GetDeathCount() = 0;
	// returns if this player slot is actually valid
	virtual bool IsConnected() = 0;
	// returns the armor/health of the player (exact meaning is mod dependent)
	virtual int	GetArmorValue() = 0;

	// extensions added to V2

	// various player flags
	virtual bool IsHLTV() = 0;
	virtual bool IsPlayer() = 0;
	virtual bool IsFakeClient() = 0;
	virtual bool IsDead() = 0;
	virtual bool IsInAVehicle() = 0;
	virtual bool IsObserver() = 0;

	// player position and size
	virtual const Vector GetAbsOrigin() = 0;
	virtual const QAngle GetAbsAngles() = 0;
	virtual const Vector GetPlayerMins() = 0;
	virtual const Vector GetPlayerMaxs() = 0;
	// the name of the weapon currently being carried
	virtual const char* GetWeaponName() = 0;
	// the name of the player model in use
	virtual const char* GetModelName() = 0;
	// current player health
	virtual const int GetHealth() = 0;
	// max health value
	virtual const int GetMaxHealth() = 0;
	// the last user input from this player
	virtual void* GetLastUserCommand() = 0;

	virtual bool IsReplay() = 0;
};
*/

/*
class IPlayerInfoManager
{
public:
    virtual IPlayerInfo * GetPlayerInfo(void * pEdict) = 0;
    virtual CGlobalVarsBase* GetGlobalVars() = 0;
};
class CPlayerInfoManager : public IPlayerInfoManager
{
public:
    virtual IPlayerInfo* GetPlayerInfo(void* pEdict);
    virtual CGlobalVarsBase* GetGlobalVars();
	CGlobalVarsBase* getglobals()
	{
		typedef CGlobalVarsBase* (__thiscall* fnGetGlobalVars)(void* ecx);
		return v_funk::call_func<fnGetGlobalVars>(this, 1)(this);
	}
};*/