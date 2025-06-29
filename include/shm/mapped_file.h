#ifndef SHM_MAPPED_FILE_H
#define SHM_MAPPED_FILE_H

#include <cstddef>
#include <memory>
#include <string>

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
};

} // namespace shm

#endif // SHM_MAPPED_FILE_H
