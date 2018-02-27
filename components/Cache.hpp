#pragma once
#include <bits.h>
#include <cstdlib>
#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

class Cache
{
	// Cache to store keys
	list<int32_t> keyQueue;

	// Map to store reference of keys in cache
	unordered_map<int32_t, list<int32_t>::iterator> cache;

	// Capacity of cache
	int32_t cacheSize;

public:
	Cache();
	int32_t GetSize();
	void SetSize(int32_t);
	void UpdateReference(int32_t);
	void DisplayCache();
};

Cache::Cache()
{
}

int32_t Cache::GetSize()
{
	return cacheSize;
}

void Cache::SetSize(int32_t size)
{
	if (size > 0)
		cacheSize = size;

}

void Cache::UpdateReference(int32_t target)
{
	// Check if target is present in keyMap
	if (cache.find(target) == cache.end())
	{
		// If true, Cache is full
		if (keyQueue.size() == cacheSize)
		{
			// Delete least recently used element
			int32_t last = keyQueue.back();
			keyQueue.pop_back();
			cache.erase(last);
		}
	}
	else
		// Target is present in cache
		keyQueue.erase(cache[target]);

	// Update target reference in cache
	keyQueue.push_front(target);
	cache[target] = keyQueue.begin();
}

void Cache::DisplayCache()
{
	for (auto iterator = keyQueue.begin(); iterator != keyQueue.end(); iterator++)
	{
		cout << (*iterator) << " ";
	}
	cout << endl;
}