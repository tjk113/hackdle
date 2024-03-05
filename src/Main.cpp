#include <Windows.h>

#include <iostream>
#include <string>

#include "TerminalColor.hpp"
#include "Hackdle.hpp"

void enable_terminal_escape_sequences(HANDLE stdout_handle) {
	DWORD console_mode;
	GetConsoleMode(stdout_handle, &console_mode);
	console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(stdout_handle, console_mode);
}

int main() {
	// We have to set this console mode to enable ANSI escape sequences
	HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	enable_terminal_escape_sequences(stdout_handle);

	std::string words[] = {"crane", "guide", "spite"};

	std::string guess;

	TerminalColor::print(" H ", TerminalColor::Black, true);
	TerminalColor::print(" e ", TerminalColor::Black, true);
	TerminalColor::print(" l ", TerminalColor::Yellow);
	TerminalColor::print(" l ", TerminalColor::Black, true);
	TerminalColor::print(" o ", TerminalColor::Green);
	std::cout << std::endl;

	CloseHandle(stdout_handle);
	return 0;
}