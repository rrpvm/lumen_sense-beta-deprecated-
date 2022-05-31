#include "fake_ping.h"

void fake_ping_manager::flip_state(NetworkChannel* channel)
{
	static auto last_reliable_state = -1;

	if (channel->in_reliable_state != last_reliable_state)
		flipped_state = true;

	last_reliable_state = channel->in_reliable_state;
}
