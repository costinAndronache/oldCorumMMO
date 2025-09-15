#include "FileUtilities.h"
#include <fstream>

std::vector<uint8_t> FileUtilities::readBytes(const wchar_t* filePath) {
	std::ifstream f(filePath, std::ios::binary);
	if(!f.good()) { return {}; }

	f.seekg(0, std::ios::end);
	size_t length = f.tellg();
	f.seekg(0, std::ios::beg);

	char* buffer = (char*)malloc(length);

	f.read(buffer, length);

	std::vector<uint8_t> data(buffer, buffer + length);

	free(buffer);

	return data;
}

std::string FileUtilities::readStringFromFile(const wchar_t* filePath) {
	auto data = readBytes(filePath);
	return std::string(std::begin(data), std::end(data));
}