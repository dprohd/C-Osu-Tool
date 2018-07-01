#pragma once

#include "memory.h"
#include "osu_window.h"

#include <vector>

class cheat
{
public:
	cheat();
	~cheat();

	int get_time() const;
	bool is_audio_playing() const;
	bool is_map_playing() const;
	std::string get_current_song_path() const;
	void update_osu_window() const;

	const memory* get_memory() const { return mem; }
	const osu::window* get_osu_window() const { return osu_window; }

private:
	void get_addresses();
	void get_osu_path();
	void get_songs_path();
	void get_song_files();

	memory* mem;
	DWORD time_address;
	DWORD audio_playing_address;
	std::string osu_path;
	std::string songs_path;
	std::vector<std::string> song_files;
	osu::window* osu_window;
};
