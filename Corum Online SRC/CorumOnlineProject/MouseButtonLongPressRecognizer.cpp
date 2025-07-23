#include "MouseButtonLongPressRecognizer.h"

MouseButtonLongPressRecognizer::MouseButtonLongPressRecognizer(std::shared_ptr<RunLoop> runLoop, DWORD minimumPressDurationMilliseconds):
 _leftButtonLongPressTimer(runLoop), 
 _rightButtonLongPresstimer(runLoop), 
 _minimumPressDurationMilliseconds(minimumPressDurationMilliseconds) {

}

MouseButtonLongPressRecognizer::EventListenerHandle MouseButtonLongPressRecognizer::addLeftButtonListeners(EventListeners listeners) {
	const auto id = _internalCount++;
	_leftLongPressListeners.insert({ id, listeners });
	return id;
}

MouseButtonLongPressRecognizer::EventListenerHandle MouseButtonLongPressRecognizer::addRightButtonListeners(EventListeners listeners) {
	const auto id = _internalCount++;
	_rightLongPressListeners.insert({ id, listeners });
	return id;
}

void MouseButtonLongPressRecognizer::removeListeners(EventListenerHandle handle) {
	_rightLongPressListeners.erase(handle);
	_leftLongPressListeners.erase(handle);
}

void MouseButtonLongPressRecognizer::handleRightButtonPress() {
	_rightButtonInPressState = true;
	_rightLongPressDetected = false;
	_rightButtonLongPresstimer.launchAfter(_minimumPressDurationMilliseconds, false, [this]() {
		if (this->_rightButtonInPressState) {
			this->_rightLongPressDetected = true;
			
			for (const auto& item : this->_rightLongPressListeners) {
				item.second.onDetect();
			}
		}
	});
}

void MouseButtonLongPressRecognizer::handleRightButtonRelease() {
	_rightButtonInPressState = false;
	_rightButtonLongPresstimer.cancelCurrentSetup();

	if (_rightLongPressDetected) {
		for (const auto& item : this->_rightLongPressListeners) {
			item.second.onEnd();
		}
	}

	_rightLongPressDetected = false;
}

void MouseButtonLongPressRecognizer::handleLeftButtonPress() {
	_leftButtonInPressState = true;
	_leftLongPressDetected = false;

	_leftButtonLongPressTimer.launchAfter(_minimumPressDurationMilliseconds, false, [this]() {
		if (this->_leftButtonInPressState) {
			this->_leftLongPressDetected = true;

			for (const auto& item : this->_leftLongPressListeners) {
				item.second.onDetect();
			}
		}
	});
}

void MouseButtonLongPressRecognizer::handleLeftButtonRelease() {
	_leftButtonInPressState = false;
	_leftButtonLongPressTimer.cancelCurrentSetup();

	if(_leftLongPressDetected) {
		for (const auto& item : _leftLongPressListeners) {
			item.second.onEnd();
		}
	}
	_leftLongPressDetected = false;
}