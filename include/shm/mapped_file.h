#ifndef SHM_MAPPED_FILE_H
#define SHM_MAPPED_FILE_H

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

#include "shm/impl/mapped_file_strategy.h"
#include "shm/impl/mapped_file_selector.h"

namespace shm {

class MappedFile {
public:
    MappedFile();
    ~MappedFile();

    bool create(const std::string& path, std::size_t size,
                GrowthStrategy growth = GrowthStrategy::Fixed);
    bool open(const std::string& path, std::size_t size = 0,
              GrowthStrategy growth = GrowthStrategy::Fixed);
    void close();
    void* data();
    const void* data() const;
    std::size_t size() const;
    bool ensure_size(std::size_t new_size);

private:
    std::unique_ptr<MappedFileStrategy> impl_;
    std::string path_;
    static std::unordered_map<std::string, std::size_t> reader_counts_;
    static constexpr std::size_t kMoveThreshold = 1024;

    void decrement_reader(std::size_t last_size);
};

} // namespace shm

#endif // SHM_MAPPED_FILE_H
