#include "config.h"

#include <fstream>

namespace cfg
{
	config::config(std::string path, std::string delimiter)
	{
		this->path = path;
		this->delimiter = delimiter;
	}

	void config::set_key(std::string name, bool& value)
	{
		b_keys.push_back(std::make_unique<key<bool>>(name, value));
	}

	void config::set_key(std::string name, char& value)
	{
		c_keys.push_back(std::make_unique<key<char>>(name, value));
	}

	void config::set_key(std::string name, int& value)
	{
		i_keys.push_back(std::make_unique<key<int>>(name, value));
	}

	void config::set_key(std::string name, float& value)
	{
		f_keys.push_back(std::make_unique<key<float>>(name, value));
	}

	void config::set_key(std::string name, double& value)
	{
		d_keys.push_back(std::make_unique<key<double>>(name, value));
	}

	void config::set_key(std::string name, std::string& value)
	{
		str_keys.push_back(std::make_unique<key<std::string>>(name, value));
	}

	void config::save() const
	{
		std::fstream file(path, std::ios::out);
		check_file(file);

		for (const auto& i : b_keys)
			file << i->name << delimiter << (*i->value ? "true" : "false") << std::endl;

		for (const auto& i : c_keys)
			file << i->name << delimiter << *i->value << std::endl;

		for (const auto& i : i_keys)
			file << i->name << delimiter << *i->value << std::endl;

		for (const auto& i : f_keys)
			file << i->name << delimiter << *i->value << std::endl;

		for (const auto& i : d_keys)
			file << i->name << delimiter << *i->value << std::endl;

		for (const auto& i : str_keys)
			file << i->name << delimiter << *i->value << std::endl;
	}

	auto config::file_to_pairs(std::fstream& file) const
	{
		std::vector<std::pair<std::string, std::string>> result;

		for (std::string line; std::getline(file, line);)
		{
			const size_t delimiter_pos = line.find(delimiter);

			if (delimiter_pos == std::string::npos)
				continue;

			const std::string name = line.substr(0, delimiter_pos);
			const std::string value = line.substr(delimiter_pos + delimiter.length());

			result.emplace_back(name, value);
		}

		return result;
	}

	void config::load() const
	{
		std::fstream file(path, std::ios::in);
		check_file(file);

		const auto pairs = file_to_pairs(file);

		if (pairs.size() != size())
			throw std::runtime_error("Failed to find all config keys");

		size_t keys_parsed = 0;

		for (const auto& i : b_keys)
		{
			i->name = pairs[keys_parsed].first;
			*i->value = pairs[keys_parsed].second == "true" ? true : false;
			keys_parsed++;
		}

		for (const auto& i : c_keys)
		{
			i->name = pairs[keys_parsed].first;
			*i->value = pairs[keys_parsed].second.front();
			keys_parsed++;
		}

		for (const auto& i : i_keys)
		{
			i->name = pairs[keys_parsed].first;
			*i->value = std::stoi(pairs[keys_parsed].second);
			keys_parsed++;
		}

		for (const auto& i : f_keys)
		{
			i->name = pairs[keys_parsed].first;
			*i->value = std::stof(pairs[keys_parsed].second);
			keys_parsed++;
		}

		for (const auto& i : d_keys)
		{
			i->name = pairs[keys_parsed].first;
			*i->value = std::stod(pairs[keys_parsed].second);
			keys_parsed++;
		}

		for (const auto& i : str_keys)
		{
			i->name = pairs[keys_parsed].first;
			*i->value = pairs[keys_parsed].second;
			keys_parsed++;
		}
	}

	size_t config::size() const
	{
		return b_keys.size() + c_keys.size() + i_keys.size() + f_keys.size() + d_keys.size() + str_keys.size();
	}

	void config::check_file(const std::fstream& file) const
	{
		if (!file.is_open())
			throw std::runtime_error("File could not be opened (" + path + ')');
	}
}
