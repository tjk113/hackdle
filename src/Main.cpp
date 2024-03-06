#include <Windows.h>

#include <cstring>
#include <algorithm>

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

	Hackdle hackdle = Hackdle("prank");
	std::string guess;
	while (!hackdle.is_complete()) {
		std::getline(std::cin, guess);
		std::transform(guess.begin(), guess.end(), guess.begin(), ::toupper);
		hackdle.guess(guess);
		hackdle.print();

	}

	std::cout << std::endl;

	CloseHandle(stdout_handle);
	return 0;
}