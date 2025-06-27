#include "shm/mapped_file.h"

namespace shm {

MappedFile::MappedFile() : impl_(std::make_unique<DefaultMappedFileImpl>()) {}

MappedFile::~MappedFile() = default;

bool MappedFile::create(const std::string& path, std::size_t size,
                        GrowthStrategy growth) {
    return impl_->create(path, size, growth);
}

bool MappedFile::open(const std::string& path, std::size_t size,
                      GrowthStrategy growth) {
    return impl_->open(path, size, growth);
}

void MappedFile::close() { impl_->close(); }

void* MappedFile::data() { return impl_->data(); }
const void* MappedFile::data() const { return impl_->data(); }
std::size_t MappedFile::size() const { return impl_->size(); }

bool MappedFile::ensure_size(std::size_t new_size) {
    return impl_->ensure_size(new_size);
}

} // namespace shm

