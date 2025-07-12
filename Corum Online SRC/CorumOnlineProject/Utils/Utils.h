#pragma once

#include <vector>
#include <algorithm>
#include <iterator>

template<typename T>
std::vector<std::vector<T>> splitVector(const std::vector<T>& v, unsigned chunkSize) {
	using Iterator = typename std::vector<T>::const_iterator;
	std::vector<std::vector<T>> rtn;
	Iterator it = v.cbegin();
	const Iterator end = v.cend();

	while (it != end) {
		std::vector<T> v;
		auto inserter = std::back_inserter(v);
		unsigned int num_to_copy = min(
			std::distance(it, end), 
			chunkSize
		);
		std::copy(it, it + num_to_copy, inserter);
		rtn.push_back(std::move(v));
		std::advance(it, num_to_copy);
	}

	return rtn;
}