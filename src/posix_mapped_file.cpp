#include "shm/impl/mapped_file_posix.h"

namespace shm {

PosixMappedFile::PosixMappedFile() = default;
PosixMappedFile::~PosixMappedFile() { close(); }

bool PosixMappedFile::create(const std::string& path, std::size_t size,
                             GrowthStrategy growth) {
    close();
    growth_ = growth;
    fd_ = ::open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd_ < 0) return false;
    size_ = size;
    if (!resize_file(size_)) return false;
    return true;
}

bool PosixMappedFile::open(const std::string& path, std::size_t size,
                           GrowthStrategy growth) {
    close();
    growth_ = growth;
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
}

void PosixMappedFile::close() {
    if (data_) {
        unmap_file();
    }
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
    size_ = 0;
}

void* PosixMappedFile::data() { return data_; }
const void* PosixMappedFile::data() const { return data_; }
std::size_t PosixMappedFile::size() const { return size_; }

bool PosixMappedFile::ensure_size(std::size_t new_size) {
    if (new_size <= size_) return true;
    std::size_t target = new_size;
    if (growth_ == GrowthStrategy::Double) {
        target = std::max(new_size, size_ * 2);
    }
    return resize_file(target);
}

bool PosixMappedFile::map_file(std::size_t map_size) {
    data_ = ::mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
    if (data_ == MAP_FAILED) {
        data_ = nullptr;
        return false;
    }
    size_ = map_size;
    return true;
}

void PosixMappedFile::unmap_file() {
    if (data_) {
        ::munmap(data_, size_);
        data_ = nullptr;
    }
}

bool PosixMappedFile::resize_file(std::size_t new_size) {
    unmap_file();
    if (ftruncate(fd_, new_size) != 0) return false;
    return map_file(new_size);
}

} // namespace shm

