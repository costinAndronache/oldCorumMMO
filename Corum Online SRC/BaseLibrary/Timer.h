#pragma once
#include <functional>
#include <Windows.h>
#include "WorkQueue.h"

class Timer {
public:
	typedef std::function<void()> Callback;
	void launchAfter(DWORD timeoutMilliseconds, bool repeated, Callback callback);
	void cancelCurrentSetup();
	Timer(std::shared_ptr<WorkQueue> workQueue);
	~Timer();
private:
	std::shared_ptr<WorkQueue> _workQueue;
	PTP_POOL _pool;
	TP_CALLBACK_ENVIRON _threadPoolEnvironment;
	PTP_CLEANUP_GROUP _cleanupGroup;
	PTP_TIMER _currentTimer;
	bool _repeated;
	DWORD _timeoutMilliseconds;
	Callback _currentCallback;

	static void __stdcall timerCallback(PTP_CALLBACK_INSTANCE Instance,
		PVOID Context,
		PTP_TIMER Timer
	);

	void repeatCurrentTimer(DWORD timeoutMilliseconds);
};
