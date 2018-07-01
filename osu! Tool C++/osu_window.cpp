#include "osu_window.h"
#include "osu_beatmap.h"

#include <thread>
#include <algorithm>

#ifdef min
#undef min
#endif

namespace osu
{
	void window::update(HWND window)
	{
		RECT rect;
		GetClientRect(window, &rect);

		int x = std::min(static_cast<int>(rect.right), GetSystemMetrics(SM_CXSCREEN));
		int y = std::min(static_cast<int>(rect.bottom), GetSystemMetrics(SM_CYSCREEN));

		int swidth = x;
		int sheight = y;

		if (swidth * 3 > sheight * 4)
			swidth = sheight * 4 / 3;
		else
			sheight = swidth * 3 / 4;

		multiplier.x = swidth / 640.0f;
		multiplier.y = sheight / 480.0f;

		int offset_x = static_cast<int>(x - beatmap_playfield.x * multiplier.x) / 2;
		int offset_y = static_cast<int>(y - beatmap_playfield.y * multiplier.y) / 2;

		POINT p;
		p.x = 1;
		p.y = static_cast<int>(8.0f * multiplier.y);
		ClientToScreen(window, &p);

		window_position.x = static_cast<float>(p.x + offset_x);
		window_position.y = static_cast<float>(p.y + offset_y);
	}

	vector2 window::get_scaled_object(const vector2& coords) const
	{
		return coords * multiplier + window_position;
	}
}
