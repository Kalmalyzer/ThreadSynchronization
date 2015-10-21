

#include <condition_variable>
#include <queue>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

struct Item
{
	Item(int value_) : value(value_) {}

	int value;
};

mutex itemQueueMutex; // This must be locked before reading/writing itemQueue
condition_variable itemAddedToQueue; // This will be signalled whenever a new item is added to itemQueue
queue<Item> itemQueue;


void enqueue_item(Item item)
{
	// Add an item to queue
	lock_guard<mutex> lock(itemQueueMutex);
	itemQueue.push(item);

	// If anyone is waiting on new items to be added, then wake up one of the waiting threads
	itemAddedToQueue.notify_one();
}

Item dequeue_item()
{
	// Wait until there is at least one item in the queue
	// (the itemQueueMutex will be temporarily unlocked and locked again during the wait operation)
	unique_lock<mutex> lock(itemQueueMutex);
	while (itemQueue.empty())
		itemAddedToQueue.wait(lock);

	// There is now at least one item in the queue, and the lock is being held by this thread

	// Dequeue item
	Item item = itemQueue.front();
	itemQueue.pop();
	return item;
}


void produce_numbers(int start, int end)
{
	for (int i = start; i < end; i++)
		enqueue_item(Item(i));
}

void consume_numbers()
{
	while (1)
	{
		Item item = dequeue_item();
		if (item.value == 0)
			break;
		else
		{
			cout << item.value << endl;
		}
	}
}

int main()
{
	thread producer_thread(produce_numbers, 1, 11);
	thread consumer_thread(consume_numbers);

	producer_thread.join();

	// Signal consumer thread that it is time to exit
	enqueue_item(Item(0));
	consumer_thread.join();

	return 0;
}
