#include <vector>
#include <deque>
#include <string>

//This is word-addressable disk
#define DISK_SIZE = 2048;

typedef byte_t;

class Disk {

private:
    int used = 0;

    
public:
    Disk(int size = DISK_SIZE);
    void allocate();

}
