#ifndef SHM_MAPPED_FILE_WINDOWS_H
#define SHM_MAPPED_FILE_WINDOWS_H

#include "shm/impl/mapped_file_strategy.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <algorithm>
#include <cstring>

namespace shm {

class WindowsMappedFile : public MappedFileStrategy {
public:
    WindowsMappedFile();
    ~WindowsMappedFile() override;

    bool create(const std::string& path, std::size_t size,
                GrowthStrategy growth) override;
    bool open(const std::string& path, std::size_t size,
              GrowthStrategy growth) override;
    void close() override;
    void* data() override;
    const void* data() const override;
    std::size_t size() const override;
    bool ensure_size(std::size_t new_size) override;

private:
    GrowthStrategy growth_ = GrowthStrategy::Fixed;
    void* file_ = nullptr;   // HANDLE
    void* mapping_ = nullptr; // HANDLE
    void* data_ = nullptr;
    std::size_t size_ = 0;

    bool map_file(std::size_t size);
    void unmap_file();
    bool resize_file(std::size_t new_size);
};

} // namespace shm

#endif // SHM_MAPPED_FILE_WINDOWS_H
