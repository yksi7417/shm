#include <cassert>
#include <fstream>
#include <cstdio>
#include "shm/mapped_file.h"

int main() {
    const char* path = "test_auto.bin";
    const char* moved = "test_auto.bin.moved";

    // Ensure clean slate
    std::remove(path);
    std::remove(moved);

    // Writer creates the file
    shm::MappedFile writer;
    assert(writer.create(path, 1024));
    char* wptr = static_cast<char*>(writer.data());
    assert(wptr);
    wptr[0] = 'Z';

    // Second reader opens the same file
    shm::MappedFile reader;
    assert(reader.open(path));
    const char* rptr = static_cast<const char*>(reader.data());
    assert(rptr && rptr[0] == 'Z');

    // Close writer - file should still exist
    writer.close();
    std::ifstream check(path);
    assert(check.good());
    check.close();

    // Close final reader - file should be moved
    reader.close();
    assert(!std::ifstream(path).good());
    std::ifstream moved_in(moved);
    char c = 0;
    moved_in.read(&c, 1);
    assert(c == 'Z');
    moved_in.close();

    std::remove(moved);
    return 0;
}
