#pragma once
#include <iostream>

enum Mutex{
   FREE = 0,
   LOCK = 1
};
class Mutex {
   
public: 
   void SetLock() {while (mutex == LOCK) {} mutex = LOCK;}
   void ReleaseLock() {mutex = FREE;}
}