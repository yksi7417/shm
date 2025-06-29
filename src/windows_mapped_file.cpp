#include "shm/impl/mapped_file_windows.h"

namespace shm {

WindowsMappedFile::WindowsMappedFile() = default;
WindowsMappedFile::~WindowsMappedFile() { close(); }

bool WindowsMappedFile::create(const std::string& path, std::size_t size,
                               GrowthStrategy growth) {
    close();
    growth_ = growth;
    file_ = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_ == INVALID_HANDLE_VALUE) return false;
    mapping_ = nullptr;
    size_ = size;
    if (!resize_file(size_)) return false;
    return true;
}

bool WindowsMappedFile::open(const std::string& path, std::size_t size,
                             GrowthStrategy growth) {
    close();
    growth_ = growth;
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
}

void WindowsMappedFile::close() {
    if (data_) {
        unmap_file();
    }
    if (mapping_) {
        CloseHandle(reinterpret_cast<HANDLE>(mapping_));
        mapping_ = nullptr;
    }
    if (file_ && file_ != INVALID_HANDLE_VALUE) {
        CloseHandle(reinterpret_cast<HANDLE>(file_));
        file_ = nullptr;
    }
    size_ = 0;
}

void* WindowsMappedFile::data() { return data_; }
const void* WindowsMappedFile::data() const { return data_; }
std::size_t WindowsMappedFile::size() const { return size_; }

bool WindowsMappedFile::ensure_size(std::size_t new_size) {
    if (new_size <= size_) return true;
    std::size_t target = new_size;
    if (growth_ == GrowthStrategy::Double) {
        target = std::max(new_size, size_ * 2);
    }
    return resize_file(target);
}

bool WindowsMappedFile::map_file(std::size_t map_size) {
    mapping_ = CreateFileMappingA(reinterpret_cast<HANDLE>(file_), nullptr,
                                  PAGE_READWRITE, 0, static_cast<DWORD>(map_size),
                                  nullptr);
    if (!mapping_) return false;
    data_ = MapViewOfFile(reinterpret_cast<HANDLE>(mapping_), FILE_MAP_ALL_ACCESS,
                          0, 0, map_size);
    if (!data_) return false;
    size_ = map_size;
    return true;
}

void WindowsMappedFile::unmap_file() {
    if (data_) {
        UnmapViewOfFile(data_);
        data_ = nullptr;
    }
}

bool WindowsMappedFile::resize_file(std::size_t new_size) {
    unmap_file();
    LARGE_INTEGER li;
    li.QuadPart = static_cast<LONGLONG>(new_size);
    if (SetFilePointerEx(reinterpret_cast<HANDLE>(file_), li, nullptr, FILE_BEGIN) == 0)
        return false;
    if (!SetEndOfFile(reinterpret_cast<HANDLE>(file_))) return false;
    return map_file(new_size);
}

} // namespace shm

