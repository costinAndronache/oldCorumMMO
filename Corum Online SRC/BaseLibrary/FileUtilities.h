#pragma once
#include "BaseLibrary.h"
#include <vector>
#include <string>

class BASELIBRARYDECLSPEC  FileUtilities {
public:
	static std::vector<uint8_t> readBytes(const wchar_t* filePath);
	static std::string readStringFromFile(const wchar_t* filePath);
};

