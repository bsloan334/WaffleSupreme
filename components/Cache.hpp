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
	Cache()
	{
		
	}
	Cache(int size) :
		cacheSize(size)
	{

	}
	int GetSize();
	void SetSize(int);
	void UpdateReference(int);
	void DisplayCache();
};

