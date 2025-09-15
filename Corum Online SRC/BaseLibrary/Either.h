#pragma once

#include <functional>
#include <memory>

template<typename T, typename U>
struct Either {
	Either(std::shared_ptr<T> first) { 
		_first = first; _isFirst = true; 
	}
	
	Either(std::shared_ptr<U> second) { 
		_second = second; _isFirst = false; 
	}
	
	Either(const Either& other) {
		_isFirst = other._isFirst;
		_first = other._first;
		_second = other._second;
	}

	template<typename R>
	R sswitch(
		std::function<R(std::shared_ptr<T>)> fnT, 
		std::function<R(std::shared_ptr<U>)> fnU
	) const {
		if(_isFirst) {
			return fnT(_first);
		} else {
			return fnU(_second);
		}
	}

private:
	std::shared_ptr<T> _first;
	std::shared_ptr<U> _second;

	bool _isFirst;
};