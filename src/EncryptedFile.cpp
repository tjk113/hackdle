#include "EncryptedFile.hpp"

EncryptedFile::EncryptedFile(std::vector<unsigned char>cipher, std::string path) :
	cipher(cipher), path(path) {}
