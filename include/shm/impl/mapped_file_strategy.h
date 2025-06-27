#ifndef SHM_MAPPED_FILE_STRATEGY_H
#define SHM_MAPPED_FILE_STRATEGY_H

#include <cstddef>
#include <string>

namespace shm {

enum class GrowthStrategy {
    Fixed,
    Double
};

class MappedFileStrategy {
public:
    virtual ~MappedFileStrategy() = default;
    virtual bool create(const std::string& path, std::size_t size,
                        GrowthStrategy growth) = 0;
    virtual bool open(const std::string& path, std::size_t size,
                      GrowthStrategy growth) = 0;
    virtual void close() = 0;
    virtual void* data() = 0;
    virtual const void* data() const = 0;
    virtual std::size_t size() const = 0;
    virtual bool ensure_size(std::size_t new_size) = 0;
};

} // namespace shm

#endif // SHM_MAPPED_FILE_STRATEGY_H
