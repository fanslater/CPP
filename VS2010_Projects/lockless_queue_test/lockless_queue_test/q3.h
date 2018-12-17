
#include <stdlib.h>

#include <xmmintrin.h>
#include <emmintrin.h>

#include "test.h"
#include "port.h"
#include "vs_stdint.h"
#include "get_char.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifndef likely
#define likely(x)   __builtin_expect((x), 1)
#endif

#ifndef unlikely
#define unlikely(x) __builtin_expect((x), 0)
#endif

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#pragma warning(push)
#pragma warning(disable: 4200)
#endif  /* _MSC_VER */

// 队列容量取为2的N次幂，这样计算索引时可以使用 index = (指针)sequence & mask; (其中mask = capacity - 1;) 
// 这么干的目的是提高代码效率
// p和c的值不是同步 volatile
struct queue {
    struct {
        uint32_t mask;
        uint32_t size;
        volatile uint32_t head;
        volatile uint32_t tail;
    } p;
    char pad1[CACHE_LINE_SIZE - 4 * sizeof(uint32_t)];

    struct {
        uint32_t mask;
        uint32_t size;
        volatile uint32_t head;
        volatile uint32_t tail;
    } c;
    char pad2[CACHE_LINE_SIZE - 4 * sizeof(uint32_t)];

    void *msgs[0];
};

static inline struct queue *
qinit(void)
{
    struct queue *q = (struct queue *)calloc(1, sizeof(*q) + QSIZE * sizeof(void *));
    q->p.size = q->c.size = QSIZE;
    q->p.mask = q->c.mask = QMASK;

    return q;
}

static inline void
qfree(struct queue *q)
{
    if (q != NULL)
        free(q);
}

static inline int
push(struct queue *q, void *m)
{
    uint32_t head, tail, mask, next;
    int ok;

    mask = q->p.mask;

    do {
        head = q->p.head;
        tail = q->c.tail;
		//知识点1U
        if ((mask + tail - head) < 1U)      // 这样用表示队列最大长度为mask, 而不是(mask + 1)
        //if ((head - tail) > mask)
            return -1;
        next = head + 1;
        ok = jimi_bool_compare_and_swap32(&q->p.head, head, next);
    } while (!ok);

    q->msgs[head & mask] = m;

#ifdef _MSC_VER
    Jimi_CompilerBarrier();
#else
    asm volatile ("":::"memory");
#endif

    while (jimi_unlikely((q->p.tail != head))) {
        jimi_mm_pause();
/*
//PAUSE指令提升了自旋等待循环（spin-wait loop）的性能
当执行一个循环等待时，Intel P4或Intel Xeon处理器会因为检测到一个可能的内存顺序违规（memory order violation）而在退出循环时使性能大幅下降。
PAUSE指令给处理器提了个醒：这段代码序列是个循环等待。处理器利用这个提示可以避免在大多数情况下的内存顺序违规，这将大幅提升性能。
因为这个原因，所以推荐在循环等待中使用PAUSE指令。
*/
    }

    q->p.tail = next;

    return 0;
}

static inline void *
pop(struct queue *q)
{
    uint32_t head, tail, mask, next;
    int ok;
    volatile void *ret;

    mask = q->c.mask;

    do {
        head = q->c.head;
        tail = q->p.tail;
        if ((tail - head) < 1U)
        //if (head == tail || (head > tail && (tail - head) > mask))
            return NULL;
        next = head + 1;
        ok = jimi_bool_compare_and_swap32(&q->c.head, head, next);
    } while (!ok);

    ret = q->msgs[head & mask];

#ifdef _MSC_VER
    Jimi_CompilerBarrier();
#else
    asm volatile ("":::"memory");
#endif

    while (jimi_unlikely((q->c.tail != head))) {
        jimi_mm_pause();
    }

    q->c.tail = next;

    return (void *)ret;
}

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#pragma warning(pop)
#endif  /* _MSC_VER */
