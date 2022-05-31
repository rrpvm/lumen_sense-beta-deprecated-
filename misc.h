#pragma once
#include"interfaces/CInput.h"
#include "../LumenSense/utils/VMatrix.h"
class misc
{
public:
	misc() {};
	~misc() {};
	void gravity_walk(c_user_cmd* cmd);
	void bunny_hop(c_user_cmd* cmd);
	void fake_lags(bool& bSendPacket);
	void radar_hack();
	void slow_walk(c_user_cmd* cmd);
	void chat_spam();
	matrix3x4_t last_net_matrix[128];
	Vector last_net_origin;
private:
};
inline misc g_misc;