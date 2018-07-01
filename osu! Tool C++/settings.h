#pragma once

#include <string>

namespace settings
{
	enum relax_tapping_style
	{
		relax_single,
		relax_alternate
	};

	extern std::string program_window_title;
	extern std::string osu_window_title;
	extern char relax_primary_key;
	extern char relax_secondary_key;
	extern int relax_tapping_style;
	extern int relax_offset;
	extern bool relax_hit_scan;
	extern float relax_hit_scan_circle_offset;
	extern bool randomize_key_timing;
}
