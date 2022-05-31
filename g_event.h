#pragma once
#include "interfaces/game_event_manager.h"
#include "sdk/csgo_struct.h"
#include <vector>
class game_event_handler : i_game_event_listener {
	struct hurt_data {
		Vector position;
		c_base_player* hurted_player;
		int dmg;
		float hit_time;
	};
public:
	game_event_handler() {};
	void FireGameEvent(i_game_event* event);
	int  GetEventDebugID = 42;
	void Init();
	void shutdown();
	void on_paint();
	std::vector<hurt_data>hit_data[64];
private:
	Vector last_firebullet_position;
	Vector move_hit_damage_vector_data[64];
	//std::vector<hurt_data>hit_data;
};
inline game_event_handler g_event_handler;