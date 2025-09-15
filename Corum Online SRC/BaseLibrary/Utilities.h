#pragma once
#include <functional>
#include <string>
#include <sstream>

namespace Utilities {
	template<typename T>
	std::string debugDescriptionMatrix(
		int lines, int columns, std::function<T(int line, int col)> valueAt,
		bool flipHorizontally = false, 
		const char* columnSeparator = "", 
		const char* lineSeparator = "\n",
		bool showNumbering = false) {
		std::stringstream ss; 

		// first line --- column indices

		if(showNumbering) {
			ss << columnSeparator;
			for(int j = 0; j < columns; j++) {
				ss << j << columnSeparator;
			}

			ss << lineSeparator;
		}

		
		for(int i = 0; i < lines; i++) {
			if(showNumbering) {
				ss << i << columnSeparator;
			}

			for(int j = 0; j < columns; j++) {
				auto value = flipHorizontally ? valueAt((lines - 1) - i, j) : valueAt(i, j); 
				ss << value << columnSeparator; 
			}

			ss << lineSeparator;
		}

		return ss.str();
	}
}

