#include "allinclude.h"

class Allocator {
public:
    Allocator();
    Allocator(int size);
    ~Allocator();
    void* getPtr();
    int getSize();

private:
    int m_size;
    const int INIT_SIZE = 512;
    void* pA;
};
