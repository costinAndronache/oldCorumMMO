#include "Timer.h"


Timer::Timer(std::shared_ptr<RunLoop> runLoop) noexcept {
    _runLoop = runLoop;
    _callback = nullptr;
}

Timer::~Timer() {
    _runLoop->removeSource(_runLoopHandle);
}

void Timer::cancelCurrentSetup() {
    _runLoop->removeSource(_runLoopHandle);
}

void Timer::launchAfter(DWORD timeoutMilliseconds, bool repeated, Callback callback) {
    cancelCurrentSetup();
    _callback = callback;

    _runLoopHandle = _runLoop->addSource(this, timeoutMilliseconds, repeated);
}

void Timer::runLoopStep() {
    if (_callback) { _callback(); }
}