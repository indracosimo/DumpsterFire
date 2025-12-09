#include "MessageQueue.h"

MessageQueue g_messageQueue;

void MessageQueue::push(std::unique_ptr<MessageManager> message)
{
	std::lock_guard<std::mutex> lock(mutex);
	queue.push(std::move(message));
}

void MessageQueue::processMessage()
{
	std::lock_guard<std::mutex> lock(mutex);
	if (!queue.empty())
	{
		auto message = std::move(queue.front());
		queue.pop();
		message->processMessage();
	}
}
