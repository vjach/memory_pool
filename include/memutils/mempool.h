#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <memory>

namespace memutils {

template <size_t blocks, size_t block_size>
class MemoryPool {
   public:
    MemoryPool() : free_block_{nullptr}, untouched_blocks_{blocks} {}

    auto Allocate() {
        auto free_callback = [this](void* address) {
            if (address != nullptr) {
                auto ptr = reinterpret_cast<uint8_t*>(address);
                auto block_addr =
                    reinterpret_cast<BlockHeader*>(ptr - sizeof(BlockHeader));
                block_addr->next = free_block_;
                free_block_ = block_addr;
            }
        };

        using return_type = std::unique_ptr<void, decltype(free_callback)>;

        if (free_block_) {
            auto block = free_block_;
            free_block_ = free_block_->next;
            return return_type{reinterpret_cast<void*>(block->data),
                               free_callback};
        }

        if (untouched_blocks_) {
            --untouched_blocks_;
            auto block = reinterpret_cast<BlockHeader*>(
                arena_ + total_block_size * untouched_blocks_);
            return return_type{reinterpret_cast<void*>(block->data),
                               free_callback};
        }

        return return_type{nullptr, free_callback};
    }

    constexpr const uint16_t Size() const { return blocks; }
    constexpr const uint16_t BlockSize() const { return block_size; }

   private:
    struct BlockHeader {
        struct BlockHeader* next;
        uint8_t data[0];
    } __attribute__((packed));

    static constexpr const size_t total_block_size =
        block_size + sizeof(BlockHeader);
    uint8_t arena_[total_block_size * blocks];

    BlockHeader* free_block_;
    size_t untouched_blocks_;
};

template <uint16_t chunks, uint16_t chunk_size>
using UniqueMemoryPtr =
    decltype(((MemoryPool<chunks, chunk_size>*)nullptr)->Allocate());

}  // namespace memutils

#endif
