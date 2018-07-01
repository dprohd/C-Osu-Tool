#include "osu_hit_object.h"

namespace osu
{
	bool hit_object::is_circle() const
	{
		return (type & hit_object_type_circle) == hit_object_type_circle;
	}

	bool hit_object::is_slider() const
	{
		return (type & hit_object_type_slider) == hit_object_type_slider;
	}

	bool hit_object::is_spinner() const
	{
		return (type & hit_object_type_spinner) == hit_object_type_spinner;
	}

	int hit_object::get_duration() const
	{
		return end_time - start_time;
	}
}
