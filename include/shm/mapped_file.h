#ifndef SHM_MAPPED_FILE_H
#define SHM_MAPPED_FILE_H

#include <cstddef>
#include <string>

namespace shm {

class MappedFile {
public:
    enum class GrowthStrategy {
        Fixed,
        Double
    };

    MappedFile();
    ~MappedFile();

    // Create a new file and map it. Existing file will be truncated.
    bool create(const std::string& path, std::size_t size,
                GrowthStrategy growth = GrowthStrategy::Fixed);

    // Open an existing file and map it. If size is non-zero and the file is
    // smaller, it will be extended.
    bool open(const std::string& path, std::size_t size = 0,
              GrowthStrategy growth = GrowthStrategy::Fixed);

    // Unmap and close the file.
    void close();

    // Pointer to mapped data.
    void* data();
    const void* data() const;

    // Size of the mapped region.
    std::size_t size() const;

    // Ensure capacity at least new_size using the growth strategy.
    bool ensure_size(std::size_t new_size);

private:
    GrowthStrategy growth_;
#ifdef _WIN32
    void* file_ = nullptr; // HANDLE
    void* mapping_ = nullptr; // HANDLE
#else
    int fd_ = -1;
#endif
    void* data_ = nullptr;
    std::size_t size_ = 0;

    bool map_file(std::size_t size);
    void unmap_file();
    bool resize_file(std::size_t new_size);
};

} // namespace shm

#endif // SHM_MAPPED_FILE_H
