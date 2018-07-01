#include "osu_beatmap.h"
#include "split_string.h"
#include "osu_game_rules.h"

#include <fstream>
#include <algorithm>

namespace osu
{
	enum beatmap_sections
	{
		section_general,
		section_editor,
		section_metadata,
		section_difficulty,
		section_events,
		section_timing_points,
		section_colours,
		section_hit_objects
	};

	beatmap::beatmap(const std::string& path, bool ez, bool hr)
	{
		std::ifstream file(path);

		if (!file)
			throw std::runtime_error("Could not open beatmap " + path);

		int section_i = -1;
		bool found_ar = false;
		float last_ms_per_beat;

		for (std::string line; std::getline(file, line);)
		{
			if (line.empty())
				continue;

			// Is a section name
			if (line.front() == '[' && line.back() == ']')
			{
				if (line == "[General]")
					section_i = section_general;
				else if (line == "[Editor]")
					section_i = section_editor;
				else if (line == "[Metadata]")
					section_i = section_metadata;
				else if (line == "[Difficulty]")
					section_i = section_difficulty;
				else if (line == "[Events]")
					section_i = section_events;
				else if (line == "[TimingPoints]")
					section_i = section_timing_points;
				else if (line == "[Colours]")
					section_i = section_colours;
				else if (line == "[HitObjects]")
					section_i = section_hit_objects;

				continue;
			}

			switch (section_i)
			{
			case section_general:
				parse_general(line);
				break;
			case section_difficulty:
				parse_difficulty(line, ez, hr, found_ar);
				break;
			case section_timing_points:
				parse_timing_points(line, last_ms_per_beat);
				break;
			case section_hit_objects:
				parse_hit_objects(line);
				break;
			default:
				break;
			}
		}

		// Older beatmaps don't have AR, it's the same as OD.
		if (!found_ar)
			difficulty.approach_rate = difficulty.overall_difficulty;

		calculate_stacking();
	}

	beatmap_timing_point beatmap::get_timing_point_from_offset(float offset) const
	{
		for (size_t i = timing_points.size(); i-- > 0;)
		{
			if (timing_points.at(i).offset <= offset)
				return timing_points.at(i);
		}

		return timing_points.front();
	}

	void beatmap::parse_general(const std::string& line)
	{
		const auto split_line = split_string(line, ": ");

		if (split_line.front() == "Mode")
			general.mode = static_cast<beatmap_mode>(std::stoi(split_line.back()));
		else if (split_line.front() == "StackLeniency")
			general.stack_leniency = std::stof(split_line.back());
	}

	void beatmap::parse_difficulty(const std::string& line, bool ez, bool hr, bool& found_ar)
	{
		const auto split_line = split_string(line, ":");

		if (split_line.front() == "HPDrainRate")
		{
			difficulty.hp_drain_rate = std::stof(split_line.back());

			if (ez) difficulty.hp_drain_rate /= 2;
			else if (hr) difficulty.hp_drain_rate *= 1.4f;

			difficulty.hp_drain_rate = std::min(difficulty.hp_drain_rate, 10.0f);
		}
		else if (split_line.front() == "CircleSize")
		{
			difficulty.circle_size = std::stof(split_line.back());

			if (ez) difficulty.circle_size /= 2;
			else if (hr) difficulty.circle_size *= 1.3f;

			difficulty.circle_size = std::min(difficulty.circle_size, 10.0f);
			circle_radius = (beatmap_playfield.x / 16.0f) * (1.0f - 0.7f * (difficulty.circle_size - 5.0f) / 5.0f);
			circle_diameter = circle_radius * 2.0f;
		}
		else if (split_line.front() == "OverallDifficulty")
		{
			difficulty.overall_difficulty = std::stof(split_line.back());

			if (ez) difficulty.overall_difficulty /= 2;
			else if (hr) difficulty.overall_difficulty *= 1.4f;

			difficulty.overall_difficulty = std::min(difficulty.overall_difficulty, 10.0f);
		}
		else if (split_line.front() == "ApproachRate")
		{
			difficulty.approach_rate = std::stof(split_line.back());

			if (ez) difficulty.approach_rate /= 2;
			else if (hr) difficulty.approach_rate *= 1.4f;

			difficulty.approach_rate = std::min(difficulty.approach_rate, 10.0f);
			found_ar = true;
		}
		else if (split_line.front() == "SliderMultiplier")
			difficulty.slider_multiplier = std::stof(split_line.back());
		else if (split_line.front() == "SliderTickRate")
			difficulty.slider_tick_rate = std::stof(split_line.back());
	}

	void beatmap::parse_timing_points(const std::string& line, float& last_ms_per_beat)
	{
		const auto split_line = split_string(line, ",");

		beatmap_timing_point timing_point;
		timing_point.offset = std::stof(split_line[0]);
		timing_point.ms_per_beat = std::stof(split_line[1]);
		timing_point.meter = std::stoi(split_line[2]);
		timing_point.velocity = 1;

		if (timing_point.ms_per_beat < 0)
		{
			timing_point.velocity = std::abs(100 / timing_point.ms_per_beat);

			if (last_ms_per_beat != 0)
				timing_point.ms_per_beat = last_ms_per_beat;
		}
		else
			last_ms_per_beat = timing_point.ms_per_beat;

		timing_points.push_back(timing_point);
	}

	void beatmap::parse_hit_objects(const std::string& line)
	{
		const auto split_line = split_string(line, ",");

		hit_object object;
		object.coordinates.x = static_cast<float>(std::stoi(split_line[0]));
		object.coordinates.y = static_cast<float>(std::stoi(split_line[1]));
		object.start_time = std::stoi(split_line[2]);
		object.type = std::stoi(split_line[3]);

		if (object.is_circle())
			object.end_time = object.start_time;
		else if (object.is_slider())
		{
			const auto split_slider = split_string(split_line[5], "|");
			object.slider_type = static_cast<hit_object_slider_type>(split_slider[0].front());

			for (size_t i = 1; i < split_slider.size(); i++)
			{
				const auto split_coords = split_string(split_slider[i], ":");

				vector2 coords;
				coords.x = static_cast<float>(std::stoi(split_coords.front()));
				coords.y = static_cast<float>(std::stoi(split_coords.back()));

				object.slider_curve.push_back(coords);
			}

			object.slider_repeat = std::stoi(split_line[6]);
			object.slider_pixel_length = std::stof(split_line[7]);

			const beatmap_timing_point timing_point = get_timing_point_from_offset(static_cast<float>(object.start_time));
			const float px_per_beat = difficulty.slider_multiplier * 100 * timing_point.velocity;
			const float beats_num = (object.slider_pixel_length * object.slider_repeat) / px_per_beat;
			object.end_time = object.start_time + static_cast<int>(std::ceil(beats_num * timing_point.ms_per_beat));
		}
		else if (object.is_spinner())
			object.end_time = std::stoi(split_line[5]);

		hit_objects.push_back(object);
	}

	void beatmap::calculate_stacking()
	{
		// https://gist.github.com/peppy/1167470
	}
}
