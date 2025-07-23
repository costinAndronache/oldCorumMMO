#pragma once
#include <functional>
#include <Windows.h>
#include "WorkQueue.h"
#include "RunLoop.h"

class Timer: public RunLoopSource {
public:
	typedef std::function<void()> Callback;
	void launchAfter(DWORD timeoutMilliseconds, bool repeated, Callback callback);
	void cancelCurrentSetup();
	void runLoopStep() override;
	Timer(std::shared_ptr<RunLoop> runLoop) noexcept;
	Timer(Timer& const) = delete;
	Timer(Timer&& const) = delete;
	~Timer();
private:
	std::shared_ptr<RunLoop> _runLoop;
	RunLoop::SourceHandle _runLoopHandle;
	Callback _callback;
};
