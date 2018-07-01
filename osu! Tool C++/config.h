#pragma once

#include <string>
#include <vector>
#include <memory>

namespace cfg
{
	template<typename T>
	struct key
	{
		key(std::string name, T& value)
		{
			this->name = name;
			this->value = &value;
		}

		std::string name;
		T* value;
	};

	class config
	{
	public:
		config(std::string path, std::string delimiter = " = ");

		void set_key(std::string name, bool& value);
		void set_key(std::string name, char& value);
		void set_key(std::string name, int& value);
		void set_key(std::string name, float& value);
		void set_key(std::string name, double& value);
		void set_key(std::string name, std::string& value);

		void save() const;
		void load() const;

		size_t size() const;

	private:
		void check_file(const std::fstream& file) const;
		auto file_to_pairs(std::fstream& file) const;

		std::string path;
		std::string delimiter;

		std::vector<std::unique_ptr<key<bool>>> b_keys;
		std::vector<std::unique_ptr<key<char>>> c_keys;
		std::vector<std::unique_ptr<key<int>>> i_keys;
		std::vector<std::unique_ptr<key<float>>> f_keys;
		std::vector<std::unique_ptr<key<double>>> d_keys;
		std::vector<std::unique_ptr<key<std::string>>> str_keys;
	};
}
