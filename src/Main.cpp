#include <Windows.h>

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <array>

#include <iostream>
#include <fstream>
#include <stdint.h>

#include <vector>

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
	std::vector<unsigned char> key = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	// We have to set this console mode to enable ANSI escape sequences
	HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	enable_terminal_escape_sequences(stdout_handle);

	int random = randomNum(0, Hackdle::wordlist_length);
	std::vector<EncryptedFile> encrypted_files;

	AES aes = AES(AESKeyLength::AES_128);
	const int chunk_size = 16;
	std::vector<unsigned char> plain = {};
	std::vector<unsigned char> cipher = {};
	for (const auto& entry : std::filesystem::directory_iterator(".\\demo-files"))
	{
		plain.clear();
		std::cout << entry.path() << std::endl;
		std::fstream file(entry.path(), std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
		int file_length = file.tellg();
		file.seekp(0, std::ios::beg);

		std::vector<char> _plain = {};
		_plain.reserve(file_length);
		_plain.insert(_plain.begin(),
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>());

		plain.insert(plain.begin(), _plain.begin(), _plain.end());

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
		std::cout << "Encrypted " + entry.path().string() << std::endl;
	}

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

	for (const auto& encrypted_file : encrypted_files)
	{
		plain.clear();
		std::fstream file(encrypted_file.path, std::ios::out | std::ios::binary);

		// Decrypt
		plain = aes.DecryptECB(encrypted_file.cipher, key);
		// Write back out to file
		file.seekp(0, std::ios::beg);
		for (const auto& c : plain)
			file << c;
		file.close();
		std::cout << "Decrypted " + encrypted_file.path << std::endl;
	}

	std::cout << "Correct!" << std::endl;

	CloseHandle(stdout_handle);
	return 0;
}