#pragma once

#include "vector2.h"

#include <vector>

namespace osu
{
	enum hit_object_type
	{
		hit_object_type_circle = 1,
		hit_object_type_slider = 2,
		hit_object_type_spinner = 8
	};

	enum class hit_object_slider_type
	{
		linear = 'L',
		perfect = 'P',
		bezier = 'B',
		catmull = 'C'
	};

	class hit_object
	{
	public:
		vector2 coordinates;
		int start_time;
		int end_time;
		int type;

		hit_object_slider_type slider_type;
		std::vector<vector2> slider_curve;
		int slider_repeat;
		float slider_pixel_length;

		bool is_circle() const;
		bool is_slider() const;
		bool is_spinner() const;
		int get_duration() const;
	};
}
