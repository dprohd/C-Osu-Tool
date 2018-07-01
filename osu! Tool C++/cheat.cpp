#include "cheat.h"
#include "settings.h"
#include "split_string.h"

#include <Lmcons.h>
#include <thread>
#include <fstream>
#include <filesystem>
#include <algorithm>

cheat::cheat()
{
	mem = new memory(settings::osu_window_title);
	osu_window = new osu::window();
	get_osu_path();
	get_songs_path();
	get_song_files();
	get_addresses();
}

cheat::~cheat()
{
	delete mem;
	delete osu_window;
}

int cheat::get_time() const
{
	return mem->read<int>(time_address);
}

bool cheat::is_audio_playing() const
{
	return mem->read<bool>(audio_playing_address);
}

bool cheat::is_map_playing() const
{
	return mem->get_window_title().find(" - ") != std::string::npos;
}

void cheat::get_addresses()
{
	const DWORD pattern = mem->find_pattern(PBYTE("\xDB\x5D\xE8\x8B\x45\xE8\xA3"));

	time_address = mem->read<DWORD>(pattern + 0x7);
	audio_playing_address = time_address + 0x24;
}

std::string cheat::get_current_song_path() const
{
	const std::string window_title = mem->get_window_title();

	const std::string artist_name_sep = " - ";
	std::string song_full = window_title.substr(window_title.find(artist_name_sep) + artist_name_sep.length());
	const std::string chars_to_erase = "<>\"*:\\/?";

	// Some characters are ommited from the actual file name.
	for (size_t i = 0; i < chars_to_erase.length(); i++)
		song_full.erase(std::remove(song_full.begin(), song_full.end(), chars_to_erase[i]), song_full.end());

	const std::string song_artist = song_full.substr(0, song_full.find(artist_name_sep));
	std::string song_name = song_full.substr(song_full.find(artist_name_sep) + artist_name_sep.length());
	const std::string song_difficulty = song_name.substr(song_name.find_last_of('['));
	song_name.erase(song_name.find(" ["));

	for (const auto& i : song_files)
	{
		if (i.find(song_artist) != std::string::npos &&
			i.find(song_name) != std::string::npos &&
			i.find(song_difficulty) != std::string::npos)
			return i;
	}

	throw std::runtime_error("Failed to get current song from window title (" + window_title + ')');
}

void cheat::get_osu_path()
{
	std::string osu_path = mem->get_process_path();
	osu_path.erase(osu_path.find_last_of("\\"));
	this->osu_path = osu_path;
}

void cheat::get_songs_path()
{
	char user_name[UNLEN + 1];
	DWORD user_name_len = sizeof user_name;
	GetUserName(user_name, &user_name_len);

	std::ifstream osu_cfg_file(osu_path + "osu!." + std::string(user_name) + ".cfg");
	std::string songs_path;

	for (std::string i; std::getline(osu_cfg_file, i);)
	{
		if (i.find("BeatmapDirectory") != std::string::npos)
		{
			songs_path = split_string(i, " = ").back();
			break;
		}
	}

	// Is an absolute path
	if (songs_path.find("\\") != std::string::npos)
		this->songs_path = songs_path;
	else
		this->songs_path = osu_path + "\\" + songs_path;
}

void cheat::get_song_files()
{
	for (const auto& i : std::experimental::filesystem::recursive_directory_iterator(songs_path))
	{
		if (i.path().extension() == ".osu")
			song_files.push_back(i.path().string());
	}
}

void cheat::update_osu_window() const
{
	osu_window->update(mem->get_window());
}
