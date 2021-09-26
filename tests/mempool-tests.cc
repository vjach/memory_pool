#include <queue>

#include "gtest/gtest.h"
#include "memutils/mempool.h"

using namespace memutils;

using Pool = MemoryPool<16, 1024>;

TEST(MempoolTest, CanCreateIntsance) { MemoryPool<16, 1024> pool; }
TEST(MempoolTest, CanAllocate) {
    Pool pool;
    {
        auto chunk = pool.Allocate();
        EXPECT_TRUE(chunk != nullptr);
    }
}

TEST(MempoolTest, AllocateAll) {
    Pool pool;
    auto exhaust_pool = [&pool]() {
        std::queue<Pool::UniqueMemoryPtr> queue;
        for (auto i = 0; i < pool.Size(); ++i) {
            auto chunk = pool.Allocate();
            EXPECT_TRUE(chunk != nullptr);
            queue.push(std::move(chunk));
        }

        EXPECT_TRUE(pool.Allocate() == nullptr);
    };

    exhaust_pool();
    exhaust_pool();
}

TEST(MempoolTest, NoDataCorruption) {
    Pool pool;
    std::queue<Pool::UniqueMemoryPtr> queue;
    for (auto i = 0; i < pool.Size(); ++i) {
        auto chunk = pool.Allocate();
        EXPECT_TRUE(chunk != nullptr);
        memset(chunk.get(), (uint8_t)i, pool.BlockSize());
        queue.push(std::move(chunk));
    }

    auto is_all = [](uint8_t* ptr, uint8_t value, unsigned size) -> bool {
        bool ret = true;
        for (unsigned i = 0; i < size; ++i) {
            ret = ret and (ptr[i] == value);
        }

        return ret;
    };

    for (auto i = 0; i < pool.Size(); ++i) {
        auto chunk = std::move(queue.front());
        queue.pop();
        EXPECT_TRUE(is_all(reinterpret_cast<uint8_t*>(chunk.get()), i,
                           pool.BlockSize()));
    }
}

