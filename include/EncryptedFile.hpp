#pragma once

#include <vector>
#include <string>
struct EncryptedFile {
	std::vector<unsigned char>cipher;
		std::string path;
		EncryptedFile(std::vector<unsigned char>cipher, std::string path);
};