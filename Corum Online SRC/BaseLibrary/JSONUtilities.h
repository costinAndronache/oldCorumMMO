#pragma once
#include "BaseLibrary.h"
#include "libraries/json.hpp"
#include <set>

class BASELIBRARYDECLSPEC JSONUtilities {
public:
	static std::set<std::string> allMissingValues(
		const nlohmann::json& json,
		std::set<std::string>& keys
	); 
};

