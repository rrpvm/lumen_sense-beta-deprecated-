#pragma once
#include "../utils/Vector.h"
#include "CUserCmd.h"
#include "../hooks/v_funk.h"
class CInput
{
public:
	char                pad_0x00[0x0C];
	bool                m_trackir_available;
	bool                m_mouse_initiated;
	bool                m_mouse_active;
	bool                m_fJoystickAdvancedInit;
	char                pad_0x08[0x2C];
	void* m_pKeys;
	char                pad_0x38[0x6C];
	bool                m_fCameraInterceptingMouse;
	bool                m_fCameraInThirdPerson;
	bool                m_fCameraMovingWithMouse;
	Vector		    m_vecCameraOffset;
	bool                m_fCameraDistanceMove;
	int                 m_nCameraOldX;
	int                 m_nCameraOldY;
	int                 m_nCameraX;
	int                 m_nCameraY;
	bool                m_CameraIsOrthographic;
	Vector              m_angPreviousViewAngles;
	Vector              m_angPreviousViewAnglesTilt;
	float               m_flLastForwardMove;
	int                 m_nClearInputState;
	char                pad_0xE4[0x8];
	c_user_cmd* m_pCommands;
	c_verified_cmd* m_pVerifiedCommands;

	inline c_user_cmd* get_user_cmd(int sequence_number);
	inline c_user_cmd* get_user_cmd(int nSlot, int sequence_number);
	inline c_verified_cmd* get_verified_cmd(int sequence_number);
};

c_user_cmd* CInput::get_user_cmd(int sequence_number)
{
	using OriginalFn = c_user_cmd * (__thiscall*)(void*, int, int);
	return v_funk::call_func<OriginalFn>(this, 8)(this, 0, sequence_number);
}

c_user_cmd* CInput::get_user_cmd(int nSlot, int sequence_number)
{
	typedef c_user_cmd* (__thiscall* GetUserCmd_t)(void*, int, int);
	return v_funk::call_func<GetUserCmd_t>(this, 8)(this, nSlot, sequence_number);
}

c_verified_cmd* CInput::get_verified_cmd(int sequence_number)
{
	auto verifiedCommands = *(c_verified_cmd**)(reinterpret_cast<uint32_t>(this) + 0xF8);
	return &verifiedCommands[sequence_number % 150];
}
