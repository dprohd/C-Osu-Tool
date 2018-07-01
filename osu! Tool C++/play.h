#pragma once

#include "cheat.h"
#include "osu_beatmap.h"
#include "osu_window.h"
#include "vector2.h"

class play
{
public:
	void run(const cheat& c, const osu::beatmap& b, const osu::window& w) const;

private:
	void set_key_pressed(char key, bool pressed) const;
	bool is_cursor_on_circle(const vector2& coords, float circle_radius, float offset = 0) const;
};
