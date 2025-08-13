// CorumPreferences.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "CorumPreferences.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using namespace CorumPreferences;
using namespace nlohmann;


// not sure why the json functions dont accept named const char* arguments
static auto windowMode = json::json_pointer("/windowMode");
static auto resWidth = "resWidth";
static auto resHeight = "resHeight";

Preferences::Preferences() {
	_resolution = { 1024, 768 };
	_windowMode = true;
}

Preferences Preferences::defaults() { return Preferences(); }

Preferences* Preferences::buildFromData(const Data& data) {
	auto jsonString = std::string(std::begin(data), std::end(data));

	json fromJSON = json::parse(jsonString, nullptr, false);

	if(fromJSON.is_discarded()) { return nullptr;}

	auto* result = new Preferences();
	auto def = defaults();

	result->storeWindowMode(fromJSON.value<bool>("windowMode", def.windowMode()));
	result->storeResolution({
		fromJSON.value<int>("resWidth", def.resolution().width),
		fromJSON.value<int>("resHeight", def.resolution().height)
	});
	return result;
}

Preferences* Preferences::buildFromFile(const char* filePath) {
	std::ifstream f(filePath, std::ios::binary);
	if(!f.good()) { return nullptr;}

	f.seekg(0, std::ios::end);
	size_t length = f.tellg();
	f.seekg(0, std::ios::beg);

	char* buffer = (char*)malloc(length);

	f.read(buffer, length);

	std::vector<uint8_t> data(buffer, buffer + length);

	free(buffer);

	return buildFromData(data);
}

Data Preferences::serialize() const {
	json serialized {
		{ "windowMode", _windowMode }, 
		{ "resWidth", _resolution.width }, 
		{ "resHeight", _resolution.height}
	};

	auto toString = to_string(serialized);
	return Data(std::begin(toString), std::end(toString));
}

void Preferences::serializeToFile(const char* filePath) const {
	std::ofstream o(filePath, std::ios::binary);

	auto data = serialize();
	o.write((char*)data.data(), data.size());
}