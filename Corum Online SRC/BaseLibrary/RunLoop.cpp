#include "RunLoop.h"
#include <vector>

RunLoop::RunLoop() { 
	_sourceCount = 0;
}


std::shared_ptr<RunLoop> RunLoop::mainRunLoop() {
	static std::shared_ptr<RunLoop> _mainLoop = nullptr;
	if (!_mainLoop) { _mainLoop = std::make_shared<RunLoop>(); }
	return _mainLoop;
}

RunLoop::SourceHandle RunLoop::addSource(RunLoopSource* source, unsigned long timeIntervalMilli, bool repeats) {
	if (!source) { return -1; }
	_sourceCount += 1;

	auto context = SourceContext{ source, timeIntervalMilli, _lastStepTime };

	if (repeats) {
		_repeatingSources.insert({ _sourceCount, context });
	} else {
		_oneRunSources.insert({ { _sourceCount, context } });
	}

	return _sourceCount;
}

void RunLoop::removeSource(SourceHandle handle) noexcept {
	_repeatingSources.erase(handle);
	_oneRunSources.erase(handle);
}

void RunLoop::step(unsigned long currentTimeMilli) {
	std::vector<int> toBeRemoved;

	for (auto& s : _oneRunSources) {
		if (s.second.lastStepTime + s.second.timeIntervalMilli <= currentTimeMilli) {
			toBeRemoved.push_back(s.first);
			s.second.source->runLoopStep();
		}
	}

	for (auto& s : _repeatingSources) {
		if (s.second.lastStepTime + s.second.timeIntervalMilli <= currentTimeMilli) {
			s.second.lastStepTime = currentTimeMilli;
			s.second.source->runLoopStep();
		}
	}

	for (auto i : toBeRemoved) {
		_oneRunSources.erase(i);
	}

	_lastStepTime = currentTimeMilli;
}