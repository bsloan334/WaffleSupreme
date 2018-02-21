#include <vector>
#include <deque>
#include <string>

//This is word-addressable disk
#define DISK_SIZE = 2048;

typedef byte_t;

class Disk {

private:
    
    
public:
    Disk(int size = DISK_SIZE);
    void allocate();
    
}
