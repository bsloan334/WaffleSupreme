#pragma once
#include <iostream>
#include <thread>

enum MutexStates{ FREE, LOCK };


class Mutex {

	public:
		Mutex() : mutex(FREE){};		// By default, mutex is FREE
		Mutex(int st) : mutex(st){};

		void SetState(int st) { mutex = st; }
		int GetState() { return mutex; }

		int TestLock() { return mutex; }
		void SetLock() { while (mutex == LOCK); mutex = LOCK; }

		int TestAndSet()
			/*Returns the previous state of the lock.
			  Application: If the lock is FREE and set to LOCK, 
			  function returns FREE, indicating that the lock was
			  set by the thread waiting for the resource. */
		{ 
			int prevState = mutex;
			if (mutex == FREE) 
				mutex = LOCK;
			return prevState;
		}

		void ReleaseLock() { mutex = FREE; }

	private:
		int mutex;
};