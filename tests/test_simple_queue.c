#include "data_structures.h"
#include <assert.h>
#include <stdio.h>

void test_init()
{
    simple_queue q;
    assert(init_simple_queue(5, &q) == 1);
    assert(q.N == 5);
    assert(q.front == -1);
    assert(q.rear == -1);
    destroy_simple_queue(&q);

    printf("Simple queue init test passed\n");
}

void test_basic_enqueue_dequeue()
{
    simple_queue q;
    init_simple_queue(5, &q);

    assert(enqueue_simple(&q, 30) == 1);
    assert(enqueue_simple(&q, 20) == 1);

    // FIFO: first in, first out
    assert(dequeue_simple(&q) == 30);
    assert(dequeue_simple(&q) == 20);

    destroy_simple_queue(&q);

    printf("Simple queue basic enqueue/dequeue test passed\n");
}

void test_underflow()
{
    simple_queue q;
    init_simple_queue(5, &q);

    assert(dequeue_simple(&q) == -1);

    destroy_simple_queue(&q);

    printf("Simple queue underflow test passed\n");
}

void test_overflow()
{
    simple_queue q;
    init_simple_queue(3, &q);

    assert(enqueue_simple(&q, 1) == 1);
    assert(enqueue_simple(&q, 2) == 1);
    assert(enqueue_simple(&q, 3) == 1);

    // capacity reached: rear is at the last slot
    assert(enqueue_simple(&q, 4) == -1);

    destroy_simple_queue(&q);

    printf("Simple queue overflow test passed\n");
}

void test_false_overflow()
{
    // The defining limitation of a linear queue: once rear reaches the last slot the queue
    // is full even after the front has been dequeued. The freed slots are NOT reusable
    // (unlike a circular queue, which wraps around). This test pins that behaviour.
    simple_queue q;
    init_simple_queue(3, &q);

    enqueue_simple(&q, 1);
    enqueue_simple(&q, 2);
    enqueue_simple(&q, 3); // rear == N-1, queue full

    assert(dequeue_simple(&q) == 1);
    assert(dequeue_simple(&q) == 2); // two front slots are now free

    // Only one element (3) remains in a capacity-3 queue, yet enqueue still fails:
    // rear cannot move past N-1 and the freed front slots cannot be reclaimed.
    assert(enqueue_simple(&q, 4) == -1);

    // The remaining element is still retrievable in order, then the queue is empty.
    assert(dequeue_simple(&q) == 3);
    assert(dequeue_simple(&q) == -1);

    destroy_simple_queue(&q);

    printf("Simple queue false-overflow (linear limitation) test passed\n");
}

void test_fifo_order()
{
    simple_queue q;
    init_simple_queue(4, &q);

    for (int i = 1; i <= 4; i++)
        assert(enqueue_simple(&q, i * 10) == 1);

    for (int i = 1; i <= 4; i++)
        assert(dequeue_simple(&q) == i * 10);

    destroy_simple_queue(&q);

    printf("Simple queue FIFO order test passed\n");
}

int main()
{
    test_init();
    test_basic_enqueue_dequeue();
    test_underflow();
    test_overflow();
    test_false_overflow();
    test_fifo_order();

    printf("All simple queue tests passed\n");
    return 0;
}
