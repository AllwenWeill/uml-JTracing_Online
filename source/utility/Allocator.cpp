#include "Allocator.h"
Allocator::Allocator() :m_size(INIT_SIZE) {
    pA = malloc(m_size);
}
Allocator::Allocator(int size) :m_size(size) {
    pA = malloc(m_size);
}
Allocator::~Allocator() {
    free(pA);
}
void* Allocator::getPtr() {
    return pA;
}
int Allocator::getSize() {
    return m_size;
}