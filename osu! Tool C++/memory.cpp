#include "memory.h"

#include <TlHelp32.h>
#include <Psapi.h>
#include <stdexcept>

memory::memory(const std::string& window_title)
{
	window = FindWindow(NULL, window_title.c_str());

	if (!window)
		throw std::runtime_error("Failed to find window " + window_title);

	GetWindowThreadProcessId(window, &process_id);

	process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, process_id);

	if (!process)
		throw std::runtime_error("Failed to open process (" + GetLastError() + ')');
}

memory::~memory()
{
	CloseHandle(process);
}

DWORD memory::find_pattern(BYTE pattern[]) const
{
	const size_t signature_size = sizeof(pattern);
	const size_t read_size = 4096;
	bool hit = false;

	BYTE chunk[read_size];

	for (size_t i = 0; i < INT_MAX; i += read_size - signature_size)
	{
		ReadProcessMemory(process, LPCVOID(i), &chunk, read_size, NULL);

		for (size_t a = 0; a < read_size; a++)
		{
			hit = true;

			for (size_t j = 0; j < signature_size && hit; j++)
			{
				if (chunk[a + j] != pattern[j])
					hit = false;
			}

			if (hit)
				return i + a;
		}
	}

	throw std::runtime_error("Pattern not found");
}

std::string memory::get_window_title() const
{
	char buffer[256];
	GetWindowText(window, buffer, sizeof buffer);
	return std::string(buffer);
}

bool memory::is_running() const
{
	DWORD exit_code;
	GetExitCodeProcess(process, &exit_code);

	return exit_code == STILL_ACTIVE;
}

std::string memory::get_process_path() const
{
	char buffer[MAX_PATH];
	GetModuleFileNameEx(process, NULL, buffer, sizeof buffer);
	return std::string(buffer);
}
