#ifndef __PS_STATIC_QUEUE_H__
#define __PS_STATIC_QUEUE_H__

#include <cstdint>
#include <cstdlib>
#include <atomic>

namespace ps
{

/**
 * @brief 简单的定长环形缓冲区实现，只保留最近接收的一个数据
 */
class SPSCStaticQueue
{
public:
    /**
     * @brief 缓冲区实现
     */
    struct Buffer
    {
        size_t length;
        uint8_t *ptr;
    };

    /**
     * @brief 初始化队列，如果buffer为NULL，自行分配
     */
    SPSCStaticQueue(uint8_t *(buffer[3]), uint8_t max_length)
        : max_len {max_length}
    {
        if (!buffer)
        {
            inner_buffer = true;
            buffer[0] = new uint8_t[max_length];
            buffer[1] = new uint8_t[max_length];
            buffer[2] = new uint8_t[max_length];
        }
        for (int x=0; x<3; x++)
        {
            auto *tmp = new Buffer{.length = 0, .ptr = buffer[x]};
            this->buffer[x].store(tmp);
        }
    };

    ~SPSCStaticQueue()
    {
        if (inner_buffer)
            for (int x=0; x<3; x++) delete buffer[x].load()->ptr;
    }

    /**
     * @brief 清空缓冲区长度计数
     * @note 线程不安全，必须外部保证互斥
     */
    void clear()
    {
        for (int x=0; x<3; x++)
        {
            Buffer *bf = buffer[x].load();
            bf->length = 0;
        }
    }

    /**
     * @brief 获取当前可以用于写入的缓冲区
     */
    Buffer* write()
    {
        return buffer[2].load();
    }

    /**
     * @brief 写入结束，保存当前缓冲区
     */
    void store(size_t length)
    {
        // 如果超长，不要处理
        if (length <= max_len)
        {
            // 写入数据
            Buffer *bf = buffer[2].load();
            bf->length = length;
            // 交换缓冲区
            lock_free_swap(buffer[1], buffer[2]);
        }
    }

    /**
     * @brief 读取数据
     */
    Buffer read()
    {
        // 读取数据
        Buffer *load = buffer[0].load();
        Buffer ret = *load;
        // 交换缓冲区
        load->length = 0;
        load->ptr = NULL;
        lock_free_swap(buffer[1], buffer[0]);

        return ret;
    }

    const size_t max_len;
private:

    /**
     * @brief 无锁交换两个变量
     */
    void lock_free_swap(std::atomic<Buffer*>& a, std::atomic<Buffer*>& b)
    {
        Buffer *expected_a, *expected_b;
        do {
            expected_a = a.load(std::memory_order_relaxed),
            expected_b = b.load(std::memory_order_relaxed);
        } while (   !a.compare_exchange_weak(expected_a, expected_b, std::memory_order_acq_rel)
                 || !b.compare_exchange_weak(expected_b, expected_a, std::memory_order_acq_rel));
    }

    /**
     * @brief 保证的是缓冲区指针的互斥性，不需要保证缓冲区内容的互斥性，因为写/读都只会访问
     * 自己对应的缓冲区内容
     */
    std::atomic<Buffer*> buffer[3];
    bool inner_buffer = false; // 缓冲区内部管理标志位，如果置位，说明缓冲区是初始化时分配的
};

}
#endif