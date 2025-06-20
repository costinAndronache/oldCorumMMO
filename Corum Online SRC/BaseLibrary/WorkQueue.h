#pragma once
#include <functional>
#include <mutex>
#include <queue>
#include <memory>

class WorkQueue {
public:
	typedef std::function<void()> WorkItem;
	void enqueueWorkItem(WorkItem work);
	void processCurrentWorkItems();

	static std::shared_ptr<WorkQueue> mainThreadQueue();

private:
	static std::shared_ptr<WorkQueue> _mainThreadQueue;
	std::mutex _mutex;
	std::queue<WorkItem> _workQueue;

};

