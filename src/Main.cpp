#include <Windows.h>

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <vector>
#include <string>
#include <array>

#include "AES.h"

#include "EncryptedFile.hpp"
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
	std::vector<unsigned char> key = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	// We have to set this console mode to enable ANSI escape sequences
	HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	enable_terminal_escape_sequences(stdout_handle);

	std::vector<EncryptedFile> encrypted_files;

	AES aes = AES(AESKeyLength::AES_128);
	const int chunk_size = 16;
	std::vector<unsigned char> plain = {};
	std::vector<unsigned char> cipher = {};
	for (const auto& entry : std::filesystem::directory_iterator(".\\demo-files"))
	{
		std::fstream file(entry.path(), std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
		int file_length = file.tellg();
		file.seekp(0, std::ios::beg);

		// Read in plaintext from file
		plain.clear();
		plain.reserve(file_length);
		plain.insert(plain.begin(),
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>());
		// Pad out with 0s until `plain` is a multiple of 16
		while (plain.size() % 16 != 0)
			plain.insert(plain.end(), 0);

		// Encrypt
		cipher.clear();
		cipher = aes.EncryptECB(plain, key);

		EncryptedFile encrypted_file(cipher, entry.path().string());
		encrypted_files.insert(encrypted_files.end(), encrypted_file);

		// Write back out to file
		file.clear();
		file.seekp(0, std::ios::beg);
		for (const auto& c : cipher)
			file << c;
		file.close();
	}
	std::cout << "Your files have been encrypted! Guess a word to win them back! " << std::endl;

	int random = randomNum(0, Hackdle::wordlist_length);
	Hackdle hackdle = Hackdle("steep");
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

	for (const auto& encrypted_file : encrypted_files)
	{
		std::fstream file(encrypted_file.path, std::ios::out | std::ios::binary);

		// Decrypt
		plain.clear();
		plain = aes.DecryptECB(encrypted_file.cipher, key);
		// Write back out to file
		file.seekp(0, std::ios::beg);
		for (const auto& c : plain)
			file << c;
		file.close();
	}
	std::string ca = hackdle.get_correct_answer();
	TerminalColor::clear();
	TerminalColor::print(std::format(" {}  {}  {}  {}  {} ", ca[0], ca[1], ca[2], ca[3], ca[4]), TerminalColor::Green);
	std::cout << std::endl << "Correct! Your files have been decrypted." << std::endl;

	CloseHandle(stdout_handle);
	return 0;
}