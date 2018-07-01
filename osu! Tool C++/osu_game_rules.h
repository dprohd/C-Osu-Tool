#pragma once

#include "osu_score.h"

namespace osu
{
	class game_rules
	{
	public:
		static float get_hit_window_300(float od);
		static float get_hit_window_100(float od);
		static float get_hit_window_50(float od);
		static float get_hit_window_miss();
		static score::hit get_hit_result(float delta, float od);
		static float get_approach_time(float ar);

	private:
		static float map_difficulty_range(float scaled_diff, float min, float mid, float max);
	};
}
