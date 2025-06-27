#include "shm/mapped_file.h"

#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <cstring>

namespace shm {

MappedFile::MappedFile() = default;

MappedFile::~MappedFile() { close(); }

bool MappedFile::create(const std::string& path, std::size_t size,
                        GrowthStrategy growth) {
    close();
    growth_ = growth;
#ifdef _WIN32
    file_ = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_ == INVALID_HANDLE_VALUE) return false;
    mapping_ = nullptr;
    size_ = size;
    if (!resize_file(size_)) return false;
    return true;
#else
    fd_ = ::open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd_ < 0) return false;
    size_ = size;
    if (!resize_file(size_)) return false;
    return true;
#endif
}

bool MappedFile::open(const std::string& path, std::size_t size,
                      GrowthStrategy growth) {
    close();
    growth_ = growth;
#ifdef _WIN32
    file_ = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_ == INVALID_HANDLE_VALUE) return false;
    mapping_ = nullptr;
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(reinterpret_cast<HANDLE>(file_), &fileSize)) return false;
    size_ = static_cast<std::size_t>(fileSize.QuadPart);
    if (size > size_) {
        if (!resize_file(size)) return false;
    } else {
        if (!map_file(size_)) return false;
    }
    return true;
#else
    fd_ = ::open(path.c_str(), O_RDWR);
    if (fd_ < 0) return false;
    struct stat st;
    if (fstat(fd_, &st) != 0) return false;
    size_ = static_cast<std::size_t>(st.st_size);
    if (size > size_) {
        if (!resize_file(size)) return false;
    } else {
        if (!map_file(size_)) return false;
    }
    return true;
#endif
}

void MappedFile::close() {
    if (data_) {
        unmap_file();
    }
#ifdef _WIN32
    if (mapping_) {
        CloseHandle(reinterpret_cast<HANDLE>(mapping_));
        mapping_ = nullptr;
    }
    if (file_ && file_ != INVALID_HANDLE_VALUE) {
        CloseHandle(reinterpret_cast<HANDLE>(file_));
        file_ = nullptr;
    }
#else
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
#endif
    size_ = 0;
}

void* MappedFile::data() { return data_; }
const void* MappedFile::data() const { return data_; }
std::size_t MappedFile::size() const { return size_; }

bool MappedFile::ensure_size(std::size_t new_size) {
    if (new_size <= size_) return true;
    std::size_t target = new_size;
    if (growth_ == GrowthStrategy::Double) {
        target = std::max(new_size, size_ * 2);
    }
    return resize_file(target);
}

bool MappedFile::map_file(std::size_t map_size) {
#ifdef _WIN32
    mapping_ = CreateFileMappingA(reinterpret_cast<HANDLE>(file_), nullptr,
                                  PAGE_READWRITE, 0, static_cast<DWORD>(map_size),
                                  nullptr);
    if (!mapping_) return false;
    data_ = MapViewOfFile(reinterpret_cast<HANDLE>(mapping_), FILE_MAP_ALL_ACCESS,
                          0, 0, map_size);
    if (!data_) return false;
#else
    data_ = ::mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
    if (data_ == MAP_FAILED) {
        data_ = nullptr;
        return false;
    }
#endif
    size_ = map_size;
    return true;
}

void MappedFile::unmap_file() {
#ifdef _WIN32
    if (data_) {
        UnmapViewOfFile(data_);
        data_ = nullptr;
    }
#else
    if (data_) {
        ::munmap(data_, size_);
        data_ = nullptr;
    }
#endif
}

bool MappedFile::resize_file(std::size_t new_size) {
    unmap_file();
#ifdef _WIN32
    LARGE_INTEGER li;
    li.QuadPart = static_cast<LONGLONG>(new_size);
    if (SetFilePointerEx(reinterpret_cast<HANDLE>(file_), li, nullptr, FILE_BEGIN) == 0)
        return false;
    if (!SetEndOfFile(reinterpret_cast<HANDLE>(file_))) return false;
    return map_file(new_size);
#else
    if (ftruncate(fd_, new_size) != 0) return false;
    return map_file(new_size);
#endif
}

} // namespace shm

