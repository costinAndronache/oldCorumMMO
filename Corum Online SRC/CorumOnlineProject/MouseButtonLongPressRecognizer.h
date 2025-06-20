#pragma once
#include <functional>
#include <map>
#include "../BaseLibrary/Timer.h"

class MouseButtonLongPressRecognizer {
public:
	typedef unsigned long long EventListenerHandle;
	typedef std::function<void()> EventListener;
	struct EventListeners {
		const EventListener onDetect;
		const EventListener onEnd;
	};

	MouseButtonLongPressRecognizer(std::shared_ptr<WorkQueue> workQueue, DWORD minimumPressDurationMilliseconds);
	EventListenerHandle addLeftButtonListeners(EventListeners listeners);
	EventListenerHandle addRightButtonListeners(EventListeners listeners);

	void removeListeners(EventListenerHandle handle);

	void handleRightButtonPress();
	void handleRightButtonRelease();
	void handleLeftButtonPress();
	void handleLeftButtonRelease();

private:
	std::map<EventListenerHandle, EventListeners> _leftLongPressListeners;
	std::map<EventListenerHandle, EventListeners> _rightLongPressListeners;
	DWORD _minimumPressDurationMilliseconds;

	EventListenerHandle _internalCount;

	bool _leftButtonInPressState;
	bool _rightButtonInPressState;

	bool _leftLongPressDetected;
	bool _rightLongPressDetected;

	Timer _leftButtonLongPressTimer;
	Timer _rightButtonLongPresstimer;
};