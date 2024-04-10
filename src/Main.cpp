#include <Windows.h>

#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>

#include "AES.h"

#include "TerminalColor.hpp"
#include "Hackdle.hpp"

void enable_terminal_escape_sequences(HANDLE stdout_handle) {
	DWORD console_mode;
	GetConsoleMode(stdout_handle, &console_mode);
	console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(stdout_handle, console_mode);
}

int randomNum(int min, int max)
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

int main() {
	// We have to set this console mode to enable ANSI escape sequences
	HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	enable_terminal_escape_sequences(stdout_handle);

	int random = randomNum(0, Hackdle::wordlist_length);
	Hackdle hackdle = Hackdle(Hackdle::wordlist[random]);
	std::string guess;
	while (!hackdle.is_complete()) {
		std::getline(std::cin, guess);
		std::transform(guess.begin(), guess.end(), guess.begin(), ::tolower);
		try {
			hackdle.guess(guess);
		}
		catch (Hackdle::Error e) {
			switch (e) {
				case Hackdle::Error::GuessTooShort:
					std::cout << "Error: guess too short...stupid" << std::endl;
					continue;
				case Hackdle::Error::GuessTooLong:
					std::cout << "Error: guess too long...learn how to count to five" << std::endl;
					continue;
				case Hackdle::Error::InvalidCharacter:
					std::cout << "Error: ngl found an invalid character bro" << std::endl;
					continue;
				case Hackdle::Error::NotInWordList:
					std::cout << "Error: guess not in word list...get yourself a dictonary and stop being illiterate " << std::endl;
					continue;
			}
		}
		TerminalColor::clear();
		hackdle.print();
	}
	std::cout << "Correct!" << std::endl;

	CloseHandle(stdout_handle);
	return 0;
}