#include <cassert>
#include <fstream>
#include <cstdio>
#include "shm/mapped_file.h"

int main() {
    const char* path = "test_small.bin";
    const char* moved = "test_small.bin.moved";

    std::remove(path);
    std::remove(moved);

    shm::MappedFile mf;
    assert(mf.create(path, 512));

    char* ptr = static_cast<char*>(mf.data());
    assert(ptr);
    ptr[0] = 'X';
    mf.close();

    // File is below threshold so it should not be moved
    assert(std::ifstream(path).good());
    assert(!std::ifstream(moved).good());

    std::remove(path);
    return 0;
}
