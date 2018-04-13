# pragma once 
#include <cstdio>
#include <memory>
#include <mutex>

template <class T>
class CircularBuffer
{
public:
	// circularBuffer template allows for declaration with any data type
	// sizet size: number of elements in queue
	// Note regarding size: 
	// This implementation uses an addition spare byte to check for fullness
	CircularBuffer(int32_t s) :
		size(s)
	{
		// empty constructor
	}
private:
	std::mutex mutex_;
	std::unique_ptr<T[]> buf_;
	int32_t head = 0;
	int32_t tail = 0;
	int32_t size;

	void reset(void)
	{
	std:lock_guard<std::mutex> lock(mutex_);
		head = tail;
	}

	bool empty(void)
	{
		// if head = tail, queue is empty
		return head == tail;
	}

	bool full(void)
	{
		// if tail is ahead of head by 1, queue is full
		return ((head + 1) & size) == tail;
	}
	void setSize(int32_t bufLength)
	{
	std:lock_guard<std::mutex> lock(mutex_);
		size = bufLength;
	}

	int32_t getSize(void)
	{
		// - 1 to account for "reserved entry" for checking fullness
		return size - 1;
	}

	void put(T item)
	{
	std:lock_guard<std::mutex> lock(mutex_);

		buf_[head] = item;
		head = (head + 1) % size;

		if (head == tail)
		{
			tail = (tail + 1) % size;
		}
	}

	T get(void)
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (empty())
		{
			return T();
		}

		// Read data and advance the tail to free space
		auto val = buf_[tail];
		tail = (tail + 1) % size;

		return val;
	}
};