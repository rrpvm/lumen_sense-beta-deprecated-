#pragma once
#include <cstdint>
#include <utils\Vector.h>
#include "CGlobalVars.h"
#include <hooks\v_funk.h>
#include "ClientClass.hpp"
enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,
	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,
	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};
typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
class IBaseClientDLL
{
public:
	virtual int              Connect(CreateInterfaceFn appSystemFactory, c_globalvar_base* pGlobals) = 0;
	virtual int              Disconnect(void) = 0;
	virtual int              Init(CreateInterfaceFn appSystemFactory, c_globalvar_base* pGlobals) = 0;
	virtual void             PostInit() = 0;
	virtual void             Shutdown(void) = 0;
	virtual void             LevelInitPreEntity(char const* pMapName) = 0;
	virtual void             LevelInitPostEntity() = 0;
	virtual void             LevelShutdown(void) = 0;
	virtual ClientClass* GetAllClasses(void) = 0;
	virtual void			FrameStageNotify(ClientFrameStage_t curStage) = 0;//37(36)
	void CreateMove(int sequence_number, float input_sample_frametime, bool active) // sequence_number of this cmd
	{
		typedef void(__thiscall* CreateMove_t)(int, float, bool);
		return v_funk::call_func<CreateMove_t>(this,22)(sequence_number, input_sample_frametime, active);
	}
	void callInMouseActive() // sequence_number of this cmd
	{
		typedef void(__thiscall* InMouse_t)();
		return v_funk::call_func<InMouse_t>(this, 15)();
	}
	bool DispatchUserMessage(int messageType, int arg, int arg1, void* data)
	{
		using DispatchUserMessage_t = bool* (__thiscall*)(void*, int, int, int, void*);
		return v_funk::call_func< DispatchUserMessage_t>(this, 38)(this, messageType, arg, arg1, data);
	}
};
class CClientState
{
public:
	void ForceFullUpdate() {
		m_nDeltaTick = uint32_t(-1);
	}

	char       pad_0000[156];                 //0x0000
	uint32_t   m_NetChannel;                     //0x009C
	uint32_t   m_nChallengeNr;                   //0x00A0
	char       pad_00A4[100];                 //0x00A4
	uint32_t   m_nSignonState;                   //0x0108
	char       pad_010C[8];                   //0x010C
	float      m_flNextCmdTime;                  //0x0114
	uint32_t   m_nServerCount;                   //0x0118
	uint32_t   m_nCurrentSequence;               //0x011C
	char       pad_0120[8];                   //0x0120
	char       pad_0128[80];                  //0x0128
	uint32_t   m_nDeltaTick;                     //0x0178
	bool       m_bPaused;                        //0x017C
	char       pad_017D[3];                   //0x017D
	uint32_t   m_nViewEntity;                    //0x0180
	uint32_t   m_nPlayerSlot;                    //0x0184
	char       m_szLevelName[260];            //0x0188
	char       m_szLevelNameShort[40];        //0x028C
	char       m_szGroupName[40];             //0x02B4
	char       pad_02DC[52];                  //0x02DC
	uint32_t   m_nMaxClients;                    //0x0310
	char       pad_0314[18820];               //0x0314
	float      m_flLastServerTickTime;           //0x4C98
	bool       m_nInSimulation;                  //0x4C9C
	char       pad_4C9D[3];                   //0x4C9D
	uint32_t   m_nOldTickCount;                  //0x4CA0
	float      m_tickRemainder;                  //0x4CA4
	float      m_frameTime;                      //0x4CA8
	uint32_t   m_nLastOutgoingCommand;           //0x4CAC
	uint32_t   m_nChokedCommands;                //0x4CB0
	uint32_t   m_nLastAcknowledgedCommand;       //0x4CB4
	uint32_t   m_nCommandAcknowledged;           //0x4CB8
	uint32_t   m_nSoundSequence;                 //0x4CBC
	char	   pad_4CC0[80];                  //0x4CC0
	Vector m_vecViewAngles; //0x4D10
};