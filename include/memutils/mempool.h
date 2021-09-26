#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <memory>

namespace memutils {

template <size_t blocks, size_t block_size>
class MemoryPool {
 private:
  struct Deleter {
    Deleter(MemoryPool& pool) : pool{pool} {}
    void operator()(void* address) {
      if (address != nullptr) {
        auto ptr = reinterpret_cast<uint8_t*>(address);
        auto block_addr =
            reinterpret_cast<BlockHeader*>(ptr - sizeof(BlockHeader));
        block_addr->next = pool.free_block_;
        pool.free_block_ = block_addr;
      }
    }

    MemoryPool& pool;
  };

 public:
  using UniqueMemoryPtr = std::unique_ptr<void, Deleter>;
  MemoryPool() : free_block_{nullptr}, untouched_blocks_{blocks} {}

  auto Allocate() {
    if (free_block_) {
      auto block = free_block_;
      free_block_ = free_block_->next;
      return UniqueMemoryPtr{reinterpret_cast<void*>(block->data),
                             Deleter(*this)};
    }

    if (untouched_blocks_) {
      --untouched_blocks_;
      auto block = reinterpret_cast<BlockHeader*>(
          arena_ + total_block_size * untouched_blocks_);
      return UniqueMemoryPtr{reinterpret_cast<void*>(block->data),
                             Deleter(*this)};
    }

    return UniqueMemoryPtr{nullptr, Deleter(*this)};
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

}  // namespace memutils

#endif
