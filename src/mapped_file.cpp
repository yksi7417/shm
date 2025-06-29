#include "shm/mapped_file.h"
#include <filesystem>
#include <cstdio>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

namespace fs = std::filesystem;


namespace shm {

MappedFile::MappedFile() : impl_(std::make_unique<DefaultMappedFileImpl>()) {}

MappedFile::~MappedFile() { close(); }

bool MappedFile::create(const std::string& path, std::size_t size,
                        GrowthStrategy growth) {
    if (!impl_->create(path, size, growth)) {
        return false;
    }
    path_ = path;
    register_reader();
    return true;
}

bool MappedFile::open(const std::string& path, std::size_t size,
                      GrowthStrategy growth) {
    if (!impl_->open(path, size, growth)) {
        return false;
    }
    path_ = path;
    register_reader();
    return true;
}

void MappedFile::close() {
    std::size_t sz = impl_->size();
    impl_->close();
    decrement_reader(sz);
}

void MappedFile::register_reader() {
    std::string lock_path = path_ + ".lock";
    lock_fd_ = ::open(lock_path.c_str(), O_RDWR | O_CREAT, 0666);
    if (lock_fd_ >= 0) {
        flock(lock_fd_, LOCK_SH);
    }
}

void* MappedFile::data() { return impl_->data(); }
const void* MappedFile::data() const { return impl_->data(); }
std::size_t MappedFile::size() const { return impl_->size(); }

bool MappedFile::ensure_size(std::size_t new_size) {
    return impl_->ensure_size(new_size);
}

void MappedFile::decrement_reader(std::size_t last_size) {
    if (path_.empty() || lock_fd_ < 0) {
        path_.clear();
        return;
    }

    std::string lock_path = path_ + ".lock";

    flock(lock_fd_, LOCK_UN);
    if (flock(lock_fd_, LOCK_EX | LOCK_NB) == 0) {
        if (last_size >= kMoveThreshold) {
            std::string new_path = path_ + ".moved";
            std::rename(path_.c_str(), new_path.c_str());
        }
        ::unlink(lock_path.c_str());
        flock(lock_fd_, LOCK_UN);
    }

    ::close(lock_fd_);
    lock_fd_ = -1;
    path_.clear();
}

} // namespace shm

