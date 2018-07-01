#pragma once

#include "vector2.h"

#include <Windows.h>

namespace osu
{
	class window
	{
	public:
		void update(HWND window);
		vector2 get_scaled_object(const vector2& coords) const;

		const vector2& get_multiplier() const { return multiplier; }
		const vector2& get_window_position() const { return window_position; }

	private:
		vector2 multiplier;
		vector2 window_position;
	};
}
