#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// 环形缓冲区
#define BUF_SIZE 5
#define uint8_t unsigned char

typedef struct
{
    uint8_t buf[BUF_SIZE];
    int head;
    int tail;
    bool isFull;
} RingBuffer;

// 初始化环形缓冲区
void RingBuffer_Init(RingBuffer *rb)
{
    for (int i = 0; i < BUF_SIZE; ++i)
    {
        rb->buf[i] = 0;
    }
    rb->head = 0;
    rb->tail = 0;
    rb->isFull = false;
}

// 向环形缓冲区中写入数据
bool RingBuffer_Write(RingBuffer *rb, uint8_t data)
{
    if (rb->isFull)
    {
        return false; // 缓冲区已满，无法写入数据
    }

    rb->buf[rb->tail] = data;
    rb->tail = (rb->tail + 1) % BUF_SIZE;

    if (rb->head == rb->tail)
    {
        rb->isFull = true;
    }

    return true;
}

// 从环形缓冲区中读取数据
bool RingBuffer_Read(RingBuffer *rb, uint8_t *data)
{
    if (rb->head == rb->tail && !rb->isFull)
    {
        return false; // 缓冲区为空，无法读取数据
    }

    *data = rb->buf[rb->head];
    rb->head = (rb->head + 1) % BUF_SIZE;

    if (!rb->isFull && rb->head == rb->tail)
    {
        rb->isFull = false;
    }

    return true;
}

/**
 * @brief 主函数
 *
 * 此函数演示了如何使用环形缓冲区（RingBuffer）来写入和读取数据。
 *
 * @return 0 表示程序正常结束
 */
int main()
{
    RingBuffer rb;
    RingBuffer_Init(&rb);

    // 写入数据
    for (int i = 0; i < BUF_SIZE; ++i)
    {
        RingBuffer_Write(&rb, i);
    }

    // 读取数据并打印
    uint8_t data;
    for (int i = 0; i < BUF_SIZE; ++i)
    {
        if (RingBuffer_Read(&rb, &data))
        {
            printf("%d\n", data);
        }
        else
        {
            printf("Data is empyt\n");
            break;
        }
    }

    return 0;
}