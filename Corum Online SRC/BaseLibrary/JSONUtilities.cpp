#include "JSONUtilities.h"
#include <algorithm>

using namespace nlohmann;

std::set<std::string> JSONUtilities::allMissingValues(
	const nlohmann::json& root,
	std::set<std::string>& keys
) {
	std::set<std::string> result;
	auto resultInserter = std::inserter(result, result.begin());

	std::copy_if(
		std::begin(keys),
		std::end(keys),
		resultInserter,
		[=](std::string key) {
			return (root.find(key) == root.end());
		}
	);

	return result;
} 