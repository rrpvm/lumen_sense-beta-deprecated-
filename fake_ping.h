#pragma once
#include <interfaces\IVEngine.hpp>
class fake_ping_manager {
public:
	void flip_state(NetworkChannel* channel);
private:
	bool in_ping_spike{}, flipped_state{};
};
inline fake_ping_manager ping_manager;