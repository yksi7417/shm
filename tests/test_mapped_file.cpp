#include <cassert>
#include <fstream>
#include <string>
#include "shm/mapped_file.h"

int main() {
    shm::MappedFile mf;
    const char* path = "test_mmap.bin";
    if (!mf.create(path, 4096)) {
        return 1;
    }
    char* ptr = static_cast<char*>(mf.data());
    if (!ptr) {
        return 1;
    }
    ptr[0] = 'A';
    mf.close();

    std::string moved_path = std::string(path) + ".moved";
    std::ifstream in(moved_path, std::ios::binary);
    char c = 0;
    in.read(&c, 1);
    assert(c == 'A');
    in.close();
    std::remove(moved_path.c_str());
    return 0;
}
