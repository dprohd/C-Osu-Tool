#include "settGen.h"

bool settExist(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}

std::string genRandom(size_t length)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

void crtSett()
{
	std::cout << "Settings.cfgx does not exist.\n" << std::endl;
	std::cout << "Creating Settings.cfgx...\n" << std::endl;

	std::ofstream settCFG;
	settCFG.open("Settings.cfgx");
	settCFG << "program_window_title = " << genRandom(10) << std::endl;
	settCFG << "relax_primary_key = z" << std::endl;
	settCFG << "relax_secondary_key = x" << std::endl;
	settCFG << "relax_tapping_style = 0" << std::endl;
	settCFG << "relax_offset = -25" << std::endl;
	settCFG << "relax_hit_scan_circle_offset = 0" << std::endl;
	settCFG << "relax_hit_scan = true" << std::endl;
	settCFG << "osu_window_title = osu!" << std::endl;
	settCFG << "randomize_key_timing = 0" << std::endl;
	settCFG.close();

	std::cout << "Settings.cfgx has been created. Please restart the program.\n" << std::endl;
}