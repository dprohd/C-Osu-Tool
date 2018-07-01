#pragma once

#include <string>
#include <vector>

template<typename T>
static void split_string(const std::string& str, T& tokens, const std::string& delimiters, bool trim_empty = false)
{
	std::string::size_type pos, lastPos = 0;

	using value_type = typename T::value_type;
	using size_type = typename T::size_type;

	while (true)
	{
		pos = str.find_first_of(delimiters, lastPos);

		if (pos == std::string::npos)
		{
			pos = str.length();

			if (pos != lastPos || !trim_empty)
				tokens.push_back(value_type(str.data() + lastPos, (size_type)pos - lastPos));

			break;
		}
		else
		{
			if (pos != lastPos || !trim_empty)
				tokens.push_back(value_type(str.data() + lastPos, (size_type)pos - lastPos));
		}

		lastPos = pos + 1;
	}
}

static std::vector<std::string> split_string(const std::string& str, const std::string& delimiters, bool trim_empty = false)
{
	std::vector<std::string> result;
	split_string(str, result, delimiters, trim_empty);
	return result;
}
