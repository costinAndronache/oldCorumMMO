#include "Timer.h"

#define MILLISECONDS_TO_100NANOINTERVAL_TICKS(x) (ULONGLONG)x * 10000

static FILETIME filetimeForTimer(DWORD timeoutMilliseconds) {
    FILETIME FileDueTime;
    ULARGE_INTEGER ulDueTime;

    ulDueTime.QuadPart = (ULONGLONG)-(MILLISECONDS_TO_100NANOINTERVAL_TICKS(timeoutMilliseconds));
    FileDueTime.dwHighDateTime = ulDueTime.HighPart;
    FileDueTime.dwLowDateTime = ulDueTime.LowPart;

    return FileDueTime;
}

void __stdcall Timer::timerCallback(PTP_CALLBACK_INSTANCE, PVOID Context, PTP_TIMER) {
    Timer* tthis = static_cast<Timer*>(Context);
    tthis->_workQueue->enqueueWorkItem(tthis->_currentCallback);

    if (tthis->_repeated) {
        tthis->repeatCurrentTimer(tthis->_timeoutMilliseconds);
    }
}


Timer::Timer(std::shared_ptr<WorkQueue> workQueue) {
    _workQueue = workQueue;
    InitializeThreadpoolEnvironment(&_threadPoolEnvironment);
    _pool = CreateThreadpool(NULL);
    _currentTimer = nullptr;

    if (NULL == _pool) {
        printf("CreateThreadpool failed. LastError: %u\n", GetLastError());
        return;
    }

    //
    // The thread pool is made persistent simply by setting
    // both the minimum and maximum threads to 1.
    //
    SetThreadpoolThreadMaximum(_pool, 1);
    if (FALSE == SetThreadpoolThreadMinimum(_pool, 1)) {
        printf(("SetThreadpoolThreadMinimum failed. LastError: %u\n"), GetLastError());
        return;
    }

    _cleanupGroup = CreateThreadpoolCleanupGroup();

    if (NULL == _cleanupGroup) {
        printf(("CreateThreadpoolCleanupGroup failed. LastError: %u\n"), GetLastError());
        return;
    }


    SetThreadpoolCallbackPool(&_threadPoolEnvironment, _pool);
    SetThreadpoolCallbackCleanupGroup(&_threadPoolEnvironment, _cleanupGroup, NULL);
}

Timer::~Timer() {
    if (_cleanupGroup) {
        CloseThreadpoolCleanupGroupMembers(_cleanupGroup, FALSE, NULL);
        CloseThreadpoolCleanupGroup(_cleanupGroup);
    }
}

void Timer::cancelCurrentSetup() {
    if (_currentTimer) {
        _currentCallback = []() {};
        CloseThreadpoolTimer(_currentTimer);
        _currentTimer = nullptr;
    }
    _repeated = false;
    _timeoutMilliseconds = 0;
}

void Timer::launchAfter(DWORD timeoutMilliseconds, bool repeated, Callback callback) {
    cancelCurrentSetup();

    _currentCallback = callback;
    _repeated = repeated;
    _timeoutMilliseconds = timeoutMilliseconds;

    const auto timer = CreateThreadpoolTimer(Timer::timerCallback, this, &_threadPoolEnvironment);
    if (NULL == timer) {
        printf(("CreateThreadpoolTimer failed. LastError: %u\n"),
            GetLastError());
        return;
    }

    auto FileDueTime = filetimeForTimer(timeoutMilliseconds);
    SetThreadpoolTimer(timer, &FileDueTime, 0, 0);
    _currentTimer = timer;
}

void Timer::repeatCurrentTimer(DWORD timeoutMilliseconds) {
    if (!_currentTimer) { return;  }

    auto FileDueTime = filetimeForTimer(timeoutMilliseconds);
    SetThreadpoolTimer(_currentTimer, &FileDueTime, 0, 0);
}