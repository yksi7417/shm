#ifndef SHM_MAPPED_FILE_SELECTOR_H
#define SHM_MAPPED_FILE_SELECTOR_H

#ifdef _WIN32
#include "shm/impl/mapped_file_windows.h"
namespace shm {
using DefaultMappedFileImpl = WindowsMappedFile;
}
#else
#include "shm/impl/mapped_file_posix.h"
namespace shm {
using DefaultMappedFileImpl = PosixMappedFile;
}
#endif

#endif // SHM_MAPPED_FILE_SELECTOR_H
