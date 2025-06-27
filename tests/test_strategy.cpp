#include <type_traits>
#include "shm/impl/mapped_file_selector.h"
#ifndef _WIN32
#  include "shm/impl/mapped_file_posix.h"
#else
#  include "shm/impl/mapped_file_windows.h"
#endif

int main() {
#ifdef _WIN32
    static_assert(std::is_same<shm::DefaultMappedFileImpl, shm::WindowsMappedFile>::value,
                  "Windows implementation should be selected on Windows");
#else
    static_assert(std::is_same<shm::DefaultMappedFileImpl, shm::PosixMappedFile>::value,
                  "POSIX implementation should be selected on non-Windows");
#endif
    return 0;
}
