#include "play.h"
#include "settings.h"
#include "osu_game_rules.h"

#include <thread>
#include <iostream>

void play::run(const cheat& c, const osu::beatmap& b, const osu::window& w) const
{
	if (b.get_general().mode != osu::beatmap_mode::standard)
		return;

	size_t object_index = 0;
	bool key_down = false;
	char active_key = settings::relax_primary_key;

	while (object_index < b.get_hit_objects().size())
	{
		if (!c.is_map_playing())
			break;

		const osu::hit_object& object = b.get_hit_objects()[object_index];
		bool waiting = false;

		if (settings::relax_hit_scan && !object.is_spinner())
		{
			c.update_osu_window();

			if (!is_cursor_on_circle(w.get_scaled_object(object.coordinates), b.get_circle_radius() * w.get_multiplier().x, settings::relax_hit_scan_circle_offset))
				waiting = true;
		}

		const int time = c.get_time();
		const int start_time = object.start_time + settings::relax_offset;
		int end_time = object.end_time;

		if (object.is_circle())
			end_time += 2;

		if (time >= start_time && !key_down && !waiting)
		{
			set_key_pressed(active_key, true);
			key_down = true;
		}
		else if (time > end_time && key_down)
		{
			set_key_pressed(active_key, false);
			key_down = false;

			if (settings::relax_tapping_style == settings::relax_alternate)
				active_key = active_key == settings::relax_primary_key ? settings::relax_secondary_key : settings::relax_primary_key;

			object_index++;
		}
		else if (time > end_time + osu::game_rules::get_hit_window_50(b.get_difficulty().overall_difficulty) && waiting)
		{
			// Advance if can't hit a 50 anymore.
			object_index++;
		}

		// Release active key if game is paused to prevent accidental menu clicks.
		if (!c.is_audio_playing() && key_down)
			set_key_pressed(active_key, false);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// Make sure active key is released when done.
	if (key_down)
		set_key_pressed(active_key, false);
}

void play::set_key_pressed(char key, bool pressed) const
{
	INPUT key_press = { 0 };
	key_press.type = INPUT_KEYBOARD;
	key_press.ki.wVk = VkKeyScanEx(key, GetKeyboardLayout(NULL)) & 0xFF;
	key_press.ki.wScan = 0;
	key_press.ki.dwExtraInfo = 0;
	key_press.ki.dwFlags = (pressed ? 0 : KEYEVENTF_KEYUP);
	SendInput(1, &key_press, sizeof INPUT);
}

bool play::is_cursor_on_circle(const vector2& coords, float circle_radius, float offset) const
{
	POINT p;
	GetCursorPos(&p);

	const float distance_x = coords.x - p.x;
	const float distance_y = coords.y - p.y;
	circle_radius -= offset;

	if (distance_x * distance_x + distance_y * distance_y <= circle_radius * circle_radius)
		return true;
	else
		return false;
}
