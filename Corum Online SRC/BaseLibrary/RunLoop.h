#pragma once
#include <memory>
#include <functional>
#include <map>
#include "BaseLibrary.h"

class BASELIBRARYDECLSPEC RunLoopSource {
public:
	virtual void runLoopStep() = 0;
};

class BASELIBRARYDECLSPEC RunLoop {
public:
	using SourceHandle = int;

	RunLoop();
	RunLoop(RunLoop const &) = delete;
	RunLoop(RunLoop&&) = delete;
	void operator=(RunLoop const&) = delete;
	static std::shared_ptr<RunLoop> mainRunLoop();

	SourceHandle addSource(RunLoopSource*, unsigned long timeIntervalMilli, bool repeats);
	void removeSource(SourceHandle handle) noexcept;

	void step(unsigned long currentTimeMilliseconds);
private:
	struct SourceContext {
		RunLoopSource* source;
		unsigned long timeIntervalMilli;
		unsigned long lastStepTime;
	};

	int _sourceCount;
	unsigned long _lastStepTime;
	std::map<int, SourceContext> _repeatingSources;
	std::map<int, SourceContext> _oneRunSources;
};

