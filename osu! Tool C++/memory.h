#pragma once

#include <Windows.h>
#include <string>

class memory
{
public:
	memory(const std::string& window_title);
	~memory();

	DWORD find_pattern(BYTE pattern[]) const;
	std::string get_window_title() const;
	bool is_running() const;
	std::string get_process_path() const;

	template<typename T>
	T read(DWORD address) const
	{
		T buffer;

		if (!ReadProcessMemory(process, (LPCVOID)address, &buffer, sizeof T, NULL))
			throw std::runtime_error("Could not read memory address: " + address);

		return buffer;
	}

	DWORD get_process_id() const { return process_id; }
	HANDLE get_process() const { return process; }
	HWND get_window() const { return window; }

private:
	DWORD process_id;
	HANDLE process;
	HWND window;
};
