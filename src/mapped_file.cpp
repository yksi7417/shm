#include "shm/mapped_file.h"
#include <filesystem>
#include <unordered_map>
#include <cstdio>

namespace fs = std::filesystem;

std::unordered_map<std::string, std::size_t> shm::MappedFile::reader_counts_;

namespace shm {

MappedFile::MappedFile() : impl_(std::make_unique<DefaultMappedFileImpl>()) {}

MappedFile::~MappedFile() { close(); }

bool MappedFile::create(const std::string& path, std::size_t size,
                        GrowthStrategy growth) {
    if (!impl_->create(path, size, growth)) {
        return false;
    }
    path_ = path;
    ++reader_counts_[path_];
    return true;
}

bool MappedFile::open(const std::string& path, std::size_t size,
                      GrowthStrategy growth) {
    if (!impl_->open(path, size, growth)) {
        return false;
    }
    path_ = path;
    ++reader_counts_[path_];
    return true;
}

void MappedFile::close() {
    impl_->close();
    decrement_reader();
}

void* MappedFile::data() { return impl_->data(); }
const void* MappedFile::data() const { return impl_->data(); }
std::size_t MappedFile::size() const { return impl_->size(); }

bool MappedFile::ensure_size(std::size_t new_size) {
    return impl_->ensure_size(new_size);
}

void MappedFile::decrement_reader() {
    if (path_.empty()) return;
    auto it = reader_counts_.find(path_);
    if (it == reader_counts_.end()) {
        path_.clear();
        return;
    }
    if (--it->second == 0) {
        reader_counts_.erase(it);
        std::string new_path = path_ + ".moved";
        std::rename(path_.c_str(), new_path.c_str());
    }
    path_.clear();
}

} // namespace shm

