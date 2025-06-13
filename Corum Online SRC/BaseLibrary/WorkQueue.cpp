#include "WorkQueue.h"

std::shared_ptr<WorkQueue> WorkQueue::_mainThreadQueue(nullptr);

std::shared_ptr<WorkQueue> WorkQueue::mainThreadQueue() {
	if (!_mainThreadQueue) {
		_mainThreadQueue = std::make_shared<WorkQueue>();
	}

	return _mainThreadQueue;
}

void WorkQueue::enqueueWorkItem(WorkItem work) {
	std::lock_guard<std::mutex> guard(_mutex);
	_workQueue.push(work);
}

void WorkQueue::processCurrentWorkItems() {
	std::lock_guard<std::mutex> guard(_mutex);
	while (!_workQueue.empty()) {
		const auto& work = _workQueue.front();
		work();
		_workQueue.pop();
	}
}